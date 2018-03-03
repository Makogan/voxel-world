//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *	@file		Voxel-renderer-fragment.glsl
 *	@author		Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief Basic fragment shader
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#version 450

in vec3 normal;//Normal to the vertex
in vec2 texture_coord;

out vec4 outColor;//Final color of the pixel

uniform sampler2D text;

void main()
{
    outColor = vec4(1);
}