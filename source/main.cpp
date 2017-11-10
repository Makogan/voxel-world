//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.2
*
*	References:
*	https://open.gl
*	http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
*	http://www.glfw.org/docs/latest/
*
*	Note: Based on the Boiler Plate written by Dr. Sonny Chan, University of Calgary,
*		Alberta Canada.
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "Window-Management.hpp"
#include "Cube.hpp"
#include "World.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	List of function headers:
*/
//========================================================================================
//TODO: document
void render_loop(GLFWwindow* window);
void end_rendering(GLFWwindow* window);

//########################################################################################

//**************************************************************************************\\

	/*	MAIN FUNCTION 	*/

//**************************************************************************************\\
//--------------------------------------------------------------------------------------\\

int main(int argc, char **argv)
{
	//Init OpenGL
	GLFWwindow* window = create_context();

	Rendering_Handler = new Renderer();

	int width, height;
    glfwGetWindowSize(window, &width, &height);
	Rendering_Handler->set_camera(new Camera(mat3(1), vec3(0,-20,0), width, height));
	
	//Render loop
	render_loop(window);
	//cleanup
	end_rendering(window);
}

//**************************************************************************************\\

//========================================================================================
/*
*	Core functions:
*/
//========================================================================================

//TODO: verify that the following are up to date and well refactored.
/*
* The following functions are not final at all, if modifications can be done, do them
*/

#include <stdlib.h>     /* srand, rand */
int const size = 256;
int const mask = size-1;

int perm[ size ];
float vec_field_x[ size ], vec_field_y[ size ];
void vec_field_init()
{
    for ( int index = 0; index < size; ++index ) 
    {
        int other = rand() % ( index + 1 );
        if ( index > other )
            perm[ index ] = perm[ other ];
        perm[ other ] = index;
        vec_field_x[ index ] = cosf( 2.0f * M_PI * index / size );
        vec_field_y[ index ] = sinf( 2.0f * M_PI * index / size );
    }
}

//main render loop
void render_loop(GLFWwindow* window)
{
	//Set default OpenGL values for rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glPointSize(10.f);

	World c = World();
	double prevTime = 0, currentTime=0;
	//TODO: this is temprorary, implement this correctly
	loadTexture(Rendering_Handler->current_program, *(Cube::textures[0]));
	
	vec_field_init();

	glUseProgram(Rendering_Handler->current_program);
	GLint loc = glGetUniformLocation(Rendering_Handler->current_program, "perm");
	glUniform1iv(loc, size, (GLint*)&perm);

	loc = glGetUniformLocation(Rendering_Handler->current_program, "vec_field_x");
	glUniform1fv(loc, size, (GLfloat*)&vec_field_x);

	loc = glGetUniformLocation(Rendering_Handler->current_program, "vec_field_y");
	glUniform1fv(loc, size, (GLfloat*)&vec_field_y);

    while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		Rendering_Handler->update(window);

		//currentTime = glfwGetTime();
		c.render_world();

		//cout << (currentTime-prevTime) << endl;
		//prevTime = currentTime;

		openGLerror();
	}
}

//cleanup
void end_rendering(GLFWwindow* window)
{


	Cube::cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
}
//########################################################################################


