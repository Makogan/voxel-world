//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       World.hpp
 *	@author	    Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief Header for the definition of a generic chunk object
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#pragma once

#include "Cube.hpp"
#include "tools.hpp"

class World;

#define CHUNK_DIMS 16
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Class declarations:
*/
//========================================================================================
//TODO: document this section

class Chunk
{
    private:
        World* world;                                               //< World conaitning the chunk
        Cube *chunk_cubes[CHUNK_DIMS*CHUNK_DIMS*CHUNK_DIMS] = {};   //< Array of all cubes in the chunk
        Object_3D *render_data;                                     //< Rendereable 3D object of chunk outline
        vector<vec4> cubes_info;                                    //< Instance information of the chunk 
        
        void update_visible_cubes();

        inline bool check_neighbour(Cube *c, Cube *n);

    public:
        vec3 position;                      //< Global Position of the chunk

        Cube* operator()(int, int, int);

        Chunk();
        Chunk(vec3);
        Chunk(vec3, World*);
        ~Chunk();

        void create_cubes(vec3);
        void update();
        void inline send_render_data(Renderer*);
};

class Chunk_Holder
{
    private:
        cirArray<cirArray<cirArray<Chunk*>>> chunkBox;  //< Triple circular array of chunks
        World* world;                                   //< World where this holder exists

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
        Chunk_Holder *loaded_chunks;    //< holder for all chunks, effectively world box

    public:
        int h_radius = 7;   //< horizontal size of the world in the x and y directions
        int v_radius = 4;   //< veritcal size of the world in the z direction
        
        ivec3 origin = ivec3(0);    //< upper left most corner of the world
        World();
        ~World();

        Cube* operator()(int x, int y, int z);
        void center_frame(ivec3 offset);
        void send_render_data(Renderer*);        
};

extern World* the_world; //< global pointer to the world object
//########################################################################################