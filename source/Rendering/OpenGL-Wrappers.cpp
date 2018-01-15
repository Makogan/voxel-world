//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.2
*
*   Wrapper structures to abstract OpenGL function calls
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

Renderer *Rendering_Handler;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Shader Functions:
*/
//========================================================================================

/*
*	Initialize the fields of a shader object using a glsl shader file
*
*	Params:
*		s: the shader struct into which the info will be loaded
*		file: the file path (relative or absolute) where the shader program is defined 
*		type: the type of shader (e.g vertex,fragment, tesselation...)
*/
void createShader(Shader &s, string file, GLenum type)
{
	s.fileName = file;
	compileShader(s.shaderID, file, type);
	s.type = GL_VERTEX_SHADER;
	//s.program = 0;
}

/*
* Delete a shader struct
*	
*	Params: 
*		s: the shader struct to delete
*/
void deleteShader(Shader &s)
{
	glUseProgram(0);
	glDeleteShader(s.shaderID);
	//s.program = 0;
}

/*
* Compile a glsl file and generate an OpenGL shading program on the GPU
*
*	Params:	
*		shader: where the shader ID will be returned
*		filename: file path to the glsl program definition
*		type: the type of shader (e.g vertex,fragment, tesselation...)
*/
void compileShader(GLuint &shader, string &filename, GLenum shaderType)
{
	string source = loadSourceFile(filename);
	const GLchar* s_ptr = source.c_str();//get raw c string (char array)

	shader = glCreateShader(shaderType);//create shader on GPU
	glShaderSource(shader, 1, &s_ptr, NULL);//set shader program source

	glCompileShader(shader);


	//verify compilation
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status!=GL_TRUE)
	{
		cout << "Shader compilation error. Could not compile: "
		<< filename << "\nShader type: "
		<< shaderType
		<<endl;

		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		string log(length, ' ');
		glGetShaderInfoLog(shader, log.length(), &length, &log[0]);

		cerr<< endl << source <<endl;
		cerr << endl << log <<endl;
	}
}

/*
* Copy a string from a file into a a string
*	
*	Params:
*		filepath: path to the file
*
*	Return: a string that is the copy of the source file
*/
string loadSourceFile(string &filepath)
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
//########################################################################################


//========================================================================================
/*
*	Mesh Structure definition:
*/
//========================================================================================

Mesh::~Mesh()
{
	delete(vertices);
	delete(normals);
	delete(indices);
	delete(uvs);
}

//########################################################################################

//========================================================================================
/*
*	Texture Functions:
*/
//========================================================================================

