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
uniform sampler3D voxel_map;

uniform vec4 color = vec4(1);//Default color
//TODO: make this an array

uniform vec3 lums[2] = {vec3(0,0,10), vec3(80,90,50)};
uniform vec3 cameraPos = vec3(0);//The position of the camera in the world
uniform vec3 cameraDir = vec3(0);

vec4 fetchVoxel(vec3 pos)
{
	pos += vec3(1,1,0);
	vec4 voxelVal = texelFetch(voxel_map, ivec3(pos-vec3(0.4999, 0.4999, 0)), 0);
	if(voxelVal.w==0)
		voxelVal = texelFetch(voxel_map, ivec3(pos-vec3(0.5001, 0.5001, 0)), 0);

	return voxelVal;
}

vec4 grabVoxel(vec3 pos)
{
	//pos += vec3(0.5,0.5,0);
	vec4 voxelVal = texelFetch(voxel_map, ivec3(round(pos)), 0);

	return voxelVal;
}

float bound(float val)
{
	if(val > 0)
		return 1.f;
	return 0.f;
}
float coeff;
vec3 get_next_voxel(vec3 start, vec3 direction)
{
	direction = normalize(direction);

	float xdir, ydir, zdir;
	xdir = direction.x;
	ydir = direction.y;
	zdir = direction.z;

	float m_x, m_y, m_z; //mapped components at [0,voxel_size] of position
	#define voxel_size 1.0f
	m_x = mod(start.x, voxel_size);
	m_y = mod(start.y, voxel_size);
	m_z = mod(start.z, voxel_size);

	float bound_x, bound_y, bound_z;
	bound_x = bound(xdir);
	bound_y = bound(ydir);
	bound_z = bound(zdir);

	float x_coeff, y_coeff, z_coeff;

	x_coeff = (bound_x-m_x)/xdir;
	y_coeff = (bound_y-m_y)/ydir;
	z_coeff = (bound_z-m_z)/zdir;

	coeff = min(abs(x_coeff), abs(y_coeff));
	coeff = min(coeff, abs(z_coeff));

	return start + direction*coeff;
}

float sign(float x)
{
	if(x > 0)
		return 1;
	else if(x <0 )
		return -1;

	return 0;
}

float planeIntersection(vec3 ray, vec3 origin, vec3 n, vec3 q)
{
	n = normalize(n);
	if(dot(ray,n)!=0)
		return (dot(q,n)-dot(n,origin))/dot(ray,n);

	return -1;
}

float t_modem;
vec3 get_voxel(vec3 start, vec3 direction)
{
	direction = normalize(direction);

	ivec3 discretized_pos = ivec3(start);

	vec3 n_x = vec3(sign(direction.x), 0,0);
	vec3 n_y = vec3(0, sign(direction.y),0);	
	vec3 n_z = vec3(0, 0,sign(direction.z));

	float bound_x, bound_y, bound_z;

	bound_x = bound(direction.x);
	bound_y = bound(direction.y);
	bound_z = bound(direction.z);

	float t_x, t_y, t_z;

	t_x = planeIntersection(direction, start, n_x, 
		discretized_pos+ivec3(bound_x,0,0));
	
	t_y = planeIntersection(direction, start, n_y, 
		discretized_pos+ivec3(0,bound_y,0));

	t_z = planeIntersection(direction, start, n_z, 
		discretized_pos+ivec3(0,0,bound_z));

	if(t_x < 0)
		t_x = 1.f/0.f;
	if(t_y < 0)
		t_y = 1.f/0.f;
	if(t_z < 0)
		t_z = 1.f/0.f;

	t_modem = min(t_x, t_y);
		t_modem = min(t_modem, t_z);

	return start + direction*t_modem;
}

void main()
{
	vec3 pos = vertexPos;
	//pos += vec3(1,1,0);

	outColor = vec4(0);

	vec4 color = vec4(0);
	vec3 lum = lums[0];
	vec3 l = vec3(lum-vertexPos);
	if(length(l)>0)
		l = normalize(l);
	vec3 c = vec3(1,1,1);//vec3(fetchVoxel(pos));
	vec3 n = normalize(normal);
	vec3 e = cameraPos-vertexPos;
	e = normalize(e);
	vec3 h = normalize(e+l);

	color = vec4(c*(vec3(0.5)+0.5*max(0,dot(n,l))) + 
		vec3(0.1)*max(0,pow(dot(h,n), 100)), 1);

	int count =0;
	vec3 start = vertexPos + vec3(0.4999,0.4999,0.4999);
	vec3 direction = normalize(lums[0] - vertexPos);

	//color = vec4(1,0,0,0);

	do
	{
		count++;
		//start = get_voxel(start, direction);
		start = get_next_voxel(start, direction);
		//start += direction*0.02;
		vec4 voxel_val = grabVoxel(start);

		//if (voxel_val.w>0 && length(vertexPos-start)>0.0001)
		{
			color /= 2.0f;
			color = vec4(abs(coeff));
			break;
		}
		
	}
	while(count < 250);

	outColor = color;
}