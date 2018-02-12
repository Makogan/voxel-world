//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.2
*
*	Header for the definition of a generic cube object
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#pragma once

#include <string>

#include "OpenGL-Wrappers.hpp"
#include "wavefront-loader.hpp"

//Enumerator definition for cube IDs

enum CubeID {DEFAULT=0};
const uint cube_types = 1;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Class declaration:
*/
//========================================================================================

/*
*   Cube class definition
*/
class Cube
{
    public:
        static vector<Mesh*> meshes;
        static vector<Texture*> textures;
        
        static void initialize();
        static void cleanup();

        vec3 position;
        CubeID cube_type = DEFAULT;
        bool transparent = false;

        void update(vec3 offset);
        
        Cube(vec3 p, CubeID type);
        Cube(vec3 p);
        Cube();
        ~Cube();

        Mesh getMesh();
};
//########################################################################################