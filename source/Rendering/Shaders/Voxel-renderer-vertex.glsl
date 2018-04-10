//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       Voxel-renderer-vertex.glsl
 *	@author 	Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief Basic vertex shader
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#version 450

layout(location = 0) in vec3 position; //(x,y,z) coordinates of a vertex
layout(location = 1) in vec3 norm; //a 3D vector representing the normal to the vertex 
layout(location = 2) in vec2 texture_coordinate; // texture coordinates

layout(std430, binding = 3) buffer instance_buffer
{
    vec4 cubes_info[];//first 3 values are position of object 
};

uniform vec3 origin = vec3(0,0,0);

out vec3 normal; 
out vec2 texture_coord;

void main()
{
    texture_coord = texture_coordinate; 
    vec4 pos = (vec4(position, 1.0) + vec4(vec3(cubes_info[gl_InstanceID]),0));

    gl_Position = pos - vec4(origin,0);

    normal = normalize(norm);
    texture_coord = texture_coordinate;
}
