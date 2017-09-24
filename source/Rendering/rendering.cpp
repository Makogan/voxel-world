//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.1
*
*	File to define methods that abstract and simplify the use of opengl for rendering.
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "rendering.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Global Values
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

vector<GLuint> programs;//Global shading programs list
vector<Shader> shaders;//Global Shaders list 
//TODO: Maybe delete Geometry list and change it for a different data structure or 
//		Avoid it alltogether
vector<Geometry> shapes(2);//Global Shapes list Temporary!
vector<Texture> textures(2); //Temporary

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Rendering Functions:
*/
//========================================================================================

//TODO: look into making other functions that also load geometry info into the shaders
//TODO: comment undocumented functions
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

/*
*	Function to load geometry information into an OpenGL shader program
*
*	Params:
*		program: the ID of the shader program into which we will looad the Geometry info
*		g:	the Geometry structure containing the information to load into teh program
*/
void loadGeometryArrays(GLuint program, Geometry &g)
{
	glUseProgram(program);//Make program the current shading program

	glBindVertexArray(g.vertexArray);//bind g's vertex array to be OpenGL's vertex arr

	glBindBuffer(GL_ARRAY_BUFFER, g.vertexBuffer);//set OpenGL's vertex buffer (vertex data)
	//Set the buffer data with, type, size of buffer in bytes, array pointer, drawing mode
	glBufferData(GL_ARRAY_BUFFER, g.vertices.size()*sizeof(vec3), 
		g.vertices.data(), GL_DYNAMIC_DRAW);

	//Wich layout in the shader to pass current information (layout 0 is vertex for us)
	glEnableVertexAttribArray(0);
	//Specify how to read the data: layout index, components per vertex attribute, 
	//Type of the data, normalization, byte offset between consecutive elements, 
	//offset of first component
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);

	//Check if there are normals to process for the geometry object
	if(g.normals.size()>0)
	{
		//Set attribute layout to 1 (layout 1 is normals for us)
		glEnableVertexAttribArray(1);
		//Set the buffer array
		glBindBuffer(GL_ARRAY_BUFFER, g.normalsBuffer);
		//As above specify how to read the information
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vec3), (void*)0);
		//Fill the buffer array with the data 
		glBufferData(GL_ARRAY_BUFFER, g.normals.size()*sizeof(vec3),
			g.normals.data(), GL_DYNAMIC_DRAW);
	}

	//Check if there are texture coordinates to process for the geometry object
	if(g.uvs.size()>0)
	{
		//Set attribute layout to 2 (layout 2 is uvs for us)
		glEnableVertexAttribArray(2);
		//Set the buffer array
		glBindBuffer(GL_ARRAY_BUFFER, g.uvBuffer);
		//As above specify how to read the information
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(vec2), (void*)0);
		//Fill the buffer array with the data 
		glBufferData(GL_ARRAY_BUFFER, g.uvs.size()*sizeof(vec2),
			g.uvs.data(), GL_DYNAMIC_DRAW);
	}

	//Check if there are any indices specified in g
	if(g.indices.size()>0)
	{
		//Set the element buffer (non linear reading/loading of data information)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.elmentBuffer);
		//Indicate how to read the element data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, g.indices.size()*sizeof(uint),
			g.indices.data(), GL_DYNAMIC_DRAW);
	}
}

/*
*	Fundamental rendering function, it assumes initialization of all global values 
*	used to organize the process of sending information to the GPU for rendering
*
*	Params:
*		program: OpenGL shading program used to render 
*		g: Geometry object containing mesh information
*		drawType: the drawing mode (Lines, triangles, points, strips...)
*/
void render(GLuint program, Geometry &g, GLenum drawType)
{
	//Set the OpenGL shading program
	glUseProgram(program);

	//Bind the VAO of the geometry
	glBindVertexArray(g.vertexArray);

	//Chose appropriate drawing method based on whether there are indices in the
	//geometry strucutre has explicit indices
	if(g.indices.size()>0)
		//non sequencial vertex drawing
		glDrawElements(drawType, g.indices.size(), GL_UNSIGNED_INT, (void*)0);

	else
		//sequential vertex drawing
		glDrawArrays(drawType, 0, g.vertices.size());
}

/*
*	Load the color uniform for the fragment shader (Assumes existence of a color uniform)
*
*	Params:
*		color: a representation of the RGBA values of a color
*		program: the OpenGL shading program to use
*/
void loadColor(vec4 color, GLuint program)
{
	glUseProgram(program); 	//Active program
	GLint loc = glGetUniformLocation(program, "color");
	glUniform4f(loc, color[0], color[1], color[2], color[3]);
}

/*
*	Load camera parameters into a shading program for perspective projection
*	
*	Params:
*		c: the camera struct used to create the projection matrix
*		program: program in wich to load the projection matrix
*	
*	return: 0 on error or 1 on success
*/
int loadViewProjMatrix(Camera &c, GLuint &program)
{
	glUseProgram(program);
	GLint loc = glGetUniformLocation(program, "view");
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{
		cerr << "Error returned when trying to find view matrix."
			<< "\nuniform: view"
			<< "Error num: " << loc
			<< endl;
		return 0;
	}
	//Pass the calculated view matrix onto the shader
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(c.getViewMatrix()));

	loc = glGetUniformLocation(program, "proj");
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{

		cerr << "Error returned when trying to find projection matrix."
			<< "\nuniform: proj"
			<< "Error num: " << loc
			<< endl;
		return 0;
	}
	//Pass the calculated projection/perspective matrix onto the shader
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(c.getPerspectiveMatrix()));

	return 1;
}


