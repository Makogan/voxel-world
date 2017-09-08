#include "rendering.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

bool InitializeTexture(Texture* texture, const char* filename, GLuint target = GL_TEXTURE_2D)
{
	int numComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(filename, &texture->width, &texture->height, &numComponents, 0);
	if (data != nullptr)
	{
		texture->target = target;
		glGenTextures(1, &texture->textureID);
		glBindTexture(texture->target, texture->textureID);
		GLuint format = numComponents == 3 ? GL_RGB : GL_RGBA;
		//cout << numComponents << endl;
		glTexImage2D(texture->target, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, data);

		// Note: Only wrapping modes supported for GL_TEXTURE_RECTANGLE when defining
		// GL_TEXTURE_WRAP are GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Clean up
		glBindTexture(texture->target, 0);
		stbi_image_free(data);
	}
	return true; //error
}

void DestroyTexture(Texture *texture)
{
	glBindTexture(texture->target, 0);
	glDeleteTextures(1, &texture->textureID);
}
//--------------------------------------------------------------------------------------\\

//========================================================================================
/*
*	Rendering Functions:
*/
//========================================================================================
void setDrawingMode(int mode, GLuint program)
{
	glUseProgram(program);
	GLint loc = glGetUniformLocation(program, "drawMode");
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{
		cerr << "Error returned when trying to find uniform location."
			<< "\nuniform: drawMode"
			<< "Error num: " << loc
			<< endl;
	}
	glUniform1i(loc, mode);
}

//Need more versions of this:
void loadGeometryArrays(GLuint program, Geometry &g)
{
	glUseProgram(program);

	glBindVertexArray(g.vertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, g.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, g.vertices.size()*sizeof(vec3),
		g.vertices.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);

	if(g.normals.size()>0)
	{
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, g.normalsBuffer);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vec3), (void*)0);
		glBufferData(GL_ARRAY_BUFFER, g.normals.size()*sizeof(vec3),
			g.normals.data(), GL_DYNAMIC_DRAW);
	}

	if(g.indices.size()>0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.elmentBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, g.indices.size()*sizeof(uint),
			g.indices.data(), GL_DYNAMIC_DRAW);
	}
}

void render(GLuint program, Geometry &g, GLenum drawType)
{
	glUseProgram(program);

	glBindVertexArray(g.vertexArray);

	if(g.indices.size()>0)
		glDrawElements(drawType, g.indices.size(), GL_UNSIGNED_INT, (void*)0);

	else
		glDrawArrays(drawType, 0, g.vertices.size());
}

int loadColor(vec4 color, GLuint program)
{
	glUseProgram(program);
	GLint loc = glGetUniformLocation(program, "color");
	/*if (loc == -1)
	{
		cerr << "Uniform: error loading \"color\"." << endl;
		return -1;
	}*/
	glUniform4f(loc, color[0], color[1], color[2], color[3]);

	return 1;
}

int loadViewProjMatrix(Camera &c, GLuint &program)
{
	glUseProgram(program);
	GLint loc = glGetUniformLocation(program, "view");
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{
		cerr << "Error returned when trying to find uniform location."
			<< "\nuniform: view"
			<< "Error num: " << loc
			<< endl;
		return -1;
	}
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(c.getViewMatrix()));

	loc = glGetUniformLocation(program, "proj");
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{

		cerr << "Error returned when trying to find uniform location."
			<< "\nuniform: proj"
			<< "Error num: " << loc
			<< endl;
		return -1;
	}
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(c.getPerspectiveMatrix()));

	return 0;
}

int loadCamera(vec3 cameraPos, GLuint program)
{
	glUseProgram(program);
	GLint loc = glGetUniformLocation(program, "cameraPos");
	if (loc == -1)
	{
		cerr << "Uniform: \"cameraPos\" not found." << endl;
		return -1;
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
void createShader(Shader &s, string file, GLenum type)
{
	s.fileName = file;
	compileShader(s.shaderID, file, type);
	s.type = GL_VERTEX_SHADER;
	s.program = 0;
}

void deleteShader(Shader &s)
{
	glUseProgram(0);
	glDeleteShader(s.shaderID);
	s.program = 0;
}

GLuint createShadingProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	return shaderProgram;
}

void compileShader(GLuint &shader, string &filename, GLenum shaderType)
{
	string source = loadSourceFile(filename);
	const GLchar* s_ptr = source.c_str();

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &s_ptr, NULL);

	glCompileShader(shader);

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
void createGeometry(Geometry &g, vector<vec3> vertices, vector<uint> indices)
{
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &(g.vertexBuffer));
	glBindBuffer(GL_ARRAY_BUFFER, g.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vec3),
		vertices.data(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &(g.elmentBuffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.elmentBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size()*sizeof(uint),
		indices.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glGenBuffers(1, &g.normalsBuffer);

	glGenVertexArrays(1, &(g.vertexArray));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	g.vertices=vertices;
	g.indices=indices;
}

void createGeometry(Geometry &g)
{
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &(g.vertexBuffer));

	glGenBuffers(1, &(g.elmentBuffer));

	glEnableVertexAttribArray(1);
	glGenBuffers(1, &g.normalsBuffer);

	glGenVertexArrays(1, &(g.vertexArray));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

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

/*
*	InitDefaultProgram and InitDefaultShader are examples on how to initialize
*	the OpenGL pipeline. It is recommended to re-implement these.
*/
void initDefaultProgram(vector<GLuint> &programs, vector<Shader> &shaders)
{
	programs.push_back(glCreateProgram());
	glAttachShader(programs[0], shaders[0].shaderID);
	glAttachShader(programs[0], shaders[1].shaderID);

	glLinkProgram(programs[0]);
	glUseProgram(programs[0]);

	shaders[0].program=programs[0];
	shaders[1].program=programs[0];
}

void initDefaultShaders(vector<Shader> &shaders)
{
	Shader s1;
	shaders.push_back(s1);

	createShader(shaders[0], "./Shaders/VertexShader.glsl", GL_VERTEX_SHADER);

	Shader s2;
	shaders.push_back(s2);

	createShader(shaders[1], "./Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
}

vector<GLuint> programs;
vector<Shader> shaders;
vector<Geometry> shapes(2);

void loop(GLFWwindow* window)
{
    while (!glfwWindowShouldClose(window))
	{
		if(loadViewProjMatrix(cam, programs[0])!=0)
			return;

		glClearColor(0, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		loadColor(vec4(1,1,1,1), programs[0]);
		setDrawingMode(1, programs[0]);
		shapes[0].vertices.clear();
		load_obj("Objs/corner1.obj", (vector<float>*) &shapes[0].vertices, (vector<float>*) &shapes[0].normals, (vector<float>*) &shapes[0].uvs);
		loadGeometryArrays(programs[0], shapes[0]);
		render(programs[0], shapes[0], GL_TRIANGLES);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

void end(GLFWwindow* window)
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