/*
*	Initialize the fields of a texture object using arrays
*
*	Params:
*		texture: a pointer to a texture struct into which the info will be loaded
*		filename: the filepath to the texture file
*		target: the OpenGL texture target (e.g 2D, rectangle...)
*
*	Return: a boolean value indicating whether an error ocurred (true means no error)
*/
bool createTexture(Texture &texture, const char* filename, GLuint target)
{
	int numComponents;
	stbi_set_flip_vertically_on_load(true);
	void *data = stbi_load(filename, &texture.width, &texture.height, &numComponents, 0);
	if (data != nullptr)
	{
		texture.target = target;
		glGenTextures(1, &texture.textureID);
		glBindTexture(texture.target, texture.textureID);
		GLuint format = numComponents == 3 ? GL_RGB : GL_RGBA;
		//cout << numComponents << endl;
		glTexImage2D(texture.target, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, data);

		// Note: Only wrapping modes supported for GL_TEXTURE_RECTANGLE when defining
		// GL_TEXTURE_WRAP are GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER
		glTexParameteri(texture.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(texture.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(texture.target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(texture.target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Clean up
		glBindTexture(texture.target, 0);
		stbi_image_free(data);

		return true;
	}
	else
	{
		cerr << "Problem when loading texture" << endl;
	}
	return false; //error
}

/*
* Delete a texture struct
*	
*	Params: 
*		texture: the texture struct to delete
*/
void DestroyTexture(Texture &texture)
{
	glBindTexture(texture.target, 0);
	glDeleteTextures(1, &texture.textureID);
}

void loadTexture(GLuint program, Texture &t)
{
	glUseProgram(program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t.textureID);
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
//########################################################################################

//========================================================================================
/*
*	Renderer Class implementation:
*/
//========================================================================================

/*
*	Default constructor for the Renderer Class
*/
Renderer::Renderer()
{
	render_queue.reserve(4096);
	//Create cube rendering shader
	vertex_shaders.push_back(Shader());
	createShader(vertex_shaders[0],"./Shaders/CubeFaceVertexShader.glsl", GL_VERTEX_SHADER);

	//create fragment shader
	fragment_shaders.push_back(Shader());
	createShader(fragment_shaders[0], "./Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);

	//Initialize and create the first rendering program
	shading_programs.push_back(glCreateProgram());

	glAttachShader(shading_programs[0], vertex_shaders[0].shaderID);
	glAttachShader(shading_programs[0], fragment_shaders[0].shaderID);

	glLinkProgram(shading_programs[0]);
	current_program = shading_programs[0];
	glUseProgram(0);
}

/*
*	Class destructor
*/
Renderer::~Renderer()
{
	for(Shader s: vertex_shaders)
		deleteShader(s);
	for(Shader s: fragment_shaders)
		deleteShader(s);
	for(Shader s: tessellation_shaders)
		deleteShader(s);
	for(GLuint p: shading_programs)
		glDeleteProgram(p);
}

/*
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
		cerr << "Mismatching VBO's and buffer_types sizes" << endl;
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

/* 
* 	Update general rendering values
*/
//TODO: Camera update may need to be changed to all programs
void Renderer::update(GLFWwindow* window)
{
	//GLFW updtae functions
	glfwPollEvents();
	glfwSwapBuffers(window);

	glClearColor(0, 0.7f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Update general uniform values in the rendering program
	glUseProgram(current_program);
	GLint loc = glGetUniformLocation(current_program, "view");
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{
		cerr << "Error returned when trying to find view matrix."
			<< "\nuniform: view"
			<< "Error num: " << loc
			<< endl;
		return;
	}
	//Pass the calculated view matrix onto the shader
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(cam->getViewMatrix()));

	loc = glGetUniformLocation(current_program, "proj");
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{

		cerr << "Error returned when trying to find projection matrix."
			<< "\nuniform: proj"
			<< "Error num: " << loc
			<< endl;
		return;
	}
	//Pass the calculated projection/perspective matrix onto the shader
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(cam->getPerspectiveMatrix()));
	glUseProgram(0);
}

/*
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

/*
*	Add a new shader to the set of all shaders
*/
void Renderer::add_Shader(string shader, GLuint type)
{
	switch(type)
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
	}
}

//TODO: check only base file name somehow
Shader* Renderer::find_shader(string shader_name)
{
	for(uint i=0; i<vertex_shaders.size(); i++)
		if(shader_name==vertex_shaders[i].fileName)
			return &vertex_shaders[i];
	
	for(uint i=0; i<fragment_shaders.size(); i++)
		if(shader_name==fragment_shaders[i].fileName)
			return &fragment_shaders[i];

	for(uint i=0; i<tessellation_shaders.size(); i++)
		if(shader_name==tessellation_shaders[i].fileName)
			return &tessellation_shaders[i];
	
	return NULL;
}


void Renderer::add_data(Render_Info* data)
{
	//busy_queue.lock();
	render_queue.push_back(data);
	//busy_queue.unlock();
}

void Renderer::render()
{
	busy_queue.lock();

	for(uint i=0; i<render_queue.size(); i++)
	{
		Render_Info *render_data = render_queue[i]; 
		multi_render(render_data->VAO, &(render_data->VBOs), 
			&(render_data->types), render_data->layouts, 
			render_data->geometry->indices->size(), render_data->render_instances);
	}

	busy_queue.unlock();
}

void Renderer::clear()
{
	render_queue.clear();
}
//########################################################################################