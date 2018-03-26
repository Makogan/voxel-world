//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *	@file		3D-shader-fragment.glsl
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

uniform float width = 128;
uniform float depth = 128;
uniform float height = 128;

uniform vec4 color = vec4(1);//Default color

uniform float mip_maps = 0;

uniform float voxel_size = 1;
float current_mip_map = 0;
//TODO: make this an array

vec3 lums[2] = {vec3(0,0,10), vec3(80,90,50)};
uniform vec3 cameraPos = vec3(0);//The position of the camera in the world
uniform vec3 cameraDir = vec3(0);

float sign(float x)
{
	if(x>0)
		return 1;                             
	if(x<0)
		return -1;
	return 0;
}

vec4 grabVoxel(vec3 pos, vec3 direction)
{

	vec3 temp = pos;

	pos *= 1.f/voxel_size;

	pos.x /= (width-1);
	pos.y /= (depth-1);
	pos.z /= (height-1);
	vec4 voxelVal = texture(voxel_map, pos);

	return voxelVal;
}

float bound(float val)
{
	if(val > 0)
		return voxel_size;
	return 0;
}

float planeIntersection(vec3 ray, vec3 origin, vec3 n, vec3 q)
{
	n = normalize(n);
	if(dot(ray,n)!=0)
		return (dot(q,n)-dot(n,origin))/dot(ray,n);

	return -1;
}

float t;
vec3 get_voxel(vec3 start, vec3 direction)
{
	direction = normalize(direction);

	vec3 discretized_pos;
	discretized_pos = floor(start/voxel_size)*voxel_size;

	vec3 n_x = vec3(sign(direction.x), 0,0);
	vec3 n_y = vec3(0, sign(direction.y),0);	
	vec3 n_z = vec3(0, 0,sign(direction.z));

	float bound_x, bound_y, bound_z;

	bound_x = bound(direction.x);//voxel_size;
	bound_y = bound(direction.y);//voxel_size;
	bound_z = bound(direction.z);//voxel_size;

	float t_xp, t_yp, t_zp;

	t_xp = planeIntersection(direction, start, n_x, 
		discretized_pos+vec3(bound_x,0,0));
	
	t_yp = planeIntersection(direction, start, n_y, 
		discretized_pos+vec3(0,bound_y,0));

	t_zp = planeIntersection(direction, start, n_z, 
		discretized_pos+vec3(0,0,bound_z));

	if(t_xp <= 0)
		t_xp = 1.f/0.f;
	if(t_yp <= 0)
		t_yp = 1.f/0.f;
	if(t_zp <= 0)
		t_zp = 1.f/0.f;

	t = min(t_xp, t_yp);
	t = min(t, t_zp);

	return start + direction*t + direction*0.01;;
}

bool out_of_bounds(vec3 pos)
{
	if(pos.x < 0 || pos.x > width || pos.y <0 || pos.y > depth 
		|| pos.z < 0 || pos.z > height)
		return true;

	return false;
}

void main()
{
	vec3 pos = vertexPos;

	outColor = vec4(0);

	vec4 color = vec4(0);
	vec3 lum = lums[0];
	vec3 l = vec3(lum-vertexPos);
	if(length(l)>0)
		l = normalize(l);
	vec3 c = vec3(texture(text, texture_coord));//vec3(1,1,1);//vec3(fetchVoxel(pos));
	vec3 n = normalize(normal);
	vec3 e = cameraPos-vertexPos;
	e = normalize(e);
	vec3 h = normalize(e+l);

	color = vec4(c*(vec3(0.5)+0.5*max(0,dot(n,l))) + 
		vec3(0.1)*max(0,pow(dot(h,n), 100)), 1);

	int count =0;
	vec3 start = vertexPos;
	vec3 direction = normalize(lums[0] - vertexPos);

	//voxel_size = base_voxel_size;
	start += direction*voxel_size*0.001;

	current_mip_map = mip_maps;
	vec4 collision= vec4(0);
	do
	{
		count++;
		vec4 voxel_val = grabVoxel(start, direction);
		start = get_voxel(start, direction);
		if (voxel_val.w>0 )
		{
			if(voxel_val.w>0)
			{
				color *= length(start-vertexPos)/10;
				break;
			}
		}
	}
	while(!out_of_bounds(start) && count < 30);
	

	outColor = color;
}
