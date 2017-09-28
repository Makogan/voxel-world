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

#include "Chunk.hpp"
#include "cout-definitions.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Class implementation:
*/
//========================================================================================
//TODO: comment this section
Chunk::Chunk()
{
    for(int i=0; i<CUBE_SIDE*CUBE_SIDE*CUBE_SIDE; i++)
    {
        chunk_cubes[i] = new Cube(2.f*vec3(i/(CUBE_SIDE*CUBE_SIDE), 
            (i/CUBE_SIDE) % CUBE_SIDE, i % CUBE_SIDE));
    }
}

Chunk::~Chunk()
{
    for(int i=0; i<CUBE_SIDE*CUBE_SIDE*CUBE_SIDE; i++)
    {
       delete(chunk_cubes[i]);
    }
}

void Chunk::render_chunk()
{
    for(int i=0; i<CUBE_SIDE*CUBE_SIDE*CUBE_SIDE; i++)
    {
        chunk_cubes[i]->render_cube();
    }
}
//########################################################################################