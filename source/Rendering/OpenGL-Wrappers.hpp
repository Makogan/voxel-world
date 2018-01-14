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


#include "system-libraries.hpp"

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
    GLuint VAO;             //Vertex Array Object
    vector<GLuint> VBOs;    //array of VBO Ids
    vector<GLuint> types;   //Array of VBO types 
    uint layouts;           //The number of layouts to activate
    uint render_instances;  //Number of instances to render current object
    Mesh* geometry;         //Mesh to render
};

//TODO: many things in here should be memeber functions of the respective structures
/*
* General rendering manager class
*/
class Renderer
{
    private:
        vector<GLuint> shading_programs;        //Shading programs IDs
        vector<Shader> vertex_shaders;          //Vertex shader IDs
        vector<Shader> fragment_shaders;        //Fragment shader IDs
        vector<Shader> tessellation_shaders;    //Tessellation shader IDs
        vector<Render_Info*> render_queue;      //Queue of objects to render 
                                                //in the current frame
    
    public:
        mutex busy_queue;           //Lock to synchronize queue W/R
        Camera *cam;                //main (player) camera object
        GLuint current_program;     //Current shading program (program used to render)

        Renderer();                 //Constructor
        ~Renderer();                //Destructor

        Shader* find_shader(string shader_name);        //Find a shader by user defiend name

        void update(GLFWwindow* window);                //Update window (swap buffers, poll events, clear)
        void add_Shader(string shader, GLuint type);    //Add a shader (glsl) to the current list
        void make_program(vector<uint> *shaders);       //Make a shading program
        void set_camera(Camera *new_cam);               //Set the main camera
        
        void multi_render(GLuint VAO, vector<GLuint> *VBOs, //Render mutliple instances of a mesh
            vector<GLuint> *buffer_types, GLuint layout_num, 
            GLuint index_num, GLuint instances);
        void change_active_program(GLuint newProgram);      //Change the current shading program
        void add_data(Render_Info*);                        //Add rendering info to the render queue
        void render();                                      //render
        void clear();                                       //clear render queue
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