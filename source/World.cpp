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
*	Circular array Class implementation:
*/
//========================================================================================
template <typename T> cirArray<T>::cirArray() : cirArray(0){}

template <typename T> cirArray<T>::cirArray(uint size)
{
    array = vector<T>(size);

    start=0;
}

template <typename T> T& cirArray<T>::operator[](int i)
{
    int n = array.size();
    return array[(start + i+n)%n];
}

template <typename T> void cirArray<T>::shift(int i)
{
    int n = array.size();
    start = (start+i+n)%n;

    //if(i<0) return 0; else return array.size()-1;
}

template <typename T> uint cirArray<T>::size()
{
    return array.size();
}
//########################################################################################

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

    this->create_cubes(offset);

    //Create and initialize OpenGL rendering structures
    cube_rendering_VBOs = vector<GLuint>(5);
    glGenVertexArrays(1, &cubes_VAO);
    glGenBuffers(5,cube_rendering_VBOs.data());

    glBindVertexArray(cubes_VAO);

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

    glDeleteBuffers(5, cube_rendering_VBOs.data());
    glDeleteVertexArrays(1, &cubes_VAO);
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

void Chunk::create_cubes(vec3 offset)
{
    //Initialize all cubes in the chunk, with correct positions
    //offset represents the position of the chunk in the world
    for(int i=0; i<CHUNK_DIMS*CHUNK_DIMS*CHUNK_DIMS; i++)
    {
        double height = 20.d*noise_2D((double)(i/(CHUNK_DIMS*CHUNK_DIMS)) + offset[0], 
            (double)((i/CHUNK_DIMS)%CHUNK_DIMS + offset[1]));
        
        if(chunk_cubes[i]==NULL)
            chunk_cubes[i] = 
                new Cube(vec3(i/(CHUNK_DIMS*CHUNK_DIMS) + offset[0], 
                (i/CHUNK_DIMS) % CHUNK_DIMS + offset[1], 
                i%CHUNK_DIMS + offset[2]));
        else
            chunk_cubes[i]->update(vec3(i/(CHUNK_DIMS*CHUNK_DIMS) + offset[0], 
                (i/CHUNK_DIMS) % CHUNK_DIMS + offset[1], 
                i%CHUNK_DIMS + offset[2]));

        if(double(i%CHUNK_DIMS) + offset[2]>height)
            chunk_cubes[i]->transparent = true;
    }
}

/*
*   Update the current chunk
*/
void Chunk::update()
{
    faces_info.clear();
    update_visible_faces();
    update_render_info();
}

/*
*   Send rendering informationto the rendering handler
*/
//TODO: Maybe delete this and have the handler fetch the information directly
void Chunk::render_chunk()
{
    Rendering_Handler->multi_render(cubes_VAO, &cube_rendering_VBOs, 
        &cube_VBO_types, 4, MESH->indices->size(),faces_info.size());
}

//Help macro, correctly updates the visible faces info
#define CHECK_NEIGHBOUR(c,n,f) if(n==NULL)\
faces_info.push_back(vec4(c->position,f+6));\
else if(n->transparent)\
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
    glBindVertexArray(cubes_VAO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, cube_rendering_VBOs[3]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, faces_info.size()*sizeof(vec4), 
        faces_info.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, cube_rendering_VBOs[3]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_rendering_VBOs[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MESH->indices->size()*sizeof(uint),
        MESH->indices->data(), GL_DYNAMIC_DRAW);
}
//########################################################################################

//========================================================================================
/*
*	Chunk Holder Class implementation:
*/
//========================================================================================
Chunk_Holder::Chunk_Holder(){}

Chunk_Holder::Chunk_Holder(int x_dim, int y_dim, int z_dim, World* w)
{
    world = w;
    chunkBox = cirArray<cirArray<cirArray<Chunk*>>>(x_dim);

    for(uint i=0; i<x_dim; i++)
    {
        chunkBox[i] = cirArray<cirArray<Chunk*>>(y_dim);
        for(uint j=0; j<y_dim; j++)
        {
            chunkBox[i][j] = cirArray<Chunk*>(z_dim);
            for(uint k=0; k<z_dim; k++)
            {
                chunkBox[i][j][k] = 
                    new Chunk(vec3(i*CHUNK_DIMS,j*CHUNK_DIMS,k*CHUNK_DIMS), w);
            }
        }
    }
}

Chunk_Holder::~Chunk_Holder()
{
    for(uint i=0; i<chunkBox.size(); i++)
    {
        for(uint j=0; j<chunkBox[i].size(); j++)
        {
            for(uint k=0; k<chunkBox[i][j].size(); k++)
            {
                if(chunkBox[i][j][k]!=NULL)
                    delete(chunkBox[i][j][k]);
            }
        }
    }
}

