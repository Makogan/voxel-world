//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       Vertex-Shader-3D.glsl
 *	@author 	Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief Basic vertex shader
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#version 450

#define PI 3.1415926535897932384626433832795

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

out float h;

uniform float width = 128;
uniform float depth = 128;
uniform float height = 128;

uniform float voxel_size = 1;
uniform vec3 origin = vec3(0,0,0);

uniform float level=0;

void main()
{
    texture_coord = texture_coordinate; 
    vec4 pos = (vec4(position, 1.0) + vec4(vec3(cubes_info[gl_InstanceID]),0));

    //pos-=vec4(origin, 0);

    h = (pos.z)/(height);

    pos.x = (2.f*pos.x-width+2)/(width-2);
    pos.y = (2.f*pos.y-depth+2)/(depth-2);

    pos.z -= level;
    pos.z *= 1.f/voxel_size;

    gl_Position = pos;

    normalized_pos = vec3(pos);

    normal = normalize(norm);
}