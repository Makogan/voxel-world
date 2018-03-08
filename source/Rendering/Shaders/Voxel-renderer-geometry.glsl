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

uniform float height = 128;

void main()
{
    for(int layer = 0; layer < int(height); layer++)
    {
        gl_Layer = layer; // built-in variable that specifies to which layer we render.
        for(int i = 0; i < 3; i++) // for each triangle's vertices
        {
            vec4 pos = gl_in[i].gl_Position;
            pos.z = 0.5;
        
           /* if(voffset >=0 && voffset < voxel_size*0.999)
                pos.z = 1;
            else 
                pos.z = 2;*/

            gl_Position = pos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}