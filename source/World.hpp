//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.2
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

#define CHUNK_DIMS 16
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
        World* world;
        Cube *chunk_cubes[CHUNK_DIMS*CHUNK_DIMS*CHUNK_DIMS];
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
        ~Chunk();

        void update();
        void render_chunk();
};

class World
{
    private:
        int h_radius = 10;
        int v_radius = 20;
        ivec3 origin = ivec3(0);
        Chunk ****loaded_chunks;

    public:
        World();
        ~World();

        Cube* operator()(int x, int y, int z);
        void render_world();        
};
//########################################################################################