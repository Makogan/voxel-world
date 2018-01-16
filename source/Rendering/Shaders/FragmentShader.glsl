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

struct Light
{
  vec4 position;
  vec4 color;
  double intensity;
};

struct Silhouette
{
    vec4 vertices[3];

    //float transparency;
    //float reflectiveness;
};

in float visible;
in vec3 normal;//Normal to the vertex
in vec3 vertexPos;//True position of the vertex (i.e it's location in space)

in vec2 texture_coord;

in Light lights[];
in ivec2 solids_meta_data[];
in Silhouette solids[];

uniform int s_num=0;

out vec4 outColor;//Final color of the pixel

uniform vec4 color = vec4(1);//Default color
//TODO: make this an array
uniform vec3 lum = vec3(0,1000,5000);//A unique light position
uniform vec3 cameraPos = vec3(0);//The position of the camera in the world
uniform vec3 cameraDir = vec3(0);

float triangleIntersection(vec3 ray, vec3 origin, vec3 p0, vec3 p1, vec3 p2)
{
	vec3 s = origin - p0;
	vec3 e1 = p1-p0;
	vec3 e2 = p2-p0;
	
	mat3 mt = mat3(s, e1, e2);
	mat3 mu = mat3(-ray, s, e2);
	mat3 mv = mat3(-ray, e1, s);
	mat3 md = mat3(-ray,e1,e2);

  float d = determinant(md);

  if(isinf(d) || isnan(d)){
    return 500.f;
  }
/*	float t = determinant(mt)/d;
	float u = determinant(mu)/d;
	float v = determinant(mv)/d;*/

/*	if(t > 0 && (u+v)<1 && (u+v)>0 && u<1 && u>0 && v<1 && v>0)
	{
		return t;
	}*/

	return -1;

}

uniform sampler2D text;

void main()
{
  //Ignore
  vec3 l = vec3(lum-vertexPos);
  l = normalize(l);
  vec3 c = vec3(texture(text,abs(texture_coord)));
  vec3 n = normalize(normal);
  vec3 e = cameraPos-vertexPos;
  e = normalize(e);
  vec3 h = normalize(e+l);

  //look here
  for(int i=0; i<0;  i++)
  {
    
    /*
    float t = triangleIntersection(l, vertexPos, 
      vec3(solids[0].vertices[0]),
       vec3(solids[0].vertices[1]),
        vec3(solids[0].vertices[2]));
    */
    
  }


  //Ignore
  //outColor = vec4(c*(vec3(0.5)+0.5*max(0,dot(n,l))) + vec3(0.1)*max(0,pow(dot(h,n), 100)), 1);

  //bool t = solids[0].transparency != 1;

//  if(t){
    outColor = vec4(solids[0].vertices[0]);
//    outColor = vec4(s_num,s_num,s_num,s_num);
//  }else
//   outColor = vec4(0,0,0,0);

  
}