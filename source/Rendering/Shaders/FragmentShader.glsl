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

#version 450

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
//in Silhouette solids[];

in vec4 fun;

out vec4 outColor;//Final color of the pixel

uniform int s_num=0;

uniform sampler2D text;


uniform vec4 color = vec4(1);//Default color
//TODO: make this an array
uniform vec3 lum = vec3(80,70,0.1);//A unique light position
uniform vec3 cameraPos = vec3(0);//The position of the camera in the world
uniform vec3 cameraDir = vec3(0);

layout(std430, binding = 5) buffer object_buffer
{
    Silhouette solids[];
};

float sphereIntersection(vec3 ray, vec3 origin, vec3 center, float radius)
{		 

	float a = dot(ray,ray);
	float b = -2*dot(center, ray)+2*dot(ray,origin);
	float c = -2*dot(origin,center)+dot(center,center)
			  -radius*radius+dot(origin,origin);

	float discriminant =b*b - 4*a*c;
		
	float t, t1, t2;
	if(discriminant < 0)
	{
		return -1;
	}
	
	else
	{
		t1=(-b-sqrt(discriminant))/(2*a);
		t2=(-b+sqrt(discriminant))/(2*a);
	}
	
	if(t1<0 && t2>=0)
		t1=t2;
	else if (t1>=0 && t2<0)
		t2=t1;

	return min(t1,t2);
}

float triangleIntersection(vec3 ray, vec3 origin, vec3 p0, vec3 p1, vec3 p2)
{
  /*vec3 s = origin - p0;
	vec3 e1 = p1-p0;
	vec3 e2 = p2-p0;
	
	mat3 mt = mat3(s, e1, e2);
	mat3 mu = mat3(-ray, s, e2);
	mat3 mv = mat3(-ray, e1, s);
	mat3 md = mat3(-ray,e1,e2);

	float t = determinant(mt)/determinant(md);
	float u = determinant(mu)/determinant(md);
	float v = determinant(mv)/determinant(md);

	if(t > 0 && (u+v)<1 && (u+v)>0 && u<1 && u>0 && v<1 && v>0)
	{
		return t;
	}

  return -1;*/

	vec3 s = origin - p0;
	vec3 e1 = p1-p0;
	vec3 e2 = p2-p0;
	
	mat3 mt = mat3(s, e1, e2);
	mat3 mu = mat3(-ray, s, e2);
	mat3 mv = mat3(-ray, e1, s);
	mat3 md = mat3(-ray,e1,e2);

	float d = determinant(md);

	if(isinf(d) || isnan(d))
	{
		return -1;
	}

	float t = determinant(mt)/d;
	float u = determinant(mu)/d;
	float v = determinant(mv)/d;

	if(t > 0 && (u+v)<1 && (u+v)>0 && u<1 && u>0 && v<1 && v>0)
	{
		return t;
	}

	return -1;

}

void main()
{
  vec3 l = vec3(lum-vertexPos);
  if(length(l)>0)
  	l = normalize(l);
  vec3 c = vec3(texture(text,abs(texture_coord)));
  vec3 n = normalize(normal);
  vec3 e = cameraPos-vertexPos;
  e = normalize(e);
  vec3 h = normalize(e+l);

  outColor = vec4(c*(vec3(0.5)+0.5*max(0,dot(n,l))) + vec3(0.1)*max(0,pow(dot(h,n), 100)), 1);

 // for(int i=195000; i<s_num-195000;  i++)
  //{
    /*float t = triangleIntersection(lum, vertexPos, 
      cameraPos+vec3(10,20,0),
      cameraPos+vec3(0,-30,0),
      cameraPos+vec3(20,-20,0));*/
      /*float t = triangleIntersection(l, vertexPos, 
        vec3(solids[0].vertices[0]), 
        vec3(solids[0].vertices[1]),
        vec3(solids[0].vertices[2]));*/
  	float t = sphereIntersection(vec3(80,70,0.3)-vertexPos, vertexPos, vec3(80,80,0), 1);

    //if(vertexPos.z > 2)
    if(t>0.01)
	{
      outColor = vec4(t/100,0,0,0);
	}
  //}
  //outColor = solids[0].vertices[0];
  //outColor = vec4(s_num);


  //bool t = solids[0].transparency != 1;

//  if(t){
    //outColor = abs(vec4(solids[6].vertices[2])+vec4(vertexPos,0))/1000;
    //outColor = fun;
//    outColor = vec4(s_num,s_num,s_num,s_num);
//  }else
   //outColor = vec4(1,0,0,0);

  
}