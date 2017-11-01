//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.1
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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Chunk Class implementation:
*/
//========================================================================================
//TODO: comment this section
Chunk::Chunk() : Chunk(vec3(0)){}

vector<vec3> face = {vec3(-0.5,0.5,-0.5), vec3(-0.5,0.5,0.5), vec3(0.5,0.5,0.5), vec3(0.5,0.5,-0.5)};
vector<vec3> normals = {vec3(0,1,0),vec3(0,1,0),vec3(0,1,0),vec3(0,1,0)};
vector<vec2> uvs = {vec2(0,1), vec2(0,0), vec2(1/6.f,0), vec2(1/6.f,1)};
vector<uint> indices = {0,1,2,2,3,0};
vector<int> face_types = {Top, Bottom, Left, Right, Back, Front};
vector<vec3> t_move = {vec3(0), vec3(1), vec3(0,0,1), vec3(1,0,0), vec3(0,1,0), vec3(4)};

GLuint world_cubes_VAO;
vector<GLuint> cube_rendering_VBOs(6);
vector<GLuint> cube_VBO_types = {GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, 
    GL_SHADER_STORAGE_BUFFER, GL_SHADER_STORAGE_BUFFER,  GL_ELEMENT_ARRAY_BUFFER};

	
Texture *texture = NULL;

Chunk::Chunk(vec3 offset)
{    
    for(int i=0; i<CHUNK_DIMENSIONS*CHUNK_DIMENSIONS*CHUNK_DIMENSIONS; i++)
    {
        chunk_cubes[i] = new Cube(2.f*vec3(i/(CHUNK_DIMENSIONS*CHUNK_DIMENSIONS) + offset[0], 
            (i/CHUNK_DIMENSIONS) % CHUNK_DIMENSIONS + offset[1], i%CHUNK_DIMENSIONS + offset[2]));

    }

    glGenVertexArrays(1, &world_cubes_VAO);
    glGenBuffers(6,cube_rendering_VBOs.data());

    update_render_info();

    if(texture == NULL)
    {
        texture = new Texture();
        createTexture(*texture, "Assets/Textures/Face_Orientation.png", GL_TEXTURE_2D);
            
        loadTexture(Rendering_Handler->current_program, *(texture));
    }
    
}

Chunk::~Chunk()
{
    for(int i=0; i<CHUNK_DIMENSIONS*CHUNK_DIMENSIONS*CHUNK_DIMENSIONS; i++)
    {
       delete(chunk_cubes[i]);
    }
}

bool once = true;
void Chunk::render_chunk()
{
  /*  if(once)
    {
        loadTexture(Rendering_Handler->current_program, *(texture));
        once = false;
    }*/
    Rendering_Handler->multi_render(world_cubes_VAO, &cube_rendering_VBOs, 
        &cube_VBO_types, 5, indices.size(), face_types.size());
}

#define MESH Cube::meshes[0]
void Chunk::update_render_info()
{
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

    //TODO: figure out if other buffers are faster
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, cube_rendering_VBOs[3]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, face_types.size()*sizeof(Face), face_types.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, cube_rendering_VBOs[3]);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, cube_rendering_VBOs[4]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, t_move.size()*sizeof(vec3), t_move.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, cube_rendering_VBOs[4]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_rendering_VBOs[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MESH->indices->size()*sizeof(uint),
        MESH->indices->data(), GL_DYNAMIC_DRAW);
}
//########################################################################################

//========================================================================================
/*
*	Mega_Chunk Class implementation:
*/
//========================================================================================
//TODO: comment this section

World::World()
{    
    loaded_chunks = (Chunk****)malloc(load_distance*sizeof(Chunk***));

    for(int i=0; i<load_distance; i++)
    {
        loaded_chunks[i] = (Chunk***)malloc(load_distance*sizeof(Chunk**));
        for(int j=0; j<load_distance; j++)
        {
            loaded_chunks[i][j] = (Chunk**)malloc(load_distance*sizeof(Chunk*));
        }
    }
}

void World::render_world()
{
   
}
//########################################################################################