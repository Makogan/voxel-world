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
//TODO: delete when done
double noise_2D(double x, double y);
void vec_field_init();
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
        double height = 20.d*noise_2D((double)(i/(CHUNK_DIMS*CHUNK_DIMS)) + offset[0], 
            (double)((i/CHUNK_DIMS)%CHUNK_DIMS + offset[1]));
        
        chunk_cubes[i] = 
            new Cube(vec3(i/(CHUNK_DIMS*CHUNK_DIMS) + offset[0], 
            (i/CHUNK_DIMS) % CHUNK_DIMS + offset[1], 
            i%CHUNK_DIMS + offset[2]));

        if(double(i%CHUNK_DIMS) + offset[2]>height)
            chunk_cubes[i]->transparent = true;
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
                if(current->transparent)
                    continue;
                
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
    //TODO: this should not be here
    vec_field_init();
    //Allocate memory for the loaded chunks
    loaded_chunks = (Chunk****)malloc(h_radius*sizeof(Chunk***));
    for(int i=0; i<h_radius; i++)
    {
        loaded_chunks[i] = (Chunk***)malloc(h_radius*sizeof(Chunk**));
        for(int j=0; j<h_radius; j++)
        {
            loaded_chunks[i][j] = (Chunk**)malloc(v_radius*sizeof(Chunk*));
        }
    }

    //Initialize loaded chunks
    for(int i=0; i<h_radius; i++)
    {
        for(int j=0; j<h_radius; j++)
        {
            for(int k=0; k<v_radius; k++)
            {
                loaded_chunks[i][j][k] = 
                    new Chunk(vec3(i*CHUNK_DIMS,j*CHUNK_DIMS,k*CHUNK_DIMS), this);
            }
        }
    }

    //First chunk update, to assert all values
    for(int i=0; i<h_radius; i++)
    {
        for(int j=0; j<h_radius; j++)
        {
            for(int k=0; k<v_radius; k++)
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
    for(int i=0; i<h_radius; i++)
    {
        for(int j=0; j<h_radius; j++)
        {
            for(int k=0; k<v_radius; k++)
            {
                delete(loaded_chunks[i][j][k]);
            }
        }
    }

    //Free allocated memory
    for(int i=0; i<h_radius; i++)
    {
        for(int j=0; j<v_radius; j++)
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
    if(chunk_x<0 || chunk_x>=h_radius)
        return NULL;
    if(chunk_y<0 || chunk_y>=h_radius)
        return NULL;
    if(chunk_z<0 || chunk_z>=v_radius)
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
   for(int i=0; i<h_radius; i++)
   {
       for(int j=0; j<h_radius; j++)
       {
           for(int k=0; k<v_radius; k++)
           {
               Chunk* c = loaded_chunks[i][j][k];
               c->render_chunk();
           }
       }
   }
}
//########################################################################################

//TODO: move this to it's own file
int const size = 256;
int const mask = size-1;

int perm[ size ];
float vec_field_x[ size ], vec_field_y[ size ];

void vec_field_init()
{
    for ( int index = 0; index < size; ++index ) 
    {
        int other = rand() % ( index + 1 );
        if ( index > other )
            perm[ index ] = perm[ other ];
        perm[ other ] = index;
        vec_field_x[ index ] = cosf( 2.0f * M_PI * index / size );
        vec_field_y[ index ] = sinf( 2.0f * M_PI * index / size );
    }
}

double fade(double d)
{
  d = abs(d);
  if(d>1)
    return 0;
  return 1.f-d*d*d*(d*(d*6-15)+10);
}

double test(double x, double y)
{
  return sqrt(x*x+y*y);
}

double surflet(double x, double y, double grad_x, double grad_y)
{
    return fade(test(x,y)) * ( grad_x * x + grad_y * y );
}

double perlin_noise(double x, double y)
{
    int xi = (int(x));
    int yi = (int(y));
    
    double result = 0;
    for(int grid_y=yi; grid_y <= yi+1; ++grid_y)
    {
        for(int grid_x=xi; grid_x <= xi+1; ++grid_x)
        {
            int hash = perm[(perm[grid_x & mask] + grid_y) & mask];
            result += surflet(x-grid_x, y-grid_y, vec_field_x[hash], vec_field_y[hash]);
        }
    }
    
    return (result);
}

double noise_2D(double x, double y)
{
    double total=0;
    double frequency=0.05;
    double amplitude=10.d;
    double maxValue=0.d;
    double persistence = 0.5;
    //cout << x*frequency << " " << y*frequency << endl;
    for(uint i=0; i<5; i++)
    {
        total += perlin_noise(x*frequency, y*frequency)*amplitude;

        maxValue+=amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }
    //cout << total << endl;
    double p=total/maxValue;
    /*if(abs(p)<0.01)
    p+=perlin_noise(x*1,y*1);
    if(p>=0)
    p=1;
    else
    p=-1;*/
    //cout <<  p << endl;
    return p;
}