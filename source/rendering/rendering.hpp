//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.1
*
*	Header to expose rendering functions to other compilation units.
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#pragma once

#define GLEW_DYNAMIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../global.hpp"
#include "wavefront-loader.hpp"

using namespace std;
using namespace glm;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Structure definitions
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
struct Shader
{
	string fileName;
	GLuint shaderID;
	GLuint type;
	GLuint program;
};

struct Geometry
{
	GLuint vertexArray;

	GLuint vertexBuffer;
	GLuint elmentBuffer;
	GLuint normalsBuffer;

	vector<vec3> vertices;
	vector<uint> indices;
	vector<vec3> normals;
	vector<vec2> uvs;
};

struct Texture
{
	GLuint textureID;
	GLuint target;
	int width;
	int height;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	List of function headers:
*/
//========================================================================================
void loadGeometryArrays(GLuint program, Geometry &g);
void setDrawingMode(int mode, GLuint program);
void render(GLuint program, Geometry &g, GLenum drawType);

void compileShader(GLuint &shader, string &filepath, GLenum shaderType);
void initDefaultShaders(vector<Shader> &shaders);
void initDefaultProgram(vector<GLuint> &programs, vector<Shader> &shaders);
void createShader(Shader &s, string file, GLenum type);
void deleteShader(Shader &s);

void createGeometry(Geometry &g, vector<vec3> vertices, vector<uint> indices);
void createGeometry(Geometry &g);
void deleteGeometry(Geometry &g);

string loadSourceFile(string &filepath);

GLuint createShadingProgram(GLuint vertexShader, GLuint fragmentShader);
int loadViewProjMatrix(Camera &c, GLuint &program);
int loadColor(vec4 color, GLuint program);
int loadCamera(vec3 cameraPos, GLuint program);

void loop(GLFWwindow*);
void end(GLFWwindow* window);
//########################################################################################

//change this, this is very bad practice!
extern vector<GLuint> programs;
extern vector<Shader> shaders;
extern vector<Geometry> shapes;