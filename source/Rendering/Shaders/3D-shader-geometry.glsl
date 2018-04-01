//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *	@file		3D-shader-geometry.glsl
 *	@author		Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief Basic geometry shader
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#version 450

layout (triangles) in;
layout (triangle_strip, max_vertices=136) out;

uniform vec3 cameraPos = vec3(0);//The position of the camera in the world
uniform vec3 cameraDir = vec3(0);
uniform float fov = 45;

in vec3 v_normal[]; 
in vec3 v_vertexPos[]; //projected vertex
in vec2 v_texture_coord[];

out vec3 normal;//Normal to the vertex
out vec3 vertexPos;//True position of the vertex (i.e it's location in space)
out vec2 texture_coord;

void main()
{
    for(int i = 0; i < 3; i++) // for each triangle's vertices
    {   
        normal=v_normal[i];
        vertexPos=v_vertexPos[i];
        texture_coord=v_texture_coord[i];

        vec4 pos = gl_in[i].gl_Position;
        vec3 dir = normalize(vertexPos- cameraPos);
        // dir.z=0;
        vec3 cDir= cameraDir;
        //cDir.z=0;

        if(acos(dot(dir, cDir)) < radians(fov))
        {
            gl_Position = pos;
            EmitVertex();
        }
    }    
    EndPrimitive();
}
