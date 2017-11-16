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
*	Class declarations:
*/
//========================================================================================
//TODO: document this section
template <typename T> class cirArray
{
    private:
        vector<T> array;
        int start;

    public:
        cirArray();
        cirArray(uint size);

        int shift(int);

        T& operator[](int);
        void operator=(T);

        uint size();
};

class Chunk
{
    private:
        World* world;
        Cube *chunk_cubes[CHUNK_DIMS*CHUNK_DIMS*CHUNK_DIMS] = {};
        vector<vec4> faces_info;

        GLuint cubes_VAO;
        vector<GLuint> cube_rendering_VBOs;
        void update_visible_faces();
        void update_render_info();

    public:
        Cube* operator()(int, int, int);

        Chunk();
        Chunk(vec3);
        Chunk(vec3, World*);
        ~Chunk();

        void create_cubes(vec3);
        void update();
        void render_chunk();
};

class Chunk_Holder
{
    private:
        cirArray<cirArray<cirArray<Chunk*>>> chunkBox;
        World* world;

    public:
        Chunk_Holder();
        Chunk_Holder(int, int, int, World*);
        ~Chunk_Holder();

        Chunk* operator()(int, int, int);
        void shift(ivec3);
};

class World
{
    private:
        int h_radius = 10;
        int v_radius = 4;
        //Chunk ****loaded_chunks;
        Chunk_Holder *loaded_chunks;

    public:
        ivec3 origin = ivec3(0);
        World();
        ~World();

        Cube* operator()(int x, int y, int z);
        void center_frame(ivec3 offset);
        void render_world();        
};
//########################################################################################