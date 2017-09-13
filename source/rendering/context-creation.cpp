//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.1
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

#include "context-creation.hpp"

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
    
    callBackInit(window);

    // glew initilization, this is so that the program is crossplatform,
    // also things won't work without it
    glewExperimental = GL_TRUE;
    glewInit(); glGetError();
    // An error will always be thrown when initializing glew.
    // It can be safely discarded so we call glGetError() to delete it and move on.
	
	shaders.push_back(Shader());
	shaders.push_back(Shader());
	createShader(shaders[0], "./Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
	createShader(shaders[1], "./Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);

	programs.push_back(glCreateProgram());
	glAttachShader(programs[0], shaders[0].shaderID);
	glAttachShader(programs[0], shaders[1].shaderID);
	glLinkProgram(programs[0]);
	glUseProgram(programs[0]);
	shaders[0].program=programs[0];
	shaders[1].program=programs[0];
    // Example code, delete or modify
    //**********************************************************************************
    
       /* initDefaultShaders(shaders);
        initDefaultProgram(programs, shaders);*/
    
        createGeometry(shapes[0]);
        createGeometry(shapes[1]);
    //***********************************************************************************
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    cam = *(new Camera(mat3(1), vec3(0,-20,0), width, height));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPointSize(10.f);

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

double calculateFPS(double prevTime, double currentTime)
{
	double elapsedTime = currentTime - prevTime;
	return 1/elapsedTime;
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

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height-40,
		"OpenGL Template", NULL, NULL);
	if (!window)
	{

		cerr<< "Failed to glfwCreateWindow.\nTerminating program." << endl;
		return NULL;
	}
	//glfwMaximizeWindow(window);
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
    	cam.move(vec3(0,CAM_SPEED,0));

    else if(key == GLFW_KEY_S)
    	cam.move(vec3(0,-CAM_SPEED,0));

    else if(key == GLFW_KEY_A)
    	cam.move(vec3(-CAM_SPEED,0,0));

    else if(key == GLFW_KEY_D)
    	cam.move(vec3(CAM_SPEED,0,0));

    else if(key == GLFW_KEY_Q)
    	cam.move(vec3(0,0,-CAM_SPEED));

    else if(key == GLFW_KEY_E)
    	cam.move(vec3(0,0, CAM_SPEED));

    else if(key == GLFW_KEY_KP_6)
    	cam.turnH(radians(-1.f));

    else if(key == GLFW_KEY_KP_4)
    	cam.turnH(radians(1.f));

    else if(key == GLFW_KEY_KP_8)
    	cam.turnV(radians(1.f));

    else if(key == GLFW_KEY_KP_2)
    	cam.turnV(radians(-1.f));

    else if(key == GLFW_KEY_KP_ADD)
    	cam.incline(radians(1.f));

    else if(key == GLFW_KEY_KP_SUBTRACT)
    	cam.incline(radians(-1.f));

    else if(key == GLFW_KEY_KP_MULTIPLY)
    	cam.resetView();

    else if(key == GLFW_KEY_KP_DIVIDE)
    	cam.resetCamera();
}
//########################################################################################