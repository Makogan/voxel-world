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
        static Geometry *mesh;
        static Texture *texture;

        static string Default_Mesh;
        static string Default_Texture;

    public:
        static void make_template(string wavefront_file, string t);
        static void cleanup();

        vec3 position;
    
        Cube(vec3 p);
        Cube();
        ~Cube();

        void render_cube();
};
//########################################################################################