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
typedef std::chrono::duration<int, std::ratio<1, 600>> world_duration;

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

GLuint DEVAO;


/* SCOTT ERROR */

#define lcase(X)	case(X): cout << #X << endl; break;

void check_glerror(){
	GLenum error = (glGetError());
	switch (error){
		case(GL_NO_ERROR) :
		default :
//			cout << GL_NO_ERROR << ":" << error << endl;
			return;
		case(GL_INVALID_ENUM):
			cout << "GL_INVALID_ENUM" << endl;
			break;
		case(GL_INVALID_VALUE):
			cout << "GL_INVALID_VALUE" << endl;
			break;
		case(GL_INVALID_OPERATION):
			cout << "GL_INVALID_OPERATION" << endl;
			break;
		case(GL_INVALID_FRAMEBUFFER_OPERATION):
			cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl;
			break;
		case(GL_OUT_OF_MEMORY):
			cout << "GL_OUT_OF_MEMORY" << endl;
			break;
		case(GL_STACK_UNDERFLOW):
			cout << "GL_STACK_UNDERFLOW" << endl;
			break;
		case(GL_STACK_OVERFLOW):
			cout << "GL_STACK_OVERFLOW" <<endl;
			break;
	}
}

void GL_error_callback(GLenum source, GLenum type, GLuint id,
   GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
	if( true && severity != GL_DEBUG_SEVERITY_NOTIFICATION){

	cout << "GL ERROR CALLBACK: " << endl;
	cout << "Source: " << source << " : ";
	switch (source){
		lcase(GL_DEBUG_SOURCE_API)
		lcase(GL_DEBUG_SOURCE_WINDOW_SYSTEM)
		lcase(GL_DEBUG_SOURCE_SHADER_COMPILER)
		lcase(GL_DEBUG_SOURCE_THIRD_PARTY)
		lcase(GL_DEBUG_SOURCE_APPLICATION)
		lcase(GL_DEBUG_SOURCE_OTHER)				//THESE ARN'T DEFINED?
	}  

	cout << "Type: " <<  type << " : ";
	switch (type){
		lcase(GL_DEBUG_TYPE_ERROR)
		lcase(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
		lcase(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
		lcase(GL_DEBUG_TYPE_PORTABILITY)
		lcase(GL_DEBUG_TYPE_PERFORMANCE)
		lcase(GL_DEBUG_TYPE_MARKER)
		lcase(GL_DEBUG_TYPE_PUSH_GROUP)
		lcase(GL_DEBUG_TYPE_POP_GROUP)
		lcase(GL_DEBUG_TYPE_OTHER)
	}
		
	cout << "ID: " << id << endl;
	cout << "Serverity: " << severity << " : ";
	switch (severity){
		lcase(GL_DEBUG_SEVERITY_HIGH)
		lcase(GL_DEBUG_SEVERITY_MEDIUM)
		lcase(GL_DEBUG_SEVERITY_LOW)
		lcase(GL_DEBUG_SEVERITY_NOTIFICATION)
	}
	cout << "Length: " << length << endl;
	cout << "Message: " << message << endl;
	cout << "UserParam: " << userParam << endl;
	cout << "END: GL ERROR CALLBACK: " << endl;

	}
}



int main(int argc, char **argv)
{
	//Init OpenGL contexts
	GLFWwindow* window = create_context(NULL, true);
	GLFWwindow* o_window = create_context(window, false);
    
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwMakeContextCurrent(window);

	glDebugMessageCallback(	GL_error_callback, NULL);
	glEnable(GL_DEBUG_OUTPUT);								//DEBUG :D
	glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS);

	Rendering_Handler = new Renderer();

	Rendering_Handler->set_camera(new Camera(mat3(1), 
		vec3(5*CHUNK_DIMS,5*CHUNK_DIMS,2*CHUNK_DIMS), width, height));

	glGenVertexArrays(1, &DEVAO);

	the_world = new World();

	glBindVertexArray(DEVAO);
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
GLuint DEVBO;
void render_loop(GLFWwindow* window)
{
	//Set default OpenGL values for rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glPointSize(10.f);
	glFinish();
	
	double prevTime = 0, currentTime=0;
	//TODO: this is temporary, implement this correctly
	loadTexture(Rendering_Handler->current_program, *(Cube::textures[0]));
	
    while (!glfwWindowShouldClose(window))
	{
		auto start_time = std::chrono::steady_clock::now();
		Rendering_Handler->update(window);

		currentTime=glfwGetTime();
		double elapsed = currentTime-prevTime;
		DEVBO = the_world->VBOs[0];
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


