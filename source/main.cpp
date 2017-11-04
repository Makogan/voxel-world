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


