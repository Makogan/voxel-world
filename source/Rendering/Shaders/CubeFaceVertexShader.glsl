//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.2
*
*	Basic vertex shader
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#version 450

#define PI 3.1415926535897932384626433832795

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

layout(location = 0) in vec3 position; //(x,y,z) coordinates of a vertex
layout(location = 1) in vec3 norm; //a 3D vertex representing the normal to teh vertex 
layout(location = 2) in vec2 texture_coordinate; // texture coordinates

layout(std430, binding = 3) buffer face_buffer
{
    vec4 face_info[];//first 3 values are position of face, final value is face type 
};

layout(std430, binding = 4) buffer index_buffer
{
   ivec2 solids_meta_data[];
};

layout(std430, binding = 5) buffer object_buffer
{
    Silhouette w_solids[];
};

layout(std430, binding = 6) buffer light_buffer
{
    Light w_lights[];
};

out float visible;
out vec3 normal; 
out vec3 vertexPos; //projected vertex
out vec2 texture_coord;

out Light lights[];
out Silhouette solids[];

uniform mat4 view = mat4(1); //Camera orientation and position
uniform mat4 proj = mat4(1); //the projection parameters (FOV, viewport dimensions)

void main()
{
    texture_coord = texture_coordinate;// + vec2((face_info[gl_InstanceID][3])*(1/6.f),0);
    gl_Position = proj*view*(vec4(position, 1.0) + vec4(vec3(face_info[gl_InstanceID]),0));
    normal = normalize(norm);
    vertexPos = vec3(vec4(position+vec3(face_info[gl_InstanceID]), 1.0)); //calculate the transformed pos

    lights = w_lights;   
    solids = w_solids;
}