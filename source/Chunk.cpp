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
    int side = 16;
    for(int i=0; i<side*side*side; i++)
    {
        chunk_cubes[i] = new Cube("Assets/Objs/cube.obj", "Assets/Textures/Cube-map.png",
            2.f*vec3(i/(side*side), (i/side) % side, i % side));
       // vec3 temp = chunk_cubes[i]->position;
     /*   cout << temp << endl;
        cout << "(" << i/(side*side) << ", " << (i/side) %side << ", " << i%side << ")" << endl;
        cout << "wtf" << endl;*/
    }
}

Chunk::~Chunk()
{
    for(int i=0; i<16*16*16; i++)
    {
       delete(chunk_cubes[i]);
    }
}

void Chunk::render_chunk()
{
    for(int i=0; i<16*16*16; i++)
    {
        chunk_cubes[i]->render_cube();
    }
}
//########################################################################################