/*
* Load camera position into the current shader program
*
*	Params:
*		c: the camera struct used to create teh projection matrix
*		program: program in wich to load the projection matrix
*	
*	return: 0 on error or 1 on success
*/
int loadCamera(vec3 cameraPos, GLuint program)
{
	glUseProgram(program);
	GLint loc = glGetUniformLocation(program, "cameraPos");
	if (loc == -1)
	{
		cerr << "Uniform: \"cameraPos\" not found." << endl;
		return 0;
	}
	glUniform3f(loc, cameraPos[0], cameraPos[1], cameraPos[2]);

	return 1;
}
//########################################################################################

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
	s.program = 0;
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
	s.program = 0;
}

/*
* Compile a glsl file and generate an OpenGL shading program on teh GPU
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
*	Geometry Functions:
*/
//========================================================================================

//TODO: comment this
void createGeometry(Geometry &g, vector<vec3> vertices,  vector<vec3> normals, 
	vector<vec2> uvs, vector<uint> indices)
{
	//set vertex info
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &(g.vertexBuffer));
	glBindBuffer(GL_ARRAY_BUFFER, g.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec3),
		vertices.data(), GL_DYNAMIC_DRAW);

	//set normals info
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &g.normalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g.normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(vec3),
		normals.data(), GL_DYNAMIC_DRAW);

	//set texture coordinates info
	glEnableVertexAttribArray(2);
	glGenBuffers(1, &g.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, g.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size()*sizeof(vec2),
		uvs.data(), GL_DYNAMIC_DRAW);

	//set element info
	glGenBuffers(1, &(g.elmentBuffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.elmentBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size()*sizeof(uint),
		indices.data(), GL_DYNAMIC_DRAW);

	//Init VAO
	glGenVertexArrays(1, &(g.vertexArray));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	g.vertices=vertices;
	g.normals=normals;
	g.uvs=uvs;
	g.indices=indices;
}

/*
*	Initialize the fields of a geometry object using arrays
*
*	Params:
*		g: the geometry struct into which the info will be loaded
*		vertices: the vertex info of the geometry
*		indices: the index information (non-sequencial association of vertices) 
*/
void createGeometry(Geometry &g, vector<vec3> vertices, vector<uint> indices)
{
	//set vertex info
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &(g.vertexBuffer));
	glBindBuffer(GL_ARRAY_BUFFER, g.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec3),
		vertices.data(), GL_DYNAMIC_DRAW);

	//set element info
	glGenBuffers(1, &(g.elmentBuffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.elmentBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size()*sizeof(uint),
		indices.data(), GL_DYNAMIC_DRAW);

	//set normals info
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &g.normalsBuffer);

	//set normals info
	glEnableVertexAttribArray(2);
	glGenBuffers(1, &g.uvBuffer);

	//Init VAO
	glGenVertexArrays(1, &(g.vertexArray));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	g.vertices=vertices;
	g.indices=indices;
}

/*
*	Initialize the fields of a default geometry struct
*
*	Params:
*		g: the geometry struct into which the info will be loaded
*/
void createGeometry(Geometry &g)
{
	//set vertices
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &(g.vertexBuffer));

	//set normals
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &g.normalsBuffer);

	//set uvs
	glEnableVertexAttribArray(2);
	glGenBuffers(1, &g.uvBuffer);

	//set indices
	glGenBuffers(1, &(g.elmentBuffer));

	//init VAO
	glGenVertexArrays(1, &(g.vertexArray));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*
* Delete a geometry struct
*	
*	Params: 
*		g: the geometry struct to delete
*/
void deleteGeometry(Geometry &g)
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &(g.vertexArray));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &(g.vertexBuffer));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &(g.elmentBuffer));
}
//########################################################################################

//========================================================================================
/*
*	Texture Functions:
*/
//========================================================================================

/*
*	Initialize the fields of a textureobject using arrays
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
		glTexParameteri(texture.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(texture.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

//########################################################################################

//========================================================================================
/*
*	Core functions:
*/
//========================================================================================

//TODO: verify that the following are up to date and well refactored
/*
* The following functions are not final at all, if modifications can be done, do them
*/

//main render loop
void render_loop(GLFWwindow* window)
{
	shapes[0].vertices.clear();
	load_obj("Assets/Objs/cube.obj", (vector<float>*) &shapes[0].vertices, 
		(vector<float>*) &shapes[0].normals, (vector<float>*) &shapes[0].uvs);

    while (!glfwWindowShouldClose(window))
	{
		if(!loadViewProjMatrix(cam, programs[0]))
		{
			cerr << "Error when loading projection matrix!" << endl;
			return;
		}
		glClearColor(0, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		loadColor(vec4(1,1,1,1), programs[0]);
		loadGeometryArrays(programs[0], shapes[0]);
		loadTexture(programs[0], textures[0]);

		render(programs[0], shapes[0], GL_TRIANGLES);

		glfwPollEvents();
		glfwSwapBuffers(window);
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

    glfwDestroyWindow(window);
    glfwTerminate();
}
//########################################################################################