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

Chunk::Chunk(vec3 offset, World* w) 
{
    world = w;
    //Initialize all cubes in the chunk, with correct positions
    //offset represents the position of the chunk in the world
    for(int i=0; i<CHUNK_DIMS*CHUNK_DIMS*CHUNK_DIMS; i++)
    {
        chunk_cubes[i] = 
            new Cube(vec3(i/(CHUNK_DIMS*CHUNK_DIMS) + offset[0], 
            (i/CHUNK_DIMS) % CHUNK_DIMS + offset[1], 
            i%CHUNK_DIMS + offset[2]));
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
    for(int i=0; i<CHUNK_DIMS*CHUNK_DIMS*CHUNK_DIMS; i++)
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
    if(x<0 || x>=CHUNK_DIMS)
    {
        //Set flag to indicat the cube is not in the current chunk
        out_of_bounds=true;
        if(x>=0)
        {
            chunk_x+=x/CHUNK_DIMS; //Index of the neighbouring chunk
            cube_x=x%CHUNK_DIMS; //Index of the neighbouring cube
        }
        else
        {
            chunk_x+=-x/CHUNK_DIMS -1; //Index of the neighbouring chunk
            cube_x=x%CHUNK_DIMS + CHUNK_DIMS; //Index of the neighbouring cube
        }
    }
}

/*
*   () operator overloading, used to fetch cube values in the chunk and the world
*/
Cube* Chunk::operator()(int x, int y, int z)
{
    if(x<0 || x>=CHUNK_DIMS)
    {
        cerr << "Cube out of bounds request" << endl;
        exit(0);
    }
    if(y<0 || y>=CHUNK_DIMS)
    {
        cerr << "Cube out of bounds request" << endl;
        exit(0);
    }
    if(z<0 || z>=CHUNK_DIMS)
    {
        cerr << "Cube out of bounds request" << endl;
        exit(0);
    }

    return chunk_cubes[x*CHUNK_DIMS*CHUNK_DIMS+y*CHUNK_DIMS+z];   
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
    for(int i=0; i<CHUNK_DIMS; i++)
    {
        for(int j=0; j<CHUNK_DIMS; j++)
        {
            for(int k=0; k<CHUNK_DIMS; k++)
            {
                //fetch current cube
                Cube* current = (*this)(i,j,k);
                if(current==NULL)
                cout << "wtf" << endl;
                int c_x=(int)(current->position.x);
                int c_y=(int)(current->position.y); 
                int c_z=(int)(current->position.z);
                //int c_x=0,c_y=0,c_z=0;
                //cout << c_x <<  " " << c_y << " " << c_z << endl;
                //fetch all 6 neighbours and update accordingly
                Cube* neighbour = (*world)(c_x-1,c_y,c_z);
                CHECK_NEIGHBOUR(current, neighbour, Left);
                neighbour = (*world)(c_x+1,c_y,c_z);
                CHECK_NEIGHBOUR(current, neighbour, Right);
                neighbour = (*world)(c_x,c_y+1,c_z);
                CHECK_NEIGHBOUR(current, neighbour, Front);
                neighbour = (*world)(c_x,c_y-1,c_z);
                CHECK_NEIGHBOUR(current, neighbour, Back);
                neighbour = (*world)(c_x,c_y,c_z+1);
                CHECK_NEIGHBOUR(current, neighbour, Top);
                neighbour = (*world)(c_x,c_y,c_z-1);
                CHECK_NEIGHBOUR(current, neighbour, Bottom);
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
                    new Chunk(vec3(i*CHUNK_DIMS,j*CHUNK_DIMS,k*CHUNK_DIMS), this);
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
Cube* World::operator()(int x, int y, int z)
{
    x-=origin.x, y-=origin.y,z-=origin.z;
    int chunk_x = x/CHUNK_DIMS, chunk_y=y/CHUNK_DIMS, chunk_z=z/CHUNK_DIMS;
    x=x%CHUNK_DIMS, y=y%CHUNK_DIMS, z=z%CHUNK_DIMS;
    //Check out of bound conditions and return  NULL when needed
    if(chunk_x<0 || chunk_x>=load_distance)
        return NULL;
    if(chunk_y<0 || chunk_y>=load_distance)
        return NULL;
    if(chunk_z<0 || chunk_z>=load_distance)
        return NULL;
    
    if(x<0 || y<0 || z<0)
        return NULL;

    return (*loaded_chunks[chunk_x][chunk_y][chunk_z])(x,y,z);
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