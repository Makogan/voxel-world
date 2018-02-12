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

enum PROGRAM {};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Structures
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Shader;
class Texture;
class Mesh;
class Renderer;
class Object_3D;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Structure definitions
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//TODO: verify the following data structures, maybe program shoud be a list to allow
//  for multiple programs
class Shader
{
public:
	string fileName;    //name of file
	GLuint shaderID;    //generated OpenGL shader ID
	GLuint type;        //shader type
    
    Shader();
    Shader(string file, GLenum type);
    ~Shader();

    string load_from_file(string&);
    void clear();
};

class Texture
{
public:
	GLuint textureID;   //OpenGL generated ID for the texture
    GLuint target;      //OpenGL target (Usuallly 2D texture or rectangle) check OpenGL doc

    string texture;

    //Dimensions of the texture
	int width;         
    int height;

    Texture(const char* filename, GLuint target = GL_TEXTURE_2D);
    ~Texture();

    void load_to_GPU(GLuint);
    void clear();
};

struct Mesh
{
    vector<vec3> vertices;  //Vertex data
    vector<vec3> normals;   //Normal data
	vector<uint> indices;   //Element data (sequence in which data will be read)
    vector<vec2> uvs;       //Texture data for this geometry 
                             //(the associated coordinates on the mesh)

    ~Mesh();
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
        vector<Object_3D*> render_queue;        //Queue of objects to render 
                                                //in the current frame
    
    public:
        mutex busy_queue;           //Lock to synchronize queue W/R
        Camera *cam;                //main (player) camera object
        GLuint current_program;     //Current shading program (program used to render)

        Renderer();                         //Default Constructor
        Renderer(int width, int height);    //Parametrized Constructor
        ~Renderer();                        //Destructor

        Shader* find_shader(string shader_name);        //Find a shader by user defiend name

        void update(GLFWwindow* window);                //Update window (swap buffers, poll events, clear)
        void add_Shader(string shader, GLuint type);    //Add a shader (glsl) to the current list
        void make_program(vector<uint> *shaders);       //Make a shading program
        void set_camera(Camera *new_cam);               //Set the main camera
        
        void multi_render(GLuint VAO, vector<GLuint> *VBOs, //Render mutliple instances of a mesh
            vector<GLuint> *buffer_types, GLuint layout_num, 
            GLuint index_num, GLuint instances);
        void change_active_program(GLuint newProgram);      //Change the current shading program
        void add_data(Object_3D*);                        //Add rendering info to the render queue
        void render();                                      //render
        void clear();                                       //clear render queue
};

extern Renderer *Rendering_Handler;

class Object_3D
{
public:
    GLuint VAO;             //Vertex Array Object
    vector<GLuint> VBOs;    //array of VBO Ids
    vector<GLuint> types;   //Array of VBO types 
    uint layouts;           //The number of layouts to activate
    uint render_instances;  //Number of instances to render current object
    uint mesh_indices;      //Indices for index rendering, if any

    Object_3D(Mesh*);
    /*~Object_3D();*/

    template <class T>
    void set_instance_data(Renderer*, vector<T>);
};

template <class T>
void Object_3D::set_instance_data(Renderer* handler, vector<T> info)
{
	handler->busy_queue.lock();
    glBindVertexArray(VAO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, (VBOs[3]));
    glBufferData(GL_SHADER_STORAGE_BUFFER, info.size()*sizeof(T), 
		info.data(), GL_DYNAMIC_COPY);
    handler->busy_queue.unlock();
    
    render_instances = info.size();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	List of function headers:
*/
//========================================================================================

//Check for OpenGL errors
int openGLerror();

//########################################################################################