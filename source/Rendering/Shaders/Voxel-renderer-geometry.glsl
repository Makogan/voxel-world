//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *	@file		Voxel-renderer-geoemtry.glsl
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

out float h;

uniform float height = 128;

uniform float voxel_size = 1;

void main()
{
    for(int layer = 0; layer < int(height); layer++)
    {
        gl_Layer = layer; // built-in variable that specifies to which layer we render.
        for(int i = 0; i < 3; i++) // for each triangle's vertices
        {
            vec4 pos = gl_in[i].gl_Position;
            h = pos.z;

            pos.z -= (layer+1);
        
            if(pos.z >=0 && pos.z < voxel_size*0.9999)
                pos.z = 1;
            else 
                pos.z = 2;
            gl_Position = pos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}
