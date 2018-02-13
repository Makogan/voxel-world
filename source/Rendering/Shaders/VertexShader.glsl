//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.3
*
*	Basic vertex shader
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#version 410

layout(location = 0) in vec3 position; //(x,y,z) coordinates of a vertex
layout(location = 1) in vec3 norm; //a 3D vertex representing the normal to teh vertex 
layout(location = 2) in vec2 texture_coordinate;

out vec3 normal; 
out vec3 vertexPos; //projected vertex
out vec2 texture_coord;

uniform mat4 model = mat4(1); //Position and orientation of the current object
uniform mat4 view = mat4(1); //Camera orientation and position
uniform mat4 proj = mat4(1); //the projection parameters (FOV, viewport dimensions)

void main()
{
    gl_Position = proj*view*model*vec4(position, 1.0);
    normal = vec3(model*vec4(norm,1.0));
    vertexPos = vec3(model*vec4(position, 1.0)); //calculate the transformed pos
    texture_coord = texture_coordinate;
}
