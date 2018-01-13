//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.2
*
*	Header to define variables, structure definitoins, include libraries... 
*   Shared among all rendering functions.
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
#include <thread>
#include <mutex>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Camera.hpp"
#include "cout-definitions.hpp"

using namespace std;
using namespace glm;

enum Data_Type {Undefined=0, Uint, Int,  Float};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Structure definitions
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//TODO: verify the following data structures, maybe program shoud be a list to allow
//  for multiple programs
struct Shader
{
	string fileName;    //name of file
	GLuint shaderID;    //generated OpenGL shader ID
	GLuint type;        //shader type
	//GLuint program;     //Associated OpenGL shading program
};

struct Texture
{
	GLuint textureID;   //OpenGL generated ID for the texture
    GLuint target;      //OpenGL target (Usuallly 2D texture or rectangle) check OpenGL doc

    //Dimensions of the texture
	int width;         
	int height;
};

struct Mesh
{
    vector<vec3> *vertices;  //Vertex data
    vector<vec3> *normals;   //Normal data
	vector<uint> *indices;   //Element data (sequence in which data will be read)
    vector<vec2> *uvs;       //Texture data for this geometry 
                            //(the associated coordinates on the mesh)

    ~Mesh();
};

struct Render_Info
{
    mutex info_lock;
    GLuint VAO;
    vector<GLuint> VBOs;
    vector<GLuint> types;
    uint layouts;
    uint render_instances;
    Mesh* geometry;
};

/*
* General rendering manager class
*/
class Renderer
{
    private:
        vector<GLuint> shading_programs;
        vector<Shader> vertex_shaders;
        vector<Shader> fragment_shaders;
        vector<Shader> tessellation_shaders;
        vector<Render_Info*> render_queue;
    
    public:
        mutex global_lock;
        mutex busy_queue;
        Camera *cam;
        GLuint current_program;
        Renderer();
        ~Renderer();

        Shader* find_shader(string shader_name);

        void update(GLFWwindow* window);
        void add_Shader(string shader, GLuint type);
        void make_program(vector<uint> *shaders);
        void set_camera(Camera *new_cam);
        void addVBO(); //TODO: figure this one out
        void multi_render(GLuint VAO, vector<GLuint> *VBOs, 
            vector<GLuint> *buffer_types, GLuint layout_num, 
            GLuint index_num, GLuint instances);
        void change_active_program(GLuint newProgram);
        void add_data(Render_Info*);
        void render();
        void clear();
};

extern Renderer *Rendering_Handler;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Global Values
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	List of function headers:
*/
//========================================================================================

//Check for OpenGL errors
int openGLerror();

void loadTexture(GLuint program, Texture &t);

void compileShader(GLuint &shader, string &filepath, GLenum shaderType);
void createShader(Shader &s, string file, GLenum type);
void deleteShader(Shader &s);

bool createTexture(Texture &texture, const char* filename, GLuint target = GL_TEXTURE_2D);
void loadTexture(GLuint program, Texture &t);
void DestroyTexture(Texture &texture);

string loadSourceFile(string &filepath);

GLuint createShadingProgram(GLuint vertexShader, GLuint fragmentShader);

//########################################################################################