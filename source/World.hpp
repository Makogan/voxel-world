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
        void update_render_info();

    public:
        Chunk();
        Chunk(vec3 offset);
        ~Chunk();
        void render_chunk();
};

class World
{
    private:
        int load_distance = 1;
        Chunk ****loaded_chunks;

    public:
        World();

        //temp
        void render_world();

        
};
//########################################################################################