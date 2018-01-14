//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.2
*
*	Basic fragment shader
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#version 410

in float visible;
in vec3 normal;//Normal to the vertex
in vec3 vertexPos;//True position of the vertex (i.e it's location in space)

in vec2 texture_coord;

struct light
{
  vec3 position;
  vec4 color;
  double intensity;
};

in light lights[];
in vec3 solids[];

out vec4 outColor;//Final color of the pixel

uniform vec4 color = vec4(1);//Default color
//TODO: make this an array
uniform vec3 lum = vec3(0,1000,5000);//A unique light position
uniform vec3 cameraPos = vec3(0);//The position of the camera in the world

uniform sampler2D text;

void main()
{
  vec3 l = vec3(lum-vertexPos);
  l = normalize(l);
  vec3 c = vec3(texture(text,abs(texture_coord)));
  vec3 n = normalize(normal);
  vec3 e = cameraPos-vertexPos;
  e = normalize(e);
  vec3 h = normalize(e+l);

  outColor = vec4(c*(vec3(0.1)+0.5*max(0,dot(n,l))) + vec3(0.1)*max(0,pow(dot(h,n), 100)), 1);
  float temp = visible;
  if(temp == 0.f)
  outColor = vec4(0);
}