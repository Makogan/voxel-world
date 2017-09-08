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

ostream &operator<<(ostream &os, vec2 &v)
{
    return os << "(" << v.x <<", "<< v.y << ")";
}

ostream &operator<<(ostream &os, vec3 &v)
{
    return os << "(" << v.x <<", "<< v.y << ", "<< v.z <<")";
}

ostream &operator<<(ostream &os, vec4 &v)
{
    return os << "(" << v.x <<", "<< v.y << ", "<< v.z << ", " << v.w << ")";
}

ostream &operator<<(ostream &os, vector<float> &v)
{
	os << "<";
	for(float t: v)
    	os << t << ", ";
    os << ">";
    return os;
}
