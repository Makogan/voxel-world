//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.2
*
*	Header for the definition of a generic cube object
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
Chunk::Chunk() : Chunk(vec3(0)){}

Chunk::Chunk(vec3 offset) : Chunk(offset, NULL){}

extern GLuint DEVAO;

Chunk::Chunk(vec3 offset, World* w) 
{
    position = offset;
    world = w;

    render_data = new Render_Info();
    //render_data->info_lock.lock();

    this->create_cubes(offset);

    render_data->types={GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, 
        GL_SHADER_STORAGE_BUFFER, GL_ELEMENT_ARRAY_BUFFER};

    //Create and initialize OpenGL rendering structures
    render_data->VBOs = vector<GLuint>(render_data->types.size());
    //glGenVertexArrays(1, &(render_data->VAO));
    render_data->VAO = DEVAO;
    
    glGenBuffers(5,(render_data->VBOs.data()));
    

    glBindVertexArray(render_data->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, render_data->VBOs[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
    glBufferData(GL_ARRAY_BUFFER, MESH->vertices.size()*sizeof(vec3), 
    MESH->vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, render_data->VBOs[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vec3), (void*)0);
    glBufferData(GL_ARRAY_BUFFER, MESH->normals.size()*sizeof(vec3),
    MESH->normals.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, render_data->VBOs[2]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
    glBufferData(GL_ARRAY_BUFFER, MESH->uvs.size()*sizeof(vec2),
        MESH->uvs.data(), GL_DYNAMIC_DRAW);
}

/*
*   Update chunk OpenGL rendering information
*/
void Chunk::update_render_info()
{
    Rendering_Handler->busy_queue.lock();
    glBindVertexArray(render_data->VAO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, (render_data->VBOs[3]));
    glBufferData(GL_SHADER_STORAGE_BUFFER, faces_info.size()*sizeof(vec4), 
        faces_info.data(), GL_DYNAMIC_COPY);
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, (render_data->VBOs)[3]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (render_data->VBOs)[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MESH->indices.size()*sizeof(uint),
        MESH->indices.data(), GL_DYNAMIC_DRAW);
    Rendering_Handler->busy_queue.unlock();
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
    position = offset;

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
void Chunk::send_render_data(Renderer* handler)
{
    render_data->layouts = 9;//render_data->types.size()-1;
    render_data->render_instances=faces_info.size();
    render_data->geometry = MESH;

    handler->add_data(render_data);
}

//correctly updates the visible faces info
inline bool Chunk::check_neighbour(Cube *c, Cube *n, Face f) 
{
    if(n==NULL || n->transparent)
        return true;
    
    return false;
}

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

                bool visible = false;
                //fetch all 6 neighbours and update accordingly
                Cube* neighbour = (*world)(c_x-1,c_y,c_z);
                visible = visible || check_neighbour(current, neighbour, Left);
                neighbour = (*world)(c_x+1,c_y,c_z);
                visible = visible || check_neighbour(current, neighbour, Right);
                neighbour = (*world)(c_x,c_y+1,c_z);
                visible = visible || check_neighbour(current, neighbour, Front);
                neighbour = (*world)(c_x,c_y-1,c_z);
                visible = visible || check_neighbour(current, neighbour, Back);
                neighbour = (*world)(c_x,c_y,c_z+1);
                visible = visible || check_neighbour(current, neighbour, Top);
                neighbour = (*world)(c_x,c_y,c_z-1);
                visible = visible || check_neighbour(current, neighbour, Bottom);

                if(visible)
                {
                    Mesh m = current->getMesh(); 
                    faces_info.push_back(vec4(current->position,0));
                        world->addSilhouette(&m,current->position,0,0);
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
                    new Chunk(vec3(i*CHUNK_DIMS,j*CHUNK_DIMS,k*CHUNK_DIMS) + 
                    vec3(world->origin), w);
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

    loaded_silhouettes = vector<vector<Silhouette>>(h_radius*h_radius*v_radius);

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

    VBOs = vector<GLuint>(2);
    //glGenVertexArrays(1, &VAO);
    VAO = DEVAO;
    /*
    glBindVertexArray(VAO);
    for (int i =0; i < 2; i++){
        GLuint tmp;
        glGenBuffers(1, &tmp);
        std::cout << tmp << std::endl;
        VBOs.push_back(tmp);
    }*/
    glGenBuffers(2,VBOs.data());
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

    ivec3 distance = 
        position - origin - ivec3(h_radius/2, h_radius/2, v_radius/2)*CHUNK_DIMS;
    if(abs(distance.x) >= CHUNK_DIMS || abs(distance.y) >= CHUNK_DIMS 
        || -(distance.z) >= CHUNK_DIMS)
    {
        clearSilhouettes();
        origin+=((distance)/CHUNK_DIMS)*CHUNK_DIMS;
        loaded_chunks->shift(distance/CHUNK_DIMS);
        loadShadingData();
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

void World::send_render_data(Renderer* handler)
{
    handler->busy_queue.lock();
    handler->clear();

    vec3 p_pos = handler->cam->getPosition();
    for(int i=0; i<h_radius; i++)
    {
        for(int j=0; j<h_radius; j++)
        {
            for(int k=0; k<v_radius; k++)
            {
                vec3 c_dir = ((*loaded_chunks)(i,j,k))->position - p_pos;
                c_dir = normalize(c_dir);

                float angle = acos(dot(c_dir, normalize(handler->cam->getForward())));

                if(angle < (handler->cam->getFov()))
                    ((*loaded_chunks)(i,j,k))->send_render_data(handler);
            }
        }
    }
    handler->busy_queue.unlock();
}

void World::addSilhouette(Mesh* mesh, vec3 offset, float trans, float ref)
{
    if(mesh==NULL)
        cerr << "Attempted to add null mesh" << endl;
    //cout << mesh->indices.size() << endl;
    for(int i=0; i<mesh->indices.size(); i+=3)
    {
        int x,y,z;
        vec3 pos = (*mesh).vertices[(*mesh).indices[i]];
        x = pos.x/CHUNK_DIMS, y=pos.y/CHUNK_DIMS, z=pos.z/CHUNK_DIMS;
        x-=origin.x/CHUNK_DIMS, y-=origin.y/CHUNK_DIMS, z-=origin.z/CHUNK_DIMS;
        x = (x%h_radius+h_radius)%h_radius, y=(y%h_radius+h_radius)%h_radius, 
        z=(z%v_radius+v_radius)%v_radius;

        for(uint j=0; j<3; j++)
        {
            Silhouette s;

            vec4 point = vec4((*mesh).vertices[(*mesh).indices[i+j]],0);
            s.vertices[j] = point+vec4(offset,0);//vec4(offset,0);
            //cout << point.x << " " << point.y << " " << point.z <<  endl;
            //s.transparency = trans;
            //s.reflectiveness = ref;

            loaded_silhouettes[0].push_back(s);
        }
    }
}

void World::clearSilhouettes()
{
    for(uint i=0; i<loaded_silhouettes.size(); i++)
    {
        loaded_silhouettes[i].clear();
    }
}

void World::loadShadingData()
{
    vector<Silhouette> holder;

    holder = loaded_silhouettes[0];
   /* Silhouette s;
    holder.clear();
   // for(uint i=0; i<temp.size()/3; i++)
    {
        s.vertices[0] = vec4(5*16,5*16,0,0);
        s.vertices[1] = vec4(5*16+10,5*16,0,0);
        s.vertices[2] = vec4(5*16,5*16,10,0);
    }
    //s.transparency = 7;
    //s.reflectiveness = 7;

    holder.push_back(s);*/


    glUseProgram(Rendering_Handler->current_program);
    glBindVertexArray(VAO);  
    GLint loc = glGetUniformLocation(Rendering_Handler->current_program, "s_num");
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{
		cerr << "Error returned when trying to find s_num uniform."
			<< endl;
		return;
	}
    glUniform1i(loc,holder.size());

   /* for(uint i=0; i<holder.size(); i++)
    {
        cout << holder[i].vertices[0] << endl;
        cout << holder[i].vertices[1] << endl;
        cout << holder[i].vertices[2] << endl;
    }*/
    
    cout << holder.size() << endl << endl;
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBOs[0]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, holder.size()*sizeof(Silhouette), 
        holder.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, VBOs[0]);
    glFinish();
}
//########################################################################################
