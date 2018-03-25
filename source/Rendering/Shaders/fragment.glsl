//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       Fragment-Shader-Depth.glsl
 *	@author 	Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief Basic fragment shader
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#version 450

in vec4 FragPos;

in vec3 normalized_pos;
in vec3 normal;//Normal to the vertex
in vec2 texture_coord;
in float l;

in float test;

out vec4 outColor;//Final color of the pixel

uniform sampler2D text;

void main()
{
    //outColor = colors[int(l)%6]*(abs(normalized_pos.x*normalized_pos.y)+0.4);//vec4(vec3(texture(text, texture_coord)), 1);
    outColor = vec4(vec3(10)/10, 1.0);
}  