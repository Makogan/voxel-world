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

in vec3 normalized_pos;
in vec3 normal;//Normal to the vertex
in vec2 texture_coord;
in float l;

in float test;

out vec4 outColor;//Final color of the pixel

uniform sampler2D text;

vec4 colors[] = {vec4(0,0,1,1), vec4(0,1,0,1), vec4(0,1,1,1), 
    vec4(1,0,0,1), vec4(1,0,1,1), vec4(1,1,0,1)};

void main()
{
    //outColor = colors[int(l)%6]*(abs(normalized_pos.x*normalized_pos.y)+0.4);//vec4(vec3(texture(text, texture_coord)), 1);
    outColor = vec4(vec3(test)/10.f, 1.0);

}