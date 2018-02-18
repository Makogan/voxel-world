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
Texture::Texture(const char* filename, GLuint target)
{
	int numComponents;
	stbi_set_flip_vertically_on_load(true);
	void *data = stbi_load(filename, &width, &height, &numComponents, 0);
	if (data != nullptr)
	{
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
*	Shadow_Map Functions:
*/
//========================================================================================

Shadow_Map::Shadow_Map()
{
	/*GLint temp; 
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &temp);
	cout << temp << endl;*/
	glGenTextures(1, &textureID);

	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
				SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); 

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	openGLerror();
}

void Shadow_Map::load_to_GPU(GLuint program, GLuint index)
{	
	glUseProgram(program);

	
	/*glUseProgram(program);
	glActiveTexture(GL_TEXTURE1+ index);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	string name = "depth_maps[" + to_string(index) + "]";

	GLint loc = glGetUniformLocation(program, name.c_str());
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{
		cerr << "Error returned when trying to find depth_map uniform."
			<< "\nuniform: " << name 
			<< "\nError num: " << loc
			<< endl;
		return;
	}
	
	glUniform1i(loc,1 + index);*/
}
//########################################################################################

//========================================================================================
/*
*	Object_3D Class implementation:
*/
//========================================================================================

/**
 * Method to initialize a basic Shader layout from a vector of data. Mainly used to make
 * the code less verbose.
 */ 
template<class T>
void inline init_buffer(vector<T> &data, GLuint buffer, GLenum buffer_type, GLuint layout, 
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
