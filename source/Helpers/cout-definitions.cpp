//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
*   @file       cout-definitions.cpp
*	@author 	Camilo Talero
*
*
*	Version: 0.0.2
*
*	@brief Implementation of the output functions for I/O debugging
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "cout-definitions.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Print operations
*/
//========================================================================================

/**
* Print a vec2
*/
ostream &operator<<(ostream &os, vec2 &v)
{
    return os << "(" << v.x <<", "<< v.y << ")";
}

/**
* Print a vec3
*/
ostream &operator<<(ostream &os, vec3 &v)
{
    return os << "(" << v.x <<", "<< v.y << ", "<< v.z <<")";
}

/**
* Print a vec4
*/
ostream &operator<<(ostream &os, vec4 &v)
{
    return os << "(" << v.x <<", "<< v.y << ", "<< v.z << ", " << v.w << ")";
}

/**
* Print a vector of floats
*/
ostream &operator<<(ostream &os, vector<float> &v)
{
    int counter=0;
	os << "<";
    for(float t: v)
    {
        if(counter<=v.size())
            os << t << ", ";
        else
            os << t << ">";
        counter++;
    }
    return os;
}
//########################################################################################