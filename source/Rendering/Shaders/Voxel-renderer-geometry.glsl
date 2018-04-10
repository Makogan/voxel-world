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
uniform float width = 128;
uniform float depth = 128;

uniform float voxel_size = 0;
uniform vec3 origin = vec3(0,0,0);


void main()
{
    float min_x, max_x, min_y, max_y, min_z, max_z;

    min_x = min(gl_in[0].gl_Position.x, gl_in[1].gl_Position.x);
    min_x = min(min_x, gl_in[2].gl_Position.x);

    min_y = min(gl_in[0].gl_Position.y, gl_in[1].gl_Position.y);
    min_y = min(min_y, gl_in[2].gl_Position.y);

    min_z = min(gl_in[0].gl_Position.z, gl_in[1].gl_Position.z);
    min_z = min(min_z, gl_in[2].gl_Position.z);

    max_x = max(gl_in[0].gl_Position.x, gl_in[1].gl_Position.x);
    max_x = max(max_x, gl_in[2].gl_Position.x);

    max_y = max(gl_in[0].gl_Position.y, gl_in[1].gl_Position.y);
    max_y = max(max_y, gl_in[2].gl_Position.y);

    max_z = max(gl_in[0].gl_Position.z, gl_in[1].gl_Position.z);
    max_z = max(max_z, gl_in[2].gl_Position.z);

    float mid_height = (max_z-min_z)/2.f + min_z;

    for(int layer = 0; layer < int(height); layer++)
    {
        gl_Layer = layer; // built-in variable that specifies to which layer we render.
        for(int i = 0; i < 3; i++) // for each triangle's vertices
        {
            vec4 pos = gl_in[i].gl_Position;//vec4(origin,0);
            h = pos.z;

            vec4 low_bounds = floor(pos/voxel_size)*voxel_size;
            vec4 high_bounds = ceil(pos/voxel_size)*voxel_size;

            if(pos.z > mid_height)
            {
                pos.x = high_bounds.x;
                pos.y = high_bounds.y;
            }

            else
            {
                pos.x = low_bounds.x;
                pos.y = low_bounds.y;
            }

            pos /= voxel_size;
            pos.w = 1;

            pos.x = (2.f*pos.x-width)/(width);
            pos.y = (2.f*pos.y-depth)/(depth);

            pos.z -= (layer+1);
        
            if(pos.z >=0 && pos.z <= 1)
                pos.z = 1;
            else 
                pos.z = 2;
                
            gl_Position = pos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}
