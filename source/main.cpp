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

	Rendering_Handler = new Renderer();

	int width, height;
    glfwGetWindowSize(window, &width, &height);
	Rendering_Handler->set_camera(new Camera(mat3(-1), vec3(0,20,0), width, height));
	
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

vector<vec3> face = {vec3(-0.5,0.5,-0.5), vec3(-0.5,0.5,0.5), vec3(0.5,0.5,0.5), vec3(0.5,0.5,-0.5)};
vector<vec3> normals = {vec3(0,1,0),vec3(0,1,0),vec3(0,1,0),vec3(0,1,0)};
vector<uint> indices = {0,1,2,2,3,0};
vector<vec2> uvs = {vec2(0), vec2(0,1), vec2(1/6.f,1), vec2(1/6.f,0)};

//main render loop
void render_loop(GLFWwindow* window)
{
	//Set default OpenGL values for rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glPointSize(10.f);

	Cube test = Cube(); 
	//Chunk test = Chunk();
	//Mega_Chunk mega_test = Mega_Chunk();
	//mega_test.chunk_block();
	/*load_obj("Assets/Objs/cube.obj", (vector<float>*) &shapes[0].vertices, 
		(vector<float>*) &shapes[0].normals, (vector<float>*) &shapes[0].uvs);*/

	GLuint testVAO;
	vector<GLuint> testVBOs(4);
	vector<GLuint> types = {GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER};
	glGenVertexArrays(1, &testVAO);
	glGenBuffers(4,testVBOs.data());

	glBindVertexArray(testVAO);

    glBindBuffer(GL_ARRAY_BUFFER, testVBOs[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
    glBufferData(GL_ARRAY_BUFFER, face.size()*sizeof(vec3), 
        face.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, testVBOs[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vec3), (void*)0);
    glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(vec3),
       normals.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, testVBOs[2]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
    glBufferData(GL_ARRAY_BUFFER, uvs.size()*sizeof(vec2),
       	uvs.data(), GL_DYNAMIC_DRAW);

    //TODO: figure out if other buffers are faster
    /*vector<int> face_types = {Top, Bottom, Left, Right, Back, Front};
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, experiment);
    glBufferData(GL_SHADER_STORAGE_BUFFER, face_types.size()*sizeof(Face), face_types.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, experiment);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

    vector<vec3> move = {vec3(0), vec3(1), vec3(0,0,1), vec3(1,0,0), vec3(0,1,0), vec3(4)};
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, experiment2);
    glBufferData(GL_SHADER_STORAGE_BUFFER, move.size()*sizeof(vec3), move.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, experiment2);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind*/

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, testVBOs[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint),
        indices.data(), GL_DYNAMIC_DRAW);

    while (!glfwWindowShouldClose(window))
	{
		Rendering_Handler->update(window);
		/*glBindVertexArray(testVAO);
		glUseProgram(Rendering_Handler->current_program);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, testVBOs[0]);
		glBindBuffer(GL_ARRAY_BUFFER, testVBOs[1]);	
		glBindBuffer(GL_ARRAY_BUFFER, testVBOs[2]);	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, testVBOs[3]);
		
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);*/
		Rendering_Handler->multi_render(testVAO, &testVBOs, &types, 2, indices.size(), 1);
		//test.render_cube();
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

	//Cube::cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
}
//########################################################################################


