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
#include "Chunk.hpp"

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
	Chunk test = Chunk();
	/*load_obj("Assets/Objs/cube.obj", (vector<float>*) &shapes[0].vertices, 
		(vector<float>*) &shapes[0].normals, (vector<float>*) &shapes[0].uvs);*/

    while (!glfwWindowShouldClose(window))
	{
		if(!loadViewProjMatrix(cam, programs[0]))
		{
			cerr << "Error when loading projection matrix!" << endl;
			return;
		}
		glClearColor(0, 0.7f, 1.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		test.render_chunk();

		glfwPollEvents();
		glfwSwapBuffers(window);

		openGLerror();
	}
}

//cleanup
void end_rendering(GLFWwindow* window)
{
    	//Cleanup
	for(Shader s: shaders)
        deleteShader(s);
    for(GLuint p: programs)
        glDeleteProgram(p);
    for(Geometry g: shapes)
		deleteGeometry(g);

	Cube::cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
}
//########################################################################################


