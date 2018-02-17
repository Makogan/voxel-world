//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file 		OpenGL-Wrappers.cpp
 *	@author		Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *   @brief Wrapper structures to abstract OpenGL function calls
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include "system-libraries.hpp"
#include "OpenGL-Wrappers.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Global Values
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! The global render handler
Renderer *Rendering_Handler;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Shader Functions:
*/
//========================================================================================

/**
 * Default Constructor
 */ 
Shader::Shader(){}

/**
 *	Initialize the fields of a shader object using a glsl shader file
 *
 *		@param file the file path (relative or absolute) where the shader program is defined 
 *		@param type the type of shader (e.g vertex,fragment, tesselation...)
 */
Shader::Shader(string file, GLenum type)
{
	fileName = file;
	string source = load_from_file(fileName);
	const GLchar* s_ptr = source.c_str();//get raw c string (char array)

	shaderID = glCreateShader(type);//create shader on GPU
	glShaderSource(shaderID, 1, &s_ptr, NULL);//set shader program source

	glCompileShader(shaderID);
	openGLerror();

	//verify compilation
	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if(status!=GL_TRUE)
	{
		cout << "Shader compilation error. Could not compile: "
		<< fileName << "\nShader type: "
		<< type
		<<endl;

		GLint length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

		string log(length, ' ');
		glGetShaderInfoLog(shaderID, log.length(), &length, &log[0]);

		cerr<< endl << source <<endl;
		cerr << endl << log <<endl;
	}
	type = GL_VERTEX_SHADER;
}

/**
 * Destructor of a shader struct
 */
Shader::~Shader(){}

/**
 * Copy a file into a a string
 *	
 *		@param filepath path to the file
 *		@return A string that is the copy of the source file
 */
string Shader::load_from_file(string &filepath)
{
	string source;
	ifstream input(filepath.c_str());
	if (input) {
		copy(istreambuf_iterator<char>(input),
			istreambuf_iterator<char>(),
			back_inserter(source));
		input.close();
	}

	else {
		cerr << "ERROR: Could not load shader source from file: "
			<< filepath << endl;
	}
	return source;
}

/**
 * Cleanup the shader OpenGL information
 */ 
void Shader::clear()
{
	glUseProgram(0);
	glDeleteShader(shaderID);
}
//########################################################################################

//========================================================================================
/*
*	Shading_Program class definition:
*/
//========================================================================================
Shading_Program::Shading_Program(string *vs, string *ts, string *gs, string *fs)
{
	vertex = new Shader(*vs, GL_VERTEX_SHADER);
	fragment = new Shader(*fs, GL_FRAGMENT_SHADER);

	if(ts!=NULL)
		tesselation = new Shader(*ts, GL_TESS_EVALUATION_SHADER);
	if(gs!=NULL)
		geometry = new Shader(*gs, GL_GEOMETRY_SHADER);

	openGLerror();
	//Initialize and create the first rendering program
	programID = glCreateProgram();
	openGLerror();
	glAttachShader(programID, vertex->shaderID);
	openGLerror();
	glAttachShader(programID, fragment->shaderID);
	openGLerror();

	if(tesselation!=NULL)
		glAttachShader(programID, tesselation->shaderID);
	if(geometry!=NULL)
		glAttachShader(programID, geometry->shaderID);
	openGLerror();

	glLinkProgram(programID);
	openGLerror();
}
//########################################################################################

//========================================================================================
/*
*	Mesh Structure definition:
*/
//========================================================================================

/**
 * Class destructor
 */
Mesh::~Mesh(){}

//########################################################################################

//========================================================================================
/*
*	Texture Functions:
*/
//========================================================================================

/**
 *	Initialize the fields of a texture object using arrays
 *
 *		@param filename the filepath to the texture file
 *		@param targ the OpenGL texture target (e.g 2D, rectangle...)
 *		@return Boolean value indicating whether an error ocurred (true means no error)
 */
Texture::Texture(const char* filename, GLuint targ)
{
	int numComponents;
	stbi_set_flip_vertically_on_load(true);
	void *data = stbi_load(filename, &width, &height, &numComponents, 0);
	if (data != nullptr)
	{
		target = targ;
		glGenTextures(1, &textureID);
		glBindTexture(target, textureID);
		GLuint format = numComponents == 3 ? GL_RGB : GL_RGBA;
		//cout << numComponents << endl;
		glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		// Note: Only wrapping modes supported for GL_TEXTURE_RECTANGLE when defining
		// GL_TEXTURE_WRAP are GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Clean up
		glBindTexture(target, 0);
		stbi_image_free(data);
	}
	else
	{
		cerr << "Problem when loading texture" << endl;
	}
}

/**
 * Destructor of a texture struct
 */
Texture::~Texture(){}

//TODO: docuemnt
void Texture::load_to_GPU(GLuint program)
{
	glUseProgram(program);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, textureID);
	GLint loc = glGetUniformLocation(program, "text");
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{
		cerr << "Error returned when trying to find texture uniform."
			<< "\nuniform: text"
			<< "Error num: " << loc
			<< endl;
		return;
	}
	
	glUniform1i(loc,0);
}

