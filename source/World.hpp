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
struct Light
{
  vec3 position;
  vec4 color;
  double intensity;
};

struct Silhouette
{
    vec4 vertices[3];

    //float transparency;
    //float reflectiveness;
};

class Chunk
{
    private:
        World* world;
        Cube *chunk_cubes[CHUNK_DIMS*CHUNK_DIMS*CHUNK_DIMS] = {};
        Render_Info *render_data;  
        vector<vec4> faces_info;
        
        void update_visible_faces();
        void update_render_info();

        inline bool check_neighbour(Cube *c, Cube *n, Face f);

    public:
        vec3 position;

        Cube* operator()(int, int, int);

        Chunk();
        Chunk(vec3);
        Chunk(vec3, World*);
        ~Chunk();

        void create_cubes(vec3);
        void update();
        void send_render_data(Renderer*);
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
        Chunk_Holder *loaded_chunks;

        GLuint VAO;
        vector<GLuint> VBOs;
        vector<Light> loaded_lights;
        vector<vector<vector<vector<Silhouette>>>> loaded_silhouettes;

    public:
        int h_radius = 10;
        int v_radius = 4;
        
        ivec3 origin = ivec3(0);
        World();
        ~World();

        Cube* operator()(int x, int y, int z);
        void center_frame(ivec3 offset);
        void send_render_data(Renderer*);        

        void addSilhouette(Mesh*, float, float);
        void clearSilhouettes();

        void loadShadingData();
};

extern World* the_world;
//########################################################################################