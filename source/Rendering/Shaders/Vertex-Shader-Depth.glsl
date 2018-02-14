//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       Vertex-Shader-Depth.glsl
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

layout(std430, binding = 3) buffer data_buffer
{
    vec4 cubes_info[];//first 3 values are position of object 
};

uniform mat4 view = mat4(1); //Camera orientation and position
uniform mat4 proj = mat4(1); //The projection parameters (FOV, viewport dimensions)

void main()
{
    gl_Position = proj*view*(vec4(position, 1.0) + vec4(vec3(cubes_info[gl_InstanceID]),0));
}