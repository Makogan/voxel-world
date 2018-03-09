//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file 		Renderer.cpp
 *	@author		Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *   @brief Renderer class implementation
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "Renderer.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Renderer Class implementation:
*/
//========================================================================================
enum Shader_type {SHADER_3D=0, SHADER_VOXELIZER};
enum FBO_type {FBO_DEFAULT=0, FBO_TEXTURE};

/**
 *	Default constructor for the Renderer Class
 */
Renderer::Renderer(){}

/**
 * Constructor for the Renderer class. Creates a renderer object that handles all render
 * calls. It's intended to be unique but has not been implemented as a singleton
 * be weary!
 */
Voxel_Map *vMap;
Renderer::Renderer(int width, int height)
{
	render_queue.reserve(4096);

	string s1, s2, s3;
	s1 = "./Shaders/3D-shader-vertex.glsl";
	s2 = "./Shaders/3D-shader-fragment.glsl";
	shading_programs.push_back(Shading_Program(
		&s1,
		NULL,
		NULL,
		&s2
	));

	s1 = "./Shaders/Voxel-renderer-vertex.glsl";
	s2 = "./Shaders/Voxel-renderer-geometry.glsl";
	s3 = "./Shaders/Voxel-renderer-fragment.glsl";
	shading_programs.push_back(Shading_Program(
		&s1,
		NULL,
		&s2,
		&s3
    ));
    
    FBOs.push_back(0);
    FBOs.push_back(0);
	glGenFramebuffers(1, &FBOs[1]);

	vMap = new Voxel_Map(7*16*2, 7*16*2, 4*16*2);

	current_program = shading_programs[SHADER_3D].programID;
	glUseProgram(current_program);

	//create the camera
	set_camera(new Camera(mat3(1), 
		vec3(0,0,10), width, height));

	openGLerror();
}

/**
 *	Class destructor
 */
Renderer::~Renderer() {}

/**
 *	Function to render multiple instances of the same mesh
 * 	index_num is the number of indices in the mesh (for drawing elements)
 *	layout_num is the number of layouts to enable (always 0 to layou_num-1)
 */
void Renderer::multi_render(GLuint VAO, vector<GLuint> *VBOs, 
	vector<GLuint> *buffer_types, GLuint layout_num, 
	GLuint index_num, GLuint instances)
{
	//error check
	if(VBOs->size() != buffer_types->size())
	{
		cerr << "Mismatching VBOs's and buffer_types sizes" << endl;
		return;
	}

	//Bind Vertex array object and rendering rpogram
	glBindVertexArray(VAO);
	glUseProgram(current_program);

	//enable shader layouts
	for(int i=0; i<layout_num;i++)
		glEnableVertexAttribArray(i);

	//Bind VBO's storing rendering data
	for(uint i=0; i<buffer_types->size(); i++)
	{
		if((*buffer_types)[i]==GL_SHADER_STORAGE_BUFFER)
		{
			glBindBuffer((*buffer_types)[i], (*VBOs)[i]);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, (*VBOs)[i]);
		}
	}
	//Draw call
	glDrawElementsInstanced(GL_TRIANGLES, index_num, GL_UNSIGNED_INT, (void*)0, instances);
}

/**
 * Error checking and message function for uniforms.
 */ 
GLint inline Renderer::get_uniform_location(string name)
{
    GLint loc = glGetUniformLocation(current_program, name.c_str());
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{
		cerr << "Error returned when trying to find uniform " << name << endl;
		exit(0);
    }
    
    return loc;
}

void inline Renderer::load_uniform(mat4 matrix, string name)
{
    GLint loc = get_uniform_location(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(matrix));
}

void inline Renderer::load_uniform(vec4 vector, string name)
{
    GLint loc = get_uniform_location(name);
    glUniform4fv(loc, 1, (GLfloat*)&(vector));
}

