//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       Cube.hpp
 *	@author 	Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief Header for the definition of a generic cube object
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

/**
 *   Cube class definition
 */
class Cube
{
    public:
        static vector<Mesh*> meshes;        //!< meshes of all cube subclasses
        static vector<Texture*> textures;   //!< textures of all cube subclasses
        
        static void initialize();
        static void cleanup();

        vec3 position;                      //!< global position of the cube
        CubeID cube_type = DEFAULT;         //!< unique ID representing the cube
        bool transparent = false;           //!< is the block transparent?

        void update(vec3 offset);
        
        Cube(vec3 p, CubeID type);
        Cube(vec3 p);
        Cube();
        ~Cube();

        Mesh getMesh();
};
//########################################################################################