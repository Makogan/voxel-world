#version 450

out vec4 outColor;

float voxel_size = 1;

float sign(float x)
{
	if(x>0)
		return 1;
	if(x<0)
		return -1;
	return 0;
}

float bound(float val)
{
	if(val > 0)
		return voxel_size;
	return 0;
}

float planeIntersection(vec3 origin, vec3 ray, vec3 n, vec3 q)
{
	n = normalize(n);
	if(dot(ray,n)!=0)
		return (dot(q,n)-dot(n,origin))/dot(ray,n);

	return -1;
}

vec3 voxel_traversal(vec3 pos, vec3 direction)
{
	vec3 discretized_pos = floor(pos/voxel_size)*voxel_size;

	vec3 n_x, n_y, n_z;

	float step_x, step_y, step_z;
	step_x = sign(direction.x);
	step_y = sign(direction.y);
	step_z = sign(direction.z);

	n_x = vec3(step_x,0,0);
	n_y = vec3(0,step_y,0);
	n_z = vec3(0, 0,step_z);

	step_x *= voxel_size;
	step_y *= voxel_size;
	step_z *= voxel_size;

	float t_x, t_y, t_z;
	t_x = (discretized_pos.x + bound(direction.x) - pos.x)/direction.x;
	t_y = (discretized_pos.y + bound(direction.y) - pos.y)/direction.y;
	t_z = (discretized_pos.z + bound(direction.z) - pos.z)/direction.z;

	if(t_x <= 0)
		t_x = 1.f/0.f;
	if(t_y <= 0)
		t_y = 1.f/0.f;
	if(t_z <= 0)
		t_z = 1.f/0.f;

	float t = min(t_x, t_y);
	t = min(t, t_z);

	return pos + t*direction;
}

vec3 get_voxel(vec3 start, vec3 direction)
{
    direction = normalize(direction);
	vec3 discretized_pos = floor(start/voxel_size)*voxel_size;

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


	float t = min(t_xp, t_yp);
	t = min(t, t_zp);

	return start + direction*t;
}

void main()
{
    outColor = vec4(0,1,0,1);

    vec3 start = vec3(7*16-1,7*16-1, 0);
    vec3 direction = vec3(-1,-5,0);

    int count =0;
    vec3 pos = gl_FragCoord.xyz;

    do
	{
		count++;

        start = get_voxel(start, direction);

        if(ivec3(pos) == ivec3(start))
            outColor = vec4(0);
		//start = voxel_traversal(start, direction);
        //start += direction;
		
	}
	while(count < 1000);
}