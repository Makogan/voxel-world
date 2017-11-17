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

//Enumerator definition for face identification and rendering
enum Face {Front=0, Left, Right, Back, Top, Bottom};
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
    private:
        static string Default_Texture;

    public:
        static vector<Mesh*> meshes;
        static vector<Texture*> textures;
        
        static void cleanup();

        vec3 position;
        uint cube_type = 0;
        bool transparent = false;

        void update(vec3 offset);
        
        Cube(vec3 p, uint type);
        Cube(vec3 p);
        Cube();
        ~Cube();
};
//########################################################################################