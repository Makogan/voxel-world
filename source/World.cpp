//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       World.cpp
 *	@author 	Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief Definitions of all world related classes and methods
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "World.hpp"
#include "cout-definitions.hpp"

#define MESH Cube::meshes[0]

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Global Values
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! Global world object, should eb treated as a singleton
World* the_world;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Chunk Class implementation:
*/
//========================================================================================

/*
*   Class Constructors
*/

/**
 * Default constructor for a chunk
 */
Chunk::Chunk() : Chunk(vec3(0)){}

/**
 * Constructor for a chunk (should not be used, for testing only)
 */
Chunk::Chunk(vec3 offset) : Chunk(offset, NULL){}

/**
 * Creates a chunk in the world \a w \a at global position \a p \a
 */ 
Chunk::Chunk(vec3 offset, World* w) 
{
    position = offset;
    world = w;

    render_data = new Object_3D(MESH);

    this->create_cubes(offset);
}

/**
 * Class destructor
 */
Chunk::~Chunk()
{
    for(int i=0; i<CHUNK_DIMS*CHUNK_DIMS*CHUNK_DIMS; i++)
    {
       delete(chunk_cubes[i]);
    }
}

/**
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

/**
 * Creates cubes based on the current position of the chunk. This basically overwrites
 * all previous data with new values associated with the current chunk location.
 */ 
void Chunk::create_cubes(vec3 offset)
{
    position = offset;

    //Initialize all cubes in the chunk, with correct positions
    //offset represents the position of the chunk in the world
    for(int i=0; i<CHUNK_DIMS*CHUNK_DIMS*CHUNK_DIMS; i++)
    {
        //Sample Perlin noise function
        double height = 20.d*noise_2D((double)(i/(CHUNK_DIMS*CHUNK_DIMS)) + offset[0], 
            (double)((i/CHUNK_DIMS)%CHUNK_DIMS + offset[1]));
        
        //initialize cube object if needed
        if(chunk_cubes[i]==NULL)
            chunk_cubes[i] = 
                new Cube(vec3(i/(CHUNK_DIMS*CHUNK_DIMS) + offset[0], 
                (i/CHUNK_DIMS) % CHUNK_DIMS + offset[1], 
                i%CHUNK_DIMS + offset[2]));
        
        //Otherwise modify existing cube data
        else
            chunk_cubes[i]->update(vec3(i/(CHUNK_DIMS*CHUNK_DIMS) + offset[0], 
                (i/CHUNK_DIMS) % CHUNK_DIMS + offset[1], 
                i%CHUNK_DIMS + offset[2]));

        //Check if the chunk is above the heightmap
        if(double(i%CHUNK_DIMS) + offset[2]>height)
            chunk_cubes[i]->transparent = true;
    }
}

/**
 *   Update the current chunk
 */
void Chunk::update()
{
    //clear previous data
    cubes_info.clear();
    //generate new data
    update_visible_cubes();
    //send render data to the handler
    render_data->set_instance_data(Rendering_Handler, cubes_info);
}

/**
 *   Send rendering information to the rendering handler
 */
void inline Chunk::send_render_data(Renderer* handler)
{
    handler->add_data(render_data);
}

/**
 * Check neighbour information
 */ 
inline bool Chunk::check_neighbour(Cube *c, Cube *n) 
{
    if(n==NULL || n->transparent)
        return true;
    
    return false;
}

/**
 *   Set the cubes_info array with the data of the currently visible cubes
 */
void Chunk::update_visible_cubes()
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
                
                //get the x,y,z indices in the global grid
                int c_x=(int)(current->position.x);
                int c_y=(int)(current->position.y); 
                int c_z=(int)(current->position.z);

                //By default cube is not visible
                bool visible = false;

                //Check all 6 neighbours and update accordingly
                Cube* neighbour = (*world)(c_x-1,c_y,c_z);
                visible = visible || check_neighbour(current, neighbour);
                neighbour = (*world)(c_x+1,c_y,c_z);
                visible = visible || check_neighbour(current, neighbour);
                neighbour = (*world)(c_x,c_y+1,c_z);
                visible = visible || check_neighbour(current, neighbour);
                neighbour = (*world)(c_x,c_y-1,c_z);
                visible = visible || check_neighbour(current, neighbour);
                neighbour = (*world)(c_x,c_y,c_z+1);
                visible = visible || check_neighbour(current, neighbour);
                neighbour = (*world)(c_x,c_y,c_z-1);
                visible = visible || check_neighbour(current, neighbour);

                //Update information only if visible
                if(visible)
                {
                    Mesh m = current->getMesh(); 
                    cubes_info.push_back(vec4(current->position,0));
                }
            }
        }
    }
}
//########################################################################################

