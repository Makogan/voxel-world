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
//uniform vec3 lum = vec3(100,90,15); vec3(80,70,10);//A unique light position

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
	vec4 voxelVal = texelFetch(voxel_map, ivec3(round(pos)), 0);

	return voxelVal;
}

float sign(float val)
{
	if(val > 0)
		return 1.f;
	else if(val < 0)
		return -1.f;

	else return 0.f;
}
vec3 get_next_voxel(vec3 start, vec3 direction)
{
	direction = normalize(direction);

	float xdir, ydir, zdir;
	xdir = direction.x;
	ydir = direction.y;
	zdir = direction.z;

	float next_x = int(start.x)+sign(xdir);//add voxel size coefficient ehre
	float next_y = int(start.y)+sign(ydir);
	float next_z = int(start.z)+sign(zdir);

	vec3 offset = vec3(next_x, next_y, next_z) - start;

	float x_coeff, y_coeff, z_coeff;

	x_coeff = offset.x/xdir; 
	y_coeff = offset.y/ydir; 
	z_coeff = offset.z/zdir; 

	float coeff = min(abs(x_coeff), abs(y_coeff));
	coeff = min(coeff, abs(z_coeff));

	return start + direction*coeff;
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
	vec3 c = vec3(fetchVoxel(pos));
	vec3 n = normalize(normal);
	vec3 e = cameraPos-vertexPos;
	e = normalize(e);
	vec3 h = normalize(e+l);

	color = vec4(c*(vec3(0.5)+0.5*max(0,dot(n,l))) + 
		vec3(0.1)*max(0,pow(dot(h,n), 100)), 1);

	int count =0;
	vec3 start = vertexPos;
	vec3 direction = normalize(lums[0] - vertexPos);

	do
	{
		count++;
		//start = get_next_voxel(start, direction);
		start += direction*0.02;
		vec4 voxel_val = grabVoxel(start+direction*0.1);

		if (voxel_val.w>0 && length(vertexPos-start)>0.01)
		{
			color 	/= 2.0f;
			break;
		}
	}
	while(count < 250);

	outColor = color;
}