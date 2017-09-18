//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.1
*
*	Header defining some output methods to print structures to the terminal
*/

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <unistd.h>

using namespace std;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Print operations
*/
//========================================================================================

/*
* Print a vec2
*/
ostream &operator<<(ostream &os, vec2 &v)
{
    return os << "(" << v.x <<", "<< v.y << ")";
}

/*
* Print a vec3
*/
ostream &operator<<(ostream &os, vec3 &v)
{
    return os << "(" << v.x <<", "<< v.y << ", "<< v.z <<")";
}

/*
* Print a vec4
*/
ostream &operator<<(ostream &os, vec4 &v)
{
    return os << "(" << v.x <<", "<< v.y << ", "<< v.z << ", " << v.w << ")";
}

/*
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