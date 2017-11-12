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

in vec3 normal;//Normal to the vertex
in vec3 vertexPos;//True position of the vertex (i.e it's location in space)

in vec2 texture_coord;

out vec4 outColor;//Final color of the pixel

uniform vec4 color = vec4(1);//Default color
//TODO: make this an array
uniform vec3 light = vec3(0,0,50);//A unique light position
uniform vec3 cameraPos = vec3(0);//The position of the camera in the world

uniform sampler2D text;

const int size = 256;
const int mask = size-1;

uniform int perm[ size ];
uniform float vec_field_x[ size ], vec_field_y[ size ];

double fade(double d)
{
  d = abs(d);
  if(d>1)
    return 0;
  return 1.f-d*d*d*(d*(d*6-15)+10);
}

double test(double x, double y)
{
  return sqrt(x*x+y*y);
}

double surflet(double x, double y, double grad_x, double grad_y)
{
    return fade(test(x,y)) * ( grad_x * x + grad_y * y );
}

double perlin_noise(double x, double y)
{
    int xi = (int(x));
    int yi = (int(y));
    
    double result = 0;
    for(int grid_y=yi; grid_y <= yi+1; ++grid_y)
    {
        for(int grid_x=xi; grid_x <= xi+1; ++grid_x)
        {
            int hash = perm[(perm[grid_x & mask] + grid_y) & mask];
            result += surflet(x-grid_x, y-grid_y, vec_field_x[hash], vec_field_y[hash]);
        }
    }
    
    return (result);
}

void main()
{
  vec3 l = vec3(light-vertexPos);
  l = normalize(l);
  vec3 c = vec3(texture(text,abs(texture_coord)));
  vec3 n = normalize(normal);
  vec3 e = cameraPos-vertexPos;
  e = normalize(e);
  vec3 h = normalize(e+l);

  //outColor = vec4(c*(vec3(0.1)+0.5*max(0,dot(n,l))) + vec3(0.1)*max(0,pow(dot(h,n), 100)), 1);

  //outColor = vec4(c,1); // mix(color,outColor,0.5);

  double x = floor(gl_FragCoord.x/4)/10.f;
  double y = floor(gl_FragCoord.y/4)/10.f;

  double total=0;
  double frequency=0.05;
  double amplitude=10;
  double maxValue=0;
  double persistence = 0.5;
  for(uint i=0; i<5; i++)
  {
    total += perlin_noise(x*frequency, y*frequency)*amplitude;

    maxValue+=amplitude;
    amplitude *= persistence;
    frequency *= 2;
  }

  double p=total/maxValue;
  if(abs(p)<0.01)
    p+=perlin_noise(x*1.5,y*1.5);
  if(p>=0)
    p=1;
  else
    p=-1;
  outColor = (vec4(p,p,p,1));
}
