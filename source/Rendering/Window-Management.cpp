//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.2
*
*	File defining all relevant OpenGL and GLFW related functions needed to create an 
*	OpenGL context and GLFW window.
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "Window-Management.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Global Values
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Opengl Context Creation:
*/
//========================================================================================

/*
* Function to create the OpenGL context.
* 
* return GLFWwindow*: the pointer to the GLFW window containing the current context.
*/
GLFWwindow* create_context()
{
    GLFWwindow* window = createWindow();
	if(window==NULL)
	{
		cerr << "Error when creating GLFW window" << endl;
	}
    callBackInit(window);

    // glew initilization, this is so that the program is crossplatform,
    // also things won't work without it
    glewExperimental = GL_TRUE;
    glewInit(); glGetError();
    // An error will always be thrown when initializing glew.
    // It can be safely discarded so we call glGetError() to delete it and move on.

	return window;
}
//########################################################################################

//========================================================================================
/*
*	Error checking and debugging functions:
*/
//========================================================================================

/*
* Check for OpenGL errors and print the appropriate error message if needed.
*
* return int: the number of the generated error.
*/
int openGLerror()
{
	GLenum errorNum = glGetError();
	switch(errorNum)
	{
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			cerr << "An unacceptable value is specified for an enumerated argument."
				<< endl;
			break;
		case GL_INVALID_VALUE:
			cerr << "A numeric argument is out of range."
				<< endl;
			break;
		case GL_INVALID_OPERATION:
			cerr << "The specified operation is not allowed in the current state."
				<< endl;
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			cerr << "The framebuffer object is not complete."
				<< endl;
			break;
		case GL_OUT_OF_MEMORY:
			cerr << "There is not enough memory left to execute the command."
				<< endl;
			break;
		default:
			cerr<< "Undefined error, you messed up big time now."
				<< endl;
	}
	return errorNum;
}
//########################################################################################

//========================================================================================
/*
*	Initialization:
*/
//========================================================================================
//Initialize GLFW callBack Functions
void callBackInit(GLFWwindow* window)
{
	//Set GLFW callback functions
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

}

/*
* Method to create a GLFW window, window will be maximized and decorated.
*
* return GLFWwindow*: a pointer to teh created window.
*/
GLFWwindow* createWindow()
{
	//Initialize GLFW
	if (!glfwInit())
	{
		cerr<< "Failed to initialize GLFW.\nTerminating program." << endl;
		return NULL;
	}

	//Get the primiray monitor of the current system's info
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//OpenGL major version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);//OpenGL minor version
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//Set Forward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//Use GLFW defaults
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);//Make the window decorated
	//Create a GLFW window with the main monitors width, reduced height, 
	//name, on windowed mode, not sharing resources with any context
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height-40, 
		"Voxel World", NULL, NULL);
	if (!window)//Check for errors
	{
		cerr<< "Failed to glfwCreateWindow.\nTerminating program." << endl;
		return NULL;
	}
	//TODO: delete or uncomment at one point
	//glfwMaximizeWindow(window);//Make the window maximized
	//Set the current window to be the current OpenGL context
	glfwMakeContextCurrent(window);

	return window;
}
//########################################################################################

//========================================================================================
/*
*	GLFW helper functions:
*/
//========================================================================================

int cursorSelectNode(GLFWwindow *window)
{
	/*double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	mat4 view= cam.getViewMatrix();
	mat4 proj= cam.getPerspectiveMatrix();

	uint count = 0;
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	vec3 screenPos;
	vec3 pos;
	float depth;
	vec3 projCursor;*/
	return 0;
}
//########################################################################################

//========================================================================================
/*
*	GLFW callback functions:
*/
//========================================================================================

/*
* GLFW callback functions for Interrupt based event handling.
* More information at http://www.glfw.org/docs/latest/input_guide.html
*/

void error_callback(int error, const char* description)
{
    cout << "Error: " << description << endl;
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

#define CAM_SPEED 0.5f
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    else if(key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
    	//Get the primary monitor and the monitor attached to the current window
    	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    	GLFWmonitor* monitor = glfwGetWindowMonitor(window);

    	//If no monitor is attached to the window attach the primary monitor
    	//This makes the window full screen
    	if(monitor==NULL)
    		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0,
    			mode->width, mode->height, mode->refreshRate);

    	//If a monitor is attached, detach it and set the dimesnions to that of
    	//The primary's monitor resolution. This makes the window windowed
    	else
    		glfwSetWindowMonitor(window, NULL, 0, 0,
    			mode->width, mode->height-1, mode->refreshRate);
    			//subtract 1 to prevent it from going into full screen mode

    	glfwMaximizeWindow(window);
    }

    else if(key == GLFW_KEY_R && action == GLFW_PRESS)
   	{
   	}

    else if(key == GLFW_KEY_F12 && action == GLFW_PRESS)
    	cout << glfwGetVersionString() << endl;

    else if(key == GLFW_KEY_W)
    	Rendering_Handler->cam->move(normalize(Rendering_Handler->cam->getForward()));

    else if(key == GLFW_KEY_S)
    	Rendering_Handler->cam->move(-normalize(Rendering_Handler->cam->getForward()));

    else if(key == GLFW_KEY_A)
    	Rendering_Handler->cam->move(-normalize(Rendering_Handler->cam->getSide()));

    else if(key == GLFW_KEY_D)
		Rendering_Handler->cam->move(normalize(Rendering_Handler->cam->getSide()));

    else if(key == GLFW_KEY_Q)
		Rendering_Handler->cam->move(normalize(Rendering_Handler->cam->getUp()));

    else if(key == GLFW_KEY_E)
		Rendering_Handler->cam->move(normalize(-Rendering_Handler->cam->getUp()));

    else if(key == GLFW_KEY_KP_6)
    	Rendering_Handler->cam->turnH(radians(-1.f));

    else if(key == GLFW_KEY_KP_4)
    	Rendering_Handler->cam->turnH(radians(1.f));

    else if(key == GLFW_KEY_KP_8)
    	Rendering_Handler->cam->turnV(radians(-1.f));

    else if(key == GLFW_KEY_KP_2)
    	Rendering_Handler->cam->turnV(radians(1.f));

    else if(key == GLFW_KEY_KP_ADD)
    	Rendering_Handler->cam->incline(radians(1.f));

    else if(key == GLFW_KEY_KP_SUBTRACT)
    	Rendering_Handler->cam->incline(radians(-1.f));

    else if(key == GLFW_KEY_KP_MULTIPLY)
    	Rendering_Handler->cam->resetView();

    else if(key == GLFW_KEY_KP_DIVIDE)
    	Rendering_Handler->cam->resetCamera();
}
//########################################################################################