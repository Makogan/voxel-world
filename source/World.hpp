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
        Cube *chunk_cubes[CHUNK_DIMENSIONS*CHUNK_DIMENSIONS*CHUNK_DIMENSIONS];
        vector<vec4> faces_info;

        GLuint world_cubes_VAO;
        vector<GLuint> cube_rendering_VBOs;
        void update_visible_faces();
        void update_render_info();

    public:
        Cube* operator()(uint x, uint y, uint z);

        Chunk();
        Chunk(vec3 offset);
        ~Chunk();

        void render_chunk();
};

class World
{
    private:
        int load_distance = 4;
        Chunk ****loaded_chunks;

    public:
        World();

        //temp
        void render_world();

        
};
//########################################################################################