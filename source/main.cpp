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
typedef std::chrono::duration<int, std::ratio<1, 10>> world_duration;

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

	Rendering_Handler = new Renderer();
	int width, height;
    glfwGetWindowSize(window, &width, &height);
	Rendering_Handler->set_camera(new Camera(mat3(1), 
		vec3(5*CHUNK_DIMS,5*CHUNK_DIMS,2*CHUNK_DIMS), width, height));

	glfwMakeContextCurrent(window);

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
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glPointSize(10.f);
	
	double prevTime = 0, currentTime=0;
	//TODO: this is temporary, implement this correctly
	loadTexture(Rendering_Handler->current_program, *(Cube::textures[0]));

    while (!glfwWindowShouldClose(window))
	{
		auto start_time = std::chrono::steady_clock::now();
		Rendering_Handler->update(window);

		currentTime=glfwGetTime();
		double elapsed = currentTime-prevTime;
		
		Rendering_Handler->render();
		prevTime=currentTime;

		cout << 1.d/elapsed << endl;
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


