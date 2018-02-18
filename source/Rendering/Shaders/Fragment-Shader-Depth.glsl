//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       Fragment-Shader-Depth.glsl
 *	@author 	Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief Basic fragment shader
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#version 450

in vec4 FragPos;
/*out vec4 color;*/

uniform vec3 position = vec3(80,70,10);
uniform float max_distance = 256;

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - position);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / max_distance;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
    /*color = vec4(lightDistance);*/
}  