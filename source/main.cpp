//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.1
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
#include "Rendering.hpp"
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

//TODO: verify that the following are up to date and well refactored. move to main?
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

	//Cube test = Cube(); 
	//Chunk test = Chunk();
	//Mega_Chunk mega_test = Mega_Chunk();
	//mega_test.chunk_block();
	/*load_obj("Assets/Objs/cube.obj", (vector<float>*) &shapes[0].vertices, 
		(vector<float>*) &shapes[0].normals, (vector<float>*) &shapes[0].uvs);*/

	/*Chunk c = Chunk(vec3(10));
	Chunk c2 = Chunk(vec3(-10));*/
	World c = World();
	double prevTime = 0, currentTime=0;

    while (!glfwWindowShouldClose(window))
	{
		Rendering_Handler->update(window);
		
		//Rendering_Handler->multi_render(testVAO, &testVBOs, &types, 5, indices.size(), face_types.size());
		//test.render_cube();
		currentTime = glfwGetTime();
		c.render_world();
		//c2.render_chunk();
		//cout << (currentTime-prevTime) << endl;
		prevTime = currentTime;

		openGLerror();
	}
}

//cleanup
void end_rendering(GLFWwindow* window)
{


	//Cube::cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
}
//########################################################################################