/**
 * Clear all OpenGL information of the texture object
 */
void Texture::clear()
{
	glBindTexture(target, 0);
	glDeleteTextures(1, &textureID);
}
//########################################################################################

//========================================================================================
/*
*	Object_3D Class implementation:
*/
//========================================================================================

/**
 * Method to initialize a basic Shader layout from a vector of data. Mainly use to make
 * the code less verbose.
 */ 
template<class T>
void inline init_buffer(vector<T> data, GLuint buffer, GLenum buffer_type, GLuint layout, 
	GLboolean normalize, GLuint elements, GLenum data_type)
{
	glBindBuffer(buffer_type, buffer);
    glBufferData(buffer_type, data.size()*sizeof(T), 
		data.data(), GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(layout, elements, data_type, normalize, sizeof(T), (void*)0);
}

/**
 * Create a 3D rendereable object from a mesh
 */
Object_3D::Object_3D(Mesh *mesh)
{
	//In order:
	//Local Position: vec3; Normal: vec3; Texture Coordinate: vec2; 
	//Global Position, as SSBO: vec3; Index: uint
	types={GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, GL_ARRAY_BUFFER, 
		GL_SHADER_STORAGE_BUFFER, GL_ELEMENT_ARRAY_BUFFER};
		
	layouts = types.size()-1;

    //Create and initialize OpenGL rendering structures
    VBOs = vector<GLuint>(types.size());
    glGenVertexArrays(1, &(VAO));
	
	//generate vertex buffer objects
    glGenBuffers(types.size(),(VBOs.data()));

	//Bind the vertex array object
    glBindVertexArray(VAO);

	//Initialize vertex data
	init_buffer(mesh->vertices, VBOs[0], GL_ARRAY_BUFFER, 0, GL_FALSE, 3, GL_FLOAT);
	//Initialize normal data
	init_buffer(mesh->normals, VBOs[1], GL_ARRAY_BUFFER, 1, GL_TRUE, 3, GL_FLOAT);
	//Initialize texture coordinates data
	init_buffer(mesh->uvs, VBOs[2], GL_ARRAY_BUFFER, 2, GL_FALSE, 2, GL_FLOAT);	

	//Initialize index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (VBOs)[4]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size()*sizeof(uint),
		mesh->indices.data(), GL_DYNAMIC_DRAW);

	mesh_indices = mesh->indices.size();
}
//########################################################################################

//========================================================================================
/*
*	Renderer Class implementation:
*/
//========================================================================================
enum Shader_type {SHADER_3D=0, SHADER_DEPTH};

/**
 *	Default constructor for the Renderer Class
 */
Renderer::Renderer(){}

/**
 * Contructor for the Renderer class. Creates a renderer object that handles all render
 * calls. It's intended to be unique but has not been implemented as a singleton
 * be weary!
 */
Renderer::Renderer(int width, int height)
{
	render_queue.reserve(4096);

	string s1, s2, s3;
	s1 = "./Shaders/Vertex-Shader-3D.glsl";
	s2 = "./Shaders/Fragment-Shader-3D.glsl";
	shading_programs.push_back(Shading_Program(
		&s1,
		NULL,
		NULL,
		&s2
	));

	s1 = "./Shaders/Vertex-Shader-Depth.glsl";
	s2 = "./Shaders/Geometry-Shader-Depth.glsl";
	s3 = "./Shaders/Fragment-Shader-Depth.glsl";
	shading_programs.push_back(Shading_Program(
		&s1,
		NULL,
		&s2,
		&s3
	));

	current_program = shading_programs[SHADER_3D].programID;

	//create the camera
	set_camera(new Camera(mat3(1), 
	vec3(80,70,10), width, height));
	//TODO: refactor this
	create_shadow_map();
	openGLerror();
}

/**
 *	Class destructor
 */
Renderer::~Renderer()
{
	/*for(Shader s: vertex_shaders)
		s.clear();
	for(Shader s: fragment_shaders)
		s.clear();
	for(Shader s: tessellation_shaders)
		s.clear();
	for(GLuint p: shading_programs)
		glDeleteProgram(p);*/

}

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
void inline verify_uniform_location(GLint location, string error_message)
{
	if(location == GL_INVALID_VALUE || location==GL_INVALID_OPERATION)
	{
		cerr << error_message << endl;
		return;
	}
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
	GLint loc = glGetUniformLocation(current_program, "view");
	verify_uniform_location(loc, "Error returned when trying to find view matrix.");

	//Pass the calculated view matrix onto the shader
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(cam->getViewMatrix()));

	loc = glGetUniformLocation(current_program, "proj");
	verify_uniform_location(loc, "Error returned when trying to find projection matrix.");

	//Pass the calculated projection/perspective matrix onto the shader
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(cam->getPerspectiveMatrix()));

	//Update the camera position in the shader
	loc = glGetUniformLocation(current_program, "cameraPos");
	verify_uniform_location(loc, "Error returned when trying to find camera position");

	//load camera position into shader
	vec3 camPos = cam->getPosition();
	glUniform3fv(loc, 1, (GLfloat*)&(camPos));

	//Pass the looking direction of the camera to the shader
	loc = glGetUniformLocation(current_program, "cameraDir");
	verify_uniform_location(loc, "Error returned when trying to find camera direction");
	
	//load camera direction into shader
	vec3 camDir = normalize(cam->getForward());
	glUniform3fv(loc, 1, (GLfloat*)&(camDir));

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
 *	Add a new shader to the set of all shaders
 */
