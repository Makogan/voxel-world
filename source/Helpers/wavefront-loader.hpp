//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       wavefront-loader.hpp
 *	@author:	Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief Header declaration of methods needed to load wavefront (.obj) meshes.
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#pragma once

#include "system-libraries.hpp"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Wavefront loader:
*/
//========================================================================================

//Function to load mesh information from a .obj file into 3 separate arrays
//Arrays must be cleared before use, function assumes triangular meshes
void load_obj(std::string filename, std::vector<float> *vertices, 
    std::vector<float> *normals, std::vector<float> *texture_coords);
//########################################################################################