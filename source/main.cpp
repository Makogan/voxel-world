//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file 		main.cpp
 *	@author		Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief main file. Thread and global loop definitoins go here, as well as 
 *	initialization
 *
 *  References:
 *	https://open.gl
 *	http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
 *	http://www.glfw.org/docs/latest/
 *	http://eastfarthing.com/blog/2015-04-21-noise/
 *	http://www.seas.upenn.edu/~pcozzi/OpenGLInsights/OpenGLInsights-SparseVoxelization.pdf
 *	https://github.com/Friduric/voxel-cone-tracing
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "system-libraries.hpp"        

#include "Window-Management.hpp"
#include "Cube.hpp"
#include "World.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Globals:
*/
//========================================================================================

typedef std::chrono::duration<int, std::ratio<1, 60>> frame_duration;
typedef std::chrono::duration<int, std::ratio<1, 60>> world_duration;

//########################################################################################

//========================================================================================
/*
*	List of function headers:
*/
//========================================================================================
//TODO: document
void render_loop(GLFWwindow* window);
void update_loop(GLFWwindow*, GLFWwindow*);
//########################################################################################

//**************************************************************************************\\

	/*	MAIN FUNCTION 	*/

//**************************************************************************************\\
//--------------------------------------------------------------------------------------\\

int main(int argc, char **argv)
{
	//Init OpenGL contexts
	GLFWwindow* window = create_context(NULL, true);
	GLFWwindow* o_window = create_context(window, false);
    
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwMakeContextCurrent(window);

	Rendering_Handler = new Renderer(width, height);

	Cube::initialize();

	//World thread
	the_world = new World();
	thread world_thread(update_loop, window, o_window);

	//Render loop
	render_loop(window);
	world_thread.join();

	//cleanup
	Cube::cleanup();
	glfwDestroyWindow(o_window);
	glfwDestroyWindow(window);
	glfwTerminate();
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
	//glClearDepth(0);
	//glDepthFunc(GL_GREATER);
	//glDepthRange(0, 10000);
    glEnable(GL_DEPTH_TEST);
	glPointSize(10.f);
	
	double prevTime = 0, currentTime=0;
	Rendering_Handler->set_voxelizer_dimensions(
		(the_world->h_radius)*CHUNK_DIMS, 
		(the_world->h_radius)*CHUNK_DIMS, 
		(the_world->v_radius)*CHUNK_DIMS
	);
	
    while (!glfwWindowShouldClose(window))
	{

		auto start_time = std::chrono::steady_clock::now();
		Rendering_Handler->update(window);

		currentTime=glfwGetTime();
		Rendering_Handler->set_voxelizer_origin(the_world->origin);
		double elapsed = currentTime-prevTime;
		Rendering_Handler->render();
		prevTime=currentTime;

		//cout << 1.d/elapsed << endl;
		auto end_time = start_time + frame_duration(1);
		glFinish();
		std::this_thread::sleep_until(end_time);

		openGLerror();
	}
}

void update_loop(GLFWwindow* window, GLFWwindow* o_window)
{
	glfwMakeContextCurrent(o_window);

	while (!glfwWindowShouldClose(window))
	{
			auto start_time = std::chrono::steady_clock::now();
			the_world->center_frame(ivec3(Rendering_Handler->cam->getPosition()));
			the_world->send_render_data(Rendering_Handler);
			glFinish();
			auto end_time = (start_time + world_duration(1));	
			std::this_thread::sleep_until(end_time);
	}
}

//########################################################################################


