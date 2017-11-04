//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.2
*
*	Header for the definition of a generic cube object
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "World.hpp"
#include "cout-definitions.hpp"

#define MESH Cube::meshes[0]
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Global Values
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
vector<GLuint> cube_VBO_types = {GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, 
    GL_SHADER_STORAGE_BUFFER, GL_ELEMENT_ARRAY_BUFFER};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Chunk Class implementation:
*/
//========================================================================================

/*
*   Class Constructors
*/
Chunk::Chunk() : Chunk(vec3(0)){}

Chunk::Chunk(vec3 offset) : Chunk(offset, NULL){}

Chunk::Chunk(vec3 offset, World* w) : Chunk(offset, w, 0,0,0){}

Chunk::Chunk(vec3 offset, World* w, int x, int j, int k)
{
    //Set postion in the world array and world container
    i_pos=x, j_pos=j, k_pos=k;
    world = w;
    //Initialize all cubes in the chunk, with correct positions
    //offset represents the position of the chunk in the world
    for(int i=0; i<CHUNK_DIMENSIONS*CHUNK_DIMENSIONS*CHUNK_DIMENSIONS; i++)
    {
        chunk_cubes[i] = 
            new Cube(vec3(i/(CHUNK_DIMENSIONS*CHUNK_DIMENSIONS) + offset[0], 
            (i/CHUNK_DIMENSIONS) % CHUNK_DIMENSIONS + offset[1], 
            i%CHUNK_DIMENSIONS + offset[2]));
    }

    //Create and initialize OpenGL rendering structures
    cube_rendering_VBOs = vector<GLuint>(5);
    glGenVertexArrays(1, &world_cubes_VAO);
    glGenBuffers(5,cube_rendering_VBOs.data());

    glBindVertexArray(world_cubes_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, cube_rendering_VBOs[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
    glBufferData(GL_ARRAY_BUFFER, MESH->vertices->size()*sizeof(vec3), 
    MESH->vertices->data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, cube_rendering_VBOs[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vec3), (void*)0);
    glBufferData(GL_ARRAY_BUFFER, MESH->normals->size()*sizeof(vec3),
    MESH->normals->data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, cube_rendering_VBOs[2]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
    glBufferData(GL_ARRAY_BUFFER, MESH->uvs->size()*sizeof(vec2),
        MESH->uvs->data(), GL_DYNAMIC_DRAW);
}

/*
* Class destructor
*/
Chunk::~Chunk()
{
    for(int i=0; i<CHUNK_DIMENSIONS*CHUNK_DIMENSIONS*CHUNK_DIMENSIONS; i++)
    {
       delete(chunk_cubes[i]);
    }

    /*glDeleteBuffers(5, cube_rendering_VBOs.data());
    glDeleteVertexArrays(1, &world_cubes_VAO);*/
}

/*
* Inline function, calculate the relevant coordinates for cube fetching in 1D
*/
inline void calculate_coordinates(int x, int &chunk_x, 
    int &cube_x, bool &out_of_bounds)
{
    if(x<0 || x>=CHUNK_DIMENSIONS)
    {
        //Set flag to indicat the cube is not in the current chunk
        out_of_bounds=true;
        if(x>=0)
        {
            chunk_x+=x/CHUNK_DIMENSIONS; //Index of the neighbouring chunk
            cube_x=x%CHUNK_DIMENSIONS; //Index of the neighbouring cube
        }
        else
        {
            chunk_x+=-x/CHUNK_DIMENSIONS -1; //Index of the neighbouring chunk
            cube_x=x%CHUNK_DIMENSIONS + CHUNK_DIMENSIONS; //Index of the neighbouring cube
        }
    }
}

/*
*   () operator overloading, used to fetch cube values in the chunk and the world
*/
Cube* Chunk::operator()(int x, int y, int z)
{
    //Initialize default values
    int chunk_x=this->i_pos, chunk_y=this->j_pos,chunk_z=this->k_pos;
    int cube_x=x,cube_y=y,cube_z=z;
    bool out_of_bounds=false;

    //Find the correct chunk coordinate sin the world and cube
    //Cooordinates inside of the correct chunk
    calculate_coordinates(x, chunk_x, cube_x, out_of_bounds);
    calculate_coordinates(y, chunk_y, cube_y, out_of_bounds);
    calculate_coordinates(z, chunk_z, cube_z, out_of_bounds);

    //Find a cube that is not in the current chunk
    if(out_of_bounds)
    {
        if(world == NULL)
            return NULL;
        //Fetch correct chunk
        Chunk* c = (*world)(chunk_x,chunk_y,chunk_z);
        if(c==NULL)
            return NULL;
        //Fetch correct cube
        return (*c)(cube_x,cube_y,cube_z);
    }
    else
        return chunk_cubes[cube_x*CHUNK_DIMENSIONS*CHUNK_DIMENSIONS
            +cube_y*CHUNK_DIMENSIONS+cube_z];   
}

/*
*   Update the current chunk
*/
void Chunk::update()
{
    update_visible_faces();
    update_render_info();
}

/*
*   Send rendering informationto the rendering handler
*/
//TODO: Maybe delete this and have the handler fetch the information directly
void Chunk::render_chunk()
{
    Rendering_Handler->multi_render(world_cubes_VAO, &cube_rendering_VBOs, 
        &cube_VBO_types, 4, MESH->indices->size(),faces_info.size());
}

//Help macro, correctly updates the visible faces info
#define CHECK_NEIGHBOUR(c,n,f) if(n==NULL || n->transparent)\
faces_info.push_back(vec4(c->position,f));

/*
*   Set the face_info array with the data of the currently visible faces
*/
void Chunk::update_visible_faces()
{
    for(int i=0; i<CHUNK_DIMENSIONS; i++)
    {
        for(int j=0; j<CHUNK_DIMENSIONS; j++)
        {
            for(int k=0; k<CHUNK_DIMENSIONS; k++)
            {
                //fetch current cube
                Cube* current = (*this)(i,j,k);

                //fetch all 6 neighbours and update accordingly
                Cube* neighbour = (*this)(i-1,j,k);
                CHECK_NEIGHBOUR(current, neighbour, Left)
                neighbour = (*this)(i+1,j,k);
                CHECK_NEIGHBOUR(current, neighbour, Right)
                neighbour = (*this)(i,j+1,k);
                CHECK_NEIGHBOUR(current, neighbour, Front)
                neighbour = (*this)(i,j-1,k);
                CHECK_NEIGHBOUR(current, neighbour, Back)
                neighbour = (*this)(i,j,k+1);
                CHECK_NEIGHBOUR(current, neighbour, Top)
                neighbour = (*this)(i,j,k-1);
                CHECK_NEIGHBOUR(current, neighbour, Bottom)
            }
        }
    }
}

/*
*   Update chunk OpenGL rendering information
*/
void Chunk::update_render_info()
{
    glBindVertexArray(world_cubes_VAO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, cube_rendering_VBOs[3]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, faces_info.size()*sizeof(vec4), faces_info.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, cube_rendering_VBOs[3]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_rendering_VBOs[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MESH->indices->size()*sizeof(uint),
        MESH->indices->data(), GL_DYNAMIC_DRAW);
}
//########################################################################################

//========================================================================================
/*
*	World Class implementation:
*/
//========================================================================================

/*
*   World default constructor
*/
World::World()
{    
    //Allocate memory for the loaded chunks
    loaded_chunks = (Chunk****)malloc(load_distance*sizeof(Chunk***));
    for(int i=0; i<load_distance; i++)
    {
        loaded_chunks[i] = (Chunk***)malloc(load_distance*sizeof(Chunk**));
        for(int j=0; j<load_distance; j++)
        {
            loaded_chunks[i][j] = (Chunk**)malloc(load_distance*sizeof(Chunk*));
        }
    }

    //Initialize loaded chunks
    for(int i=0; i<load_distance; i++)
    {
        for(int j=0; j<load_distance; j++)
        {
            for(int k=0; k<load_distance; k++)
            {
                loaded_chunks[i][j][k] = 
                new Chunk
                (
                    vec3(i*CHUNK_DIMENSIONS,j*CHUNK_DIMENSIONS,k*CHUNK_DIMENSIONS), 
                    this,
                    i,j,k
                );
            }
        }
    }

    //First chunk update, to assert all values
    for(int i=0; i<load_distance; i++)
    {
        for(int j=0; j<load_distance; j++)
        {
            for(int k=0; k<load_distance; k++)
            {
               loaded_chunks[i][j][k]->update();
            }
        }
    }
}

/*
* World class destructor
*/
World::~World()
{
    //Delete chunks
    for(int i=0; i<load_distance; i++)
    {
        for(int j=0; j<load_distance; j++)
        {
            for(int k=0; k<load_distance; k++)
            {
                delete(loaded_chunks[i][j][k]);
            }
        }
    }

    //Free allocated memory
    for(int i=0; i<load_distance; i++)
    {
        for(int j=0; j<load_distance; j++)
        {
            free(loaded_chunks[i][j]);
        }
        free(loaded_chunks[i]);
    }
    free(loaded_chunks);
}

/*
*   Overloaded () operator, used to get chunk pointers in the loaded chunks
*   of the world
*/
Chunk* World::operator()(int x, int y, int z)
{
    //Check out of bounds conditions and return  NULL when needed
    if(x<0 || x>=load_distance)
        return NULL;
    if(y<0 || y>=load_distance)
        return NULL;
    if(z<0 || z>=load_distance)
        return NULL;

    //return the requested cube if it's within bounds
    return loaded_chunks[x][y][z];
}

/*
* Render all world chunks
*/
void World::render_world()
{
   for(int i=0; i<load_distance; i++)
   {
       for(int j=0; j<load_distance; j++)
       {
           for(int k=0; k<load_distance; k++)
           {
               Chunk* c = loaded_chunks[i][j][k];
               c->render_chunk();
           }
       }
   }
}
//########################################################################################