Chunk* Chunk_Holder::operator()(int x, int y, int z)
{
    return chunkBox[x][y][z];
}

void Chunk_Holder::shift(ivec3 offset)
{
    chunkBox.shift(offset.x);
    for(int i=0; i<world->h_radius; i++)
    {
        chunkBox[i].shift(offset.y);
        for(int j=0; j<world->h_radius; j++)
            chunkBox[i][j].shift(offset.z);
    }

    int start_x = offset.x<0? 0 : world->h_radius-1;
    int sign_x = offset.x == 0? 0: -offset.x/abs(offset.x);
    for(int i=0; i<=abs(offset.x); i++)
    {
        int x = (start_x + i*sign_x + world->h_radius)%world->h_radius;
        for(int j=0; j<world->h_radius; j++)
        {
            for(int k=0; k<world->v_radius; k++)
            {
                (chunkBox[x][j][k])->create_cubes(
                    vec3(x*CHUNK_DIMS,j*CHUNK_DIMS,k*CHUNK_DIMS) + vec3(world->origin));
            }
        }
    }

    int start_y = offset.y<0? 0 : world->h_radius-1;
    int sign_y = offset.y ==0? 0: -offset.y/abs(offset.y);
    for(int i=0; i<world->h_radius; i++)
    {
        for(int j=0; j<=abs(offset.y); j++)
        {
            int y = (start_y + j*sign_y + world->h_radius)%world->h_radius;
            for(int k=0; k<world->v_radius; k++)
            {
                (chunkBox[i][y][k])->create_cubes(
                    vec3(i*CHUNK_DIMS,y*CHUNK_DIMS,k*CHUNK_DIMS) + vec3(world->origin));
            }
        }
    }

    int start_z = offset.z<0? 0 : world->v_radius-1;
    int sign_z = offset.z ==0? 0: -offset.z/abs(offset.z);
    for(int i=0; i<world->h_radius; i++)
    {
        for(int j=0; j<world->h_radius; j++)
        {
            for(int k=0; k<=abs(offset.z); k++)
            {
                int z = (start_z + k*sign_z + world->v_radius)%world->v_radius;
                (chunkBox[i][j][z])->create_cubes(
                    vec3(i*CHUNK_DIMS,j*CHUNK_DIMS,z*CHUNK_DIMS) + vec3(world->origin));
            }
        }
    }

    for(int i=0; i<world->h_radius; i++)
    {
        for(int j=0; j<world->h_radius; j++)
        {
            for(int k=0; k<world->v_radius; k++)
            {
                chunkBox[i][j][k]->update();
            }
        }
    }
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
        
    loaded_chunks = new Chunk_Holder(h_radius, h_radius, v_radius, this);

    //First chunk update, to assert all values
    for(int i=0; i<h_radius; i++)
    {
        for(int j=0; j<h_radius; j++)
        {
            for(int k=0; k<v_radius; k++)
            {
               (*loaded_chunks)(i,j,k)->update();
            }
        }
    }
}

/*
* World class destructor
*/
World::~World()
{
    delete(loaded_chunks);
}


void World::center_frame(ivec3 position)
{

    /*int distance = position.x - origin.x - (h_radius/2)*CHUNK_DIMS;
    if(abs(distance) >= 1*CHUNK_DIMS)
    {
        origin.x+=((distance)/CHUNK_DIMS)*CHUNK_DIMS;
        loaded_chunks->shift(ivec3(distance/CHUNK_DIMS,0,0));
    }*/

    ivec3 distance = 
        position - origin - ivec3(h_radius/2, h_radius/2, v_radius/2)*CHUNK_DIMS;
    if(abs(distance.x) >= 2*CHUNK_DIMS || abs(distance.y) >= 2*CHUNK_DIMS 
        || abs(distance.z) >= 2*CHUNK_DIMS)
    {
        origin+=((distance)/CHUNK_DIMS)*CHUNK_DIMS;
        loaded_chunks->shift(distance/CHUNK_DIMS);
    }
}

/*
*   Overloaded () operator, used to get chunk pointers in the loaded chunks
*   of the world
*/
Cube* World::operator()(int x, int y, int z)
{
    x-=origin.x, y-=origin.y, z-=origin.z;
    int chunk_x = (x/CHUNK_DIMS), chunk_y=y/CHUNK_DIMS, chunk_z=z/CHUNK_DIMS;
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

    return (*(*loaded_chunks)(chunk_x,chunk_y,chunk_z))(x,y,z);
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
               Chunk* c = (*loaded_chunks)(i,j,k);
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
    x = abs(x);
    y = abs(y);
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