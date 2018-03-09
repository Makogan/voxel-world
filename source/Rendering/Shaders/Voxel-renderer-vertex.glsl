//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       3D-shader-vertex.glsl
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
layout(location = 1) in vec3 norm; //a 3D vertex representing the normal to the vertex 
layout(location = 2) in vec2 texture_coordinate; // texture coordinates

layout(std430, binding = 3) buffer instance_buffer
{
    vec4 cubes_info[];//first 3 values are position of object 
};

out vec3 normalized_pos;
out vec3 normal; 
out vec2 texture_coord;
out float l;
out float test;

uniform float width = 128;
uniform float depth = 128;

uniform float voxel_size = 1;
uniform vec3 origin = vec3(0,0,0);

uniform float level=0;

void main()
{
    texture_coord = texture_coordinate; 
    vec4 pos = (vec4(position, 1.0) + vec4(vec3(cubes_info[gl_InstanceID]),0));

    pos.x = (2.f*pos.x-width+1)/(width-1);
    pos.y = (2.f*pos.y-depth+1)/(depth-1);

    test = pos.z + 1;

    gl_Position = pos;

    normalized_pos = vec3(pos);

    normal = normalize(norm);
    texture_coord = texture_coordinate;
    l = level;
}