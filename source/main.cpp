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
#include <thread>
#include <unistd.h>
#include <chrono>         
#include <ctime>          

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
void update_loop(GLFWwindow*, GLFWwindow*);
//########################################################################################

//**************************************************************************************\\

	/*	MAIN FUNCTION 	*/

//**************************************************************************************\\
//--------------------------------------------------------------------------------------\\

World* w;
int main(int argc, char **argv)
{
	//Init OpenGL
	GLFWwindow* window = create_context(NULL);
	GLFWwindow* o_window = create_context(window);

	Rendering_Handler = new Renderer();

	int width, height;
    glfwGetWindowSize(window, &width, &height);
	Rendering_Handler->set_camera(new Camera(mat3(1), 
		vec3(5*CHUNK_DIMS,5*CHUNK_DIMS,2*CHUNK_DIMS), width, height));

	glfwMakeContextCurrent(window);

	w = new World();
	thread world_thread(update_loop, window, o_window);
	//update_loop(window);

	//Render loop
	render_loop(window);

	world_thread.join();

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
mutex temp;
typedef std::chrono::duration<int, std::ratio<1, 60>> frame_duration;
void render_loop(GLFWwindow* window)
{

	//Set default OpenGL values for rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glPointSize(10.f);

	//World c = World();
	
	double prevTime = 0, currentTime=0;
	//TODO: this is temporary, implement this correctly
	loadTexture(Rendering_Handler->current_program, *(Cube::textures[0]));

    while (!glfwWindowShouldClose(window))
	{
		auto start_time = std::chrono::steady_clock::now();
		Rendering_Handler->update(window);

		currentTime=glfwGetTime();
		double elapsed = currentTime-prevTime;
		
		temp.lock();
		Rendering_Handler->render();
		prevTime=currentTime;

		cout << 1.d/elapsed << endl;
		auto end_time = start_time + frame_duration(1);
		temp.unlock();
		glFinish();
		std::this_thread::sleep_until(end_time);

		openGLerror();
	}
}

void update_loop(GLFWwindow* window, GLFWwindow* o_window)
{
	//World c = World();
	glfwMakeContextCurrent(o_window);

	while (!glfwWindowShouldClose(window))
	{
			w->center_frame(ivec3(Rendering_Handler->cam->getPosition()));
			temp.lock();
			w->send_render_data(Rendering_Handler);
			temp.unlock();
			glFinish();
			usleep(600);	
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


