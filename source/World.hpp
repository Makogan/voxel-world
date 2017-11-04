//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.1
*
*	Header for the definition of a generic chunk object
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#pragma once

#include "Cube.hpp"

class World;

#define CHUNK_DIMENSIONS 16
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Class declaration:
*/
//========================================================================================
//TODO: document this section

class Chunk
{
    private:
        int i_pos,j_pos,k_pos;
        World* world;
        Cube *chunk_cubes[CHUNK_DIMENSIONS*CHUNK_DIMENSIONS*CHUNK_DIMENSIONS];
        vector<vec4> faces_info;

        GLuint world_cubes_VAO;
        vector<GLuint> cube_rendering_VBOs;
        void update_visible_faces();
        void update_render_info();

    public:
        Cube* operator()(int x, int y, int z);

        Chunk();
        Chunk(vec3 offset);
        Chunk(vec3, World* w);
        Chunk(vec3, World* w, int i, int j, int k);
        ~Chunk();

        void update();
        void render_chunk();
};

class World
{
    private:
        int load_distance = 4;
        Chunk ****loaded_chunks;

    public:
        World();

        Chunk* operator()(int x, int y, int z);
        void render_world();        
};
//########################################################################################