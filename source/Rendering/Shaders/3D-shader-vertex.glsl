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

#define PI 3.1415926535897932384626433832795

layout(location = 0) in vec3 position; //(x,y,z) coordinates of a vertex
layout(location = 1) in vec3 norm; //a 3D vertex representing the normal to the vertex 
layout(location = 2) in vec2 texture_coordinate; // texture coordinates

layout(std430, binding = 3) buffer instance_buffer
{
    vec4 cubes_info[];//first 3 values are position of object 
};

out vec3 v_normal; 
out vec3 v_vertexPos; //projected vertex
out vec2 v_texture_coord;

uniform mat4 view = mat4(1); //Camera orientation and position
uniform mat4 proj = mat4(1); //The projection parameters (FOV, viewport dimensions)

void main()
{
    v_texture_coord = texture_coordinate;
    gl_Position = proj*view*(vec4(position, 1.0) + vec4(vec3(cubes_info[gl_InstanceID]),0));
    v_normal = normalize(norm);
    v_vertexPos = vec3(vec4(position+vec3(cubes_info[gl_InstanceID]), 1.0)); //calculate the transformed pos
}