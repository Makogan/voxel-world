//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.1
*
*	Basic vertex shader
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#version 410

layout(location = 0) in vec3 position; //(x,y,z) coordinates of a vertex
layout(location = 1) in vec3 normal; //a 3D vertex representing the normal to teh vertex 

out vec3 Normal; 
out vec3 vertexPos; //projected vertex

uniform mat4 model = mat4(1); //Position and orientation of the current object
uniform mat4 view = mat4(1); //Camera orientation and position
uniform mat4 proj = mat4(1); //the projection parameters (FOV, viewport dimensions)

void main()
{
    gl_Position = proj*view*model*vec4(position, 1.0);
    Normal = vec3(model*vec4(normal,1.0));
    vertexPos = vec3(model*vec4(position, 1.0)); //calculate the transformed pos
}