//========================================================================================
/*
*	Chunk Holder Class implementation:
*/
//========================================================================================

/**
 * Default constructor
 */ 
Chunk_Holder::Chunk_Holder(){}

/**
 * Create a chunk holder of \a x_dim \a, \a y_dim \a, \a z_dim \a dimensions in the 
 * world \a w \a
 */ 
Chunk_Holder::Chunk_Holder(int x_dim, int y_dim, int z_dim, World* w)
{
    world = w;
    //Initialize the first dimension
    chunkBox = cirArray<cirArray<cirArray<Chunk*>>>(x_dim);

    for(uint i=0; i<x_dim; i++)
    {
        //Initialize the second dimension
        chunkBox[i] = cirArray<cirArray<Chunk*>>(y_dim);
        for(uint j=0; j<y_dim; j++)
        {
            //Initialize the third dimension
            chunkBox[i][j] = cirArray<Chunk*>(z_dim);
            for(uint k=0; k<z_dim; k++)
            {
                //Create a new chunk
                chunkBox[i][j][k] = 
                    new Chunk(vec3(i*CHUNK_DIMS,j*CHUNK_DIMS,k*CHUNK_DIMS) + 
                    vec3(world->origin), w);
            }
        }
    }
}

/**
 * Class destructor
 */ 
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

/**
 * Overloaded () operator, used to fetch the chunk at local indices \a x, y, z \a
 */ 
Chunk* Chunk_Holder::operator()(int x, int y, int z)
{
    return chunkBox[x][y][z];
}

/**
 * Shift the entire loaded box in the direction specified by \a offset \a
 * This effectively moves the world into that direction
 */ 
void Chunk_Holder::shift(ivec3 offset)
{
    //Shift the x, y, z values
    chunkBox.shift(offset.x);
    for(int i=0; i<world->h_radius; i++)
    {
        chunkBox[i].shift(offset.y);
        for(int j=0; j<world->h_radius; j++)
            chunkBox[i][j].shift(offset.z);
    }

    //Re-initialize x values as needed
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

    ///Re-initialize y values as needed
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

    //Re-initialize z values as needed
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

    //Update all the chunks
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

/**
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

/**
 * World class destructor
 */
World::~World()
{
    delete(loaded_chunks);
}

/**
 * Center the frame around \a position \a
 */ 
void World::center_frame(ivec3 position)
{
    ivec3 distance = 
        position - origin - ivec3(h_radius/2, h_radius/2, v_radius/2)*CHUNK_DIMS;
    if(abs(distance.x) >= CHUNK_DIMS || abs(distance.y) >= CHUNK_DIMS 
        || abs(distance.z) >= CHUNK_DIMS)
    {
        origin+=((distance)/CHUNK_DIMS)*CHUNK_DIMS;
        loaded_chunks->shift(distance/CHUNK_DIMS);
    }
}

/**
 *   Overloaded () operator, used to get chunk pointers in the loaded chunks
 *   of the world through their global position
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

/**
 * Send all world render data to the handler
 */ 
void World::send_render_data(Renderer* handler)
{
    //Prevent other threads from using the queue
    handler->busy_queue.lock();

    //Clear the queue
	handler->clear();

    //get camera position
    vec3 p_pos = handler->cam->getPosition();
    for(int i=0; i<h_radius; i++)
    {
        for(int j=0; j<h_radius; j++)
        {
            for(int k=0; k<v_radius; k++)
            {
                //Find vector pointing from the camera position to the chunk
                vec3 c_dir = ((*loaded_chunks)(i,j,k))->position - p_pos;
                c_dir = normalize(c_dir);

                //Find the angle in between the looking direction and the relative chunk
                //direction
                float angle = acos(dot(c_dir, normalize(handler->cam->getForward())));

                //If the angle is less than the current field of view send data to handler
                //if(angle < (handler->cam->getFov()))
                    ((*loaded_chunks)(i,j,k))->send_render_data(handler);
            }
        }
    }
    //Release the queue
    handler->busy_queue.unlock();
}

//########################################################################################
