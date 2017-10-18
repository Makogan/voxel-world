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

#define CUBE_SIDE 16
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
        Cube *chunk_cubes[CUBE_SIDE*CUBE_SIDE*CUBE_SIDE];

    public:
        Chunk();
        Chunk(vec3 offset);
        ~Chunk();
        void render_chunk();
};

class Mega_Chunk
{
    private:
        int load_distance = 4;
        Chunk ****loaded_chunks;

    public:
        Mega_Chunk();

        //TODO: these are just for testing delete all of them once testing is over
        void chunk_block();
        void render_mega_chunk();
};
//########################################################################################