void Renderer::add_Shader(string shader, GLuint type)
{
	//select the shader type
	/*switch(type)
	{
		case (GL_VERTEX_SHADER):
			vertex_shaders.push_back(Shader());
			break;
		
		case (GL_FRAGMENT_SHADER):
			vertex_shaders.push_back(Shader());
			break;
		
		case (GL_TESS_EVALUATION_SHADER):
			vertex_shaders.push_back(Shader());
			break;

		default:
			cerr << "No shader type found" << endl;
			break;
	}*/
}

GLuint depth_map;
GLuint FramebufferName;
void Renderer::create_shadow_map()
{
	//TODO: change render program here
	glUseProgram(0);
	FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);

	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	glGenTextures(1, &depth_map);

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glBindTexture(GL_TEXTURE_CUBE_MAP, depth_map);
	for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
							SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); 

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
									 
	openGLerror();
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_map, 0);
	openGLerror();

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.
	glReadBuffer(GL_NONE);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER)==GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
			cerr << "shadow map not ok" << endl;
		exit(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void setup_light_perspectives(GLuint program)
{
	glUseProgram(program);
	vec3 lightPos = vec3(80,70,10);
	float aspect = 1;
	float near = 0.01f;
	float far = 256.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far); 

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * 
		glm::lookAt(lightPos, lightPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));

	shadowTransforms.push_back(shadowProj * 
		glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));

	shadowTransforms.push_back(shadowProj * 
		glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));

	shadowTransforms.push_back(shadowProj * 
		glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));

	shadowTransforms.push_back(shadowProj * 
		glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));

	shadowTransforms.push_back(shadowProj * 
		glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0, -1.0, 0.0)));

	GLint loc = glGetUniformLocation(program, "shadowMatrices");
	glUniformMatrix4fv(loc, shadowTransforms.size(), GL_FALSE, (GLfloat*)shadowTransforms.data());
	openGLerror();
}

void Renderer::render_shadow_map()
{
	glViewport(0, 0, 2048, 2048);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glClear(GL_DEPTH_BUFFER_BIT);
	render();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//maybe we need to do this like a uniform?
	glBindTexture(GL_TEXTURE_CUBE_MAP, depth_map);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//TODO: check only base file name somehow
//TODO: give shaders an enumerator identifier
/**
 * Find a shader through a string
 */
Shader* Renderer::find_shader(string shader_name)
{
	/*for(uint i=0; i<vertex_shaders.size(); i++)
		if(shader_name==vertex_shaders[i].fileName)
			return &vertex_shaders[i];
	
	for(uint i=0; i<fragment_shaders.size(); i++)
		if(shader_name==fragment_shaders[i].fileName)
			return &fragment_shaders[i];

	for(uint i=0; i<tessellation_shaders.size(); i++)
		if(shader_name==tessellation_shaders[i].fileName)
			return &tessellation_shaders[i];*/
	
	return NULL;
}

/**
 * Add a rendereable 3D object to the current render queue
 */ 
void Renderer::add_data(Object_3D* data)
{
	render_queue.push_back(data);
}

/**
 * Render all elements in the current render queue
 */
void Renderer::render()
{
	//prevent other threads from writing to the queue
	busy_queue.lock();
	current_program = shading_programs[SHADER_DEPTH].programID;
	glUseProgram(current_program);

	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glClear(GL_DEPTH_BUFFER_BIT);

/*	glBindTexture(GL_TEXTURE_CUBE_MAP, depth_map);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_map, 0);*/

	setup_light_perspectives(current_program);
	for(uint i=0; i<render_queue.size(); i++)
	{
		Object_3D *render_data = render_queue[i]; 
		//Render multiple instances of the current object
		multi_render(render_data->VAO, &(render_data->VBOs), 
			&(render_data->types), render_data->layouts, 
			render_data->mesh_indices, render_data->render_instances);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//maybe we need to do this like a uniform?
	current_program = shading_programs[SHADER_3D].programID;
	glUseProgram(current_program);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depth_map);
	
	glViewport(0, 0, cam->getWidth(), cam->getHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depth_map);
	GLint loc = glGetUniformLocation(current_program, "depth_map");
	glUniform1i(loc,1);
	openGLerror();

	for(uint i=0; i<render_queue.size(); i++)
	{
		Object_3D *render_data = render_queue[i]; 
		//Render multiple instances of the current object
		multi_render(render_data->VAO, &(render_data->VBOs), 
			&(render_data->types), render_data->layouts, 
			render_data->mesh_indices, render_data->render_instances);
	}
	openGLerror();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	//Allow other threads to write to the queue
	busy_queue.unlock();
}

/**
 * Clear all objects in the render queue
 */
void Renderer::clear()
{
	render_queue.clear();
}
//########################################################################################