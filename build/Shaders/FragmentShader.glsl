#version 410

in vec4 Normal;
in vec3 vertexPos;

out vec4 outColor;

uniform vec4 color = vec4(1);
uniform vec3 light = vec3(0,0,50);
uniform vec3 cameraPos = vec3(0);

uniform int drawMode = 1;

void main()
{
  if(drawMode==0)
    outColor = color;
  else
  {
    vec3 l = vec3(light-vertexPos);
  	l = normalize(l);
  	vec3 c = vec3(color);
  	vec3 n = vec3(Normal);
    n = normalize(n);
  	vec3 e = cameraPos-vertexPos;
  	e = normalize(e);
  	vec3 h = normalize(e+l);

  	outColor = vec4(c*(vec3(0.1)+0.5*max(0,dot(n,l))) + vec3(0.1)*max(0,pow(dot(h,n), 100)), 1);
  }
}
