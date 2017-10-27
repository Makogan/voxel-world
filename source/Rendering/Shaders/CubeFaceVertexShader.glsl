//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.1
*
*	Basic vertex shader
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#version 450

#define PI 3.1415926535897932384626433832795

layout(location = 0) in vec3 position; //(x,y,z) coordinates of a vertex
layout(location = 1) in vec3 norm; //a 3D vertex representing the normal to teh vertex 
layout(location = 2) in vec2 texture_coordinate;

layout(std430, binding = 3) buffer face_buffer
 {
     int face_types[];
 };

layout(std430, binding = 4) buffer translation_buffer
{
    vec3 translation[];
};

out vec3 normal; 
out vec3 vertexPos; //projected vertex
out vec2 texture_coord;

uniform mat4 model = mat4(1); //Position and orientation of the current object
uniform mat4 view = mat4(1); //Camera orientation and position
uniform mat4 proj = mat4(1); //the projection parameters (FOV, viewport dimensions)
//uniform int face_types[2048];

//Taken from: https://gist.github.com/neilmendoza/4512992
mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
    //int face_type = face_types[gl_InstanceID];
    mat4 rotation = mat4(1);
    switch(face_types[gl_InstanceID])
    {
        case 0:
            rotation = mat4(1);
            break;
        case 1:
            rotation = rotationMatrix(vec3(0,0,1), -PI/2.f);
            break;
        case 2:
            rotation = rotationMatrix(vec3(0,0,1), PI/2.f);
            break;
        case 3:
            rotation = rotationMatrix(vec3(0,0,1), PI);
            break;
        case 4:
            rotation = rotationMatrix(vec3(1,0,0), PI/2.f);
            break;
        case 5:
            rotation = rotationMatrix(vec3(1,0,0), -PI/2.f);
            break;
        
        default:
            rotation = mat4(1);
            break;
    }

    gl_Position = proj*view*model*rotation*vec4(position+translation[gl_InstanceID], 1.0);
    normal = vec3(model*vec4(norm,1.0));
    vertexPos = vec3(model*vec4(position, 1.0)); //calculate the transformed pos
    texture_coord = texture_coordinate + vec2(face_types[gl_InstanceID]*(1/6.f),0);
}
