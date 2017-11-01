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

#pragma once

#include <string>

#include "OpenGL-Wrappers.hpp"
#include "wavefront-loader.hpp"

enum Face {Front=0, Left, Right, Back, Top, Bottom};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Class declaration:
*/
//========================================================================================
//TODO: document this section



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
        
        
        Cube(vec3 p, uint type);
        Cube(vec3 p);
        Cube();
        ~Cube();
};
//########################################################################################