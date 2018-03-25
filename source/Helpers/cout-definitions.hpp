//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
*   @file       cout-definitions.hpp 
*	@author 	Camilo Talero
*
*
*	Version: 0.0.3
*
*	@brief Header defining some output methods to print structures to the terminal
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#pragma once

#include "system-libraries.hpp"

using namespace std;
using namespace glm;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Print operations
*/
//========================================================================================

/*
* Print a vec2
*/
ostream &operator<<(ostream &os, vec2 &v);
/*
* Print a vec3
*/
ostream &operator<<(ostream &os, vec3 &v);
ostream &operator<<(ostream &os, ivec3 &v);
/*
* Print a vec4
*/
ostream &operator<<(ostream &os, vec4 &v);
/*
* Print a vector of floats
*/
ostream &operator<<(ostream &os, vector<float> &v);
//########################################################################################