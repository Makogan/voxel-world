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
*	Chunk Class implementation:
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

Chunk::Chunk(vec3 offset)
{
    for(int i=0; i<CUBE_SIDE*CUBE_SIDE*CUBE_SIDE; i++)
    {
        chunk_cubes[i] = new Cube(2.f*vec3(i/(CUBE_SIDE*CUBE_SIDE) + offset[0], 
            (i/CUBE_SIDE) % CUBE_SIDE + offset[1], i % CUBE_SIDE + offset[2]));

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

//========================================================================================
/*
*	Mega_Chunk Class implementation:
*/
//========================================================================================
//TODO: comment this section

Mega_Chunk::Mega_Chunk()
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

//TODO: this is just for testing delete once testing is over
void Mega_Chunk::chunk_block()
{
   /* for(int i=0; i<load_distance; i++)
        loaded_chunks[i] = new Chunk();*/
    for(int i=0; i<load_distance; i++)
    {
        for(int j=0; j<load_distance; j++)
        {
            for(int k=0; k<load_distance; k++)
            {
                loaded_chunks[i][j][k] = new Chunk(vec3(i*16,j*16,k*16));
            }
        }
    }
}

void Mega_Chunk::render_mega_chunk()
{
   /* loaded_chunks[0][0][0]->render_chunk();
    loaded_chunks[1][0][0]->render_chunk();
    loaded_chunks[3][0][0]->render_chunk();*/
    for(int i=0; i<load_distance; i++)
    {
        for(int j=0; j<load_distance; j++)
        {
            for(int k=0; k<load_distance; k++)
            {
                loaded_chunks[i][j][k]->render_chunk();
            }
        }
    }
}
//########################################################################################