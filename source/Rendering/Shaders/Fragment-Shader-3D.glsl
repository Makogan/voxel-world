//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *	@file		Fragment-Shader-3D.glsl
 *	@author		Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief Basic fragment shader
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#version 450

struct Light
{
  vec4 position;//intensity here as well
  vec4 color;
};

in vec3 normal;//Normal to the vertex
in vec3 vertexPos;//True position of the vertex (i.e it's location in space)
in vec2 texture_coord;

out vec4 outColor;//Final color of the pixel

uniform sampler2D text;
uniform samplerCubeArray depth_maps;
uniform Light lights[];

uniform vec4 color = vec4(1);//Default color
//TODO: make this an array
//uniform vec3 lum = vec3(100,90,15); vec3(80,70,10);//A unique light position

uniform vec3 lums[2] = {vec3(80,70,10), vec3(100,90,15)};
uniform vec3 cameraPos = vec3(0);//The position of the camera in the world
uniform vec3 cameraDir = vec3(0);

void main()
{
	outColor = vec4(0);
	for(uint i=0 ; i<2; i++)
	{
		vec4 color = vec4(0);
		vec3 lum = lums[i];
		vec3 l = vec3(lum-vertexPos);
		if(length(l)>0)
			l = normalize(l);
		vec3 c = vec3(texture(text,abs(texture_coord)));
		vec3 n = normalize(normal);
		vec3 e = cameraPos-vertexPos;
		e = normalize(e);
		vec3 h = normalize(e+l);

		color = vec4(c*(vec3(0.5)+0.5*max(0,dot(n,l))) + 
			vec3(0.1)*max(0,pow(dot(h,n), 100)), 1);

		vec3 temp = vertexPos-lum; 

		float test = texture(depth_maps, vec4(temp, 1)).r;
		double d = length(temp);

		if(d>test*256 + 0.5)
			color = vec4(test/2);

		outColor += color;
	}

	outColor = outColor/2.f;
}