void inline Renderer::load_uniform(vec3 vector, string name)
{
    GLint loc = get_uniform_location(name);
    glUniform3fv(loc, 1, (GLfloat*)&(vector));
}

void inline Renderer::load_uniform(float num, string name)
{
    GLint loc = get_uniform_location(name);
    glUniform1f(loc, num);
}

void inline Renderer::load_uniform(double num, string name)
{
    GLint loc = get_uniform_location(name);
    glUniform1f(loc, num);
}

void inline Renderer::load_uniform(int num, string name)
{
    GLint loc = get_uniform_location(name);
    glUniform1i(loc, num);
}
/** 
 * 	Update general rendering values
 */
void Renderer::update(GLFWwindow* window)
{
	//GLFW updtae functions
	glfwPollEvents();
	glfwSwapBuffers(window);

	//clear the screen
	glClearColor(0, 0.7f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Update general uniform values in the rendering program
    glUseProgram(current_program);
    load_uniform(cam->getViewMatrix(), "view");
    load_uniform(cam->getPerspectiveMatrix(), "proj");
    load_uniform(cam->getPosition(), "camPos");
    load_uniform(cam->getForward(), "cameraDir");

	glUseProgram(0);
}

/**
 *	Initialize the main rendering camera
 */
void Renderer::set_camera(Camera *new_cam)
{
	if(new_cam==NULL)
	{
		cerr << "NULL camera given to renderer" << endl;
		return;
	}

	cam=new_cam;
}

/**
 * Add a rendereable 3D object to the current render queue
 */ 
void Renderer::add_data(Object_3D* data)
{
	render_queue.push_back(data);
}

void Renderer::draw()
{
	
	for(uint i=0; i<render_queue.size(); i++)
	{
		Object_3D *render_data = render_queue[i]; 
		//Render multiple instances of the current object
		multi_render(render_data->VAO, &(render_data->VBOs), 
			&(render_data->types), render_data->layouts, 
			render_data->mesh_indices, render_data->render_instances);
	}
	
}

//TODO: refactor this into a memeber field and function of Renderer
extern bool changed;
/**
 * Render all elements in the current render queue
 */
void Renderer::render()
{
    //prevent other threads from writing to the queue
	busy_queue.lock();

	current_program = shading_programs[SHADER_VOXELIZER].programID;
	glUseProgram(current_program);

	glViewport(0, 0, 7*16*2, 7*16*2);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOs[FBO_TEXTURE]);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		vMap->textureID, 0);

	glClearColor(0.f, 0.f, 0.f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw();	

	//glGenerateTextureMipmap(vMap->textureID);

	glViewport(0, 0, cam->getWidth(), cam->getHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, FBOs[FBO_DEFAULT]);
	current_program = shading_programs[SHADER_3D].programID;
	glUseProgram(current_program);

	vMap->load_to_GPU(current_program);
	
	draw();

	openGLerror();

	//Allow other threads to write to the queue
	busy_queue.unlock();
}

void Renderer::set_voxelizer_origin(ivec3 origin)
{
	vec3 o = vec3(origin);
	//cout << origin << endl;
	//o.z = 0;

	current_program = shading_programs[SHADER_VOXELIZER].programID;
	glUseProgram(current_program);

	load_uniform(o, "origin");
}

void Renderer::set_voxelizer_dimensions(float width, float depth, float height)
{
	current_program = shading_programs[SHADER_VOXELIZER].programID;
	glUseProgram(current_program);

	load_uniform(width, "width");
	load_uniform(height, "height");
	load_uniform(depth, "depth");

	load_uniform(0.5, "voxel_size");

	current_program = shading_programs[SHADER_3D].programID;
	glUseProgram(current_program);

	load_uniform(width, "width");
	load_uniform(height, "height");
	load_uniform(depth, "depth");

	load_uniform(0.5, "base_voxel_size");
}

/**
 * Clear all objects in the render queue
 */
void Renderer::clear()
{
	render_queue.clear();
}
//########################################################################################