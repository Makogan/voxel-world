//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       OpenGL-Wrappers.hpp
 *	@author	Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	 @brief Header to define variables, structure definitoins, include libraries... 
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
	string fileName;    //!< source file
	GLuint shaderID;    //!< generated OpenGL shader ID
	GLuint type;        //!< shader type
    
    Shader();
    Shader(string file, GLenum type);
    ~Shader();

    string load_from_file(string&);
    void clear();
};

//TODO: implement this
class Shading_Program
{
public:
    GLuint programID;
    Shader* vertex; 
    Shader* geometry;
    Shader* tesselation;
    Shader* fragment;
    Shader* compute;

    Shading_Program();
};

class Texture
{
public:
	GLuint textureID;   //!< OpenGL generated ID for the texture
    GLuint target;      //!< OpenGL target (Usuallly 2D texture or rectangle) check OpenGL doc

    string texture;     //!< Texture data

    //Dimensions of the texture
    int width;         //!< width of the texture
    int height;        //!< height of the texture

    Texture(const char* filename, GLuint target = GL_TEXTURE_2D);
    ~Texture();

    void load_to_GPU(GLuint);
    void clear();
};

struct Mesh
{
    vector<vec3> vertices;  //!< Vertex data
    vector<vec3> normals;   //!< Normal data
	vector<uint> indices;   //!< Element data (sequence in which data will be read)
    vector<vec2> uvs;       //!< Texture data for this geometry 
                            //!(the associated coordinates of the mesh)

    ~Mesh();
};

struct Light
{
  vec3 position;        //< Global position of the light
  vec4 color;           //< Color of the light
  double intensity;     //< Intensity of the light
};

/*
* General rendering manager class
*/
class Renderer
{
    private:
        vector<GLuint> shading_programs;        //!< Shading programs IDs
        vector<Shader> vertex_shaders;          //!< Vertex shader IDs
        vector<Shader> fragment_shaders;        //!< Fragment shader IDs
        vector<Shader> geometry_shaders;
        vector<Shader> tessellation_shaders;    //!< Tessellation shader IDs
        vector<Object_3D*> render_queue;        //!< Queue of objects to render 
                                                //!< in the current frame
                                
     
    public:
        mutex busy_queue;           //!< Lock to synchronize queue W/R
        Camera *cam;                //!< Main (player) camera object
        GLuint current_program;     //!< Current shading program (program used to render)

        Renderer();                         //Default Constructor
        Renderer(int width, int height);    //Parametrized Constructor
        ~Renderer();                        //Destructor
        
        void set_camera(Camera *new_cam);               //Set the main camera
        
        void add_Shader(string shader, GLuint type);        //Add a shader (glsl) to the current list
        void add_data(Object_3D*);                          //Add rendering info to the render queue

        void create_shadow_map();
        void render_shadow_map();

        void multi_render(GLuint VAO, vector<GLuint> *VBOs, //Render mutliple instances of a mesh
            vector<GLuint> *buffer_types, GLuint layout_num, 
            GLuint index_num, GLuint instances);

        void make_program(vector<uint> *shaders);           //Make a shading program
        void change_active_program(GLuint newProgram);      //Change the current shading program
        
        Shader* find_shader(string shader_name);            //Find a shader by user defiend name

        void update(GLFWwindow* window);                    //Update window (swap buffers, poll events, clear)
        void render();                                      //render
        void clear();                                       //clear render queue
};

extern Renderer *Rendering_Handler;

class Object_3D
{
public:
    GLuint VAO;             //!< Vertex Array Object
    vector<GLuint> VBOs;    //!< array of VBO Ids
    vector<GLuint> types;   //!< Array of VBO types 
    uint layouts;           //!< The number of layouts to activate
    uint render_instances;  //!< Number of instances to render current object
    uint mesh_indices;      //!< Indices for index rendering, if any

    Object_3D(Mesh*);
    /*~Object_3D();*/

    template <class T>
    void set_instance_data(Renderer*, vector<T>);
};

/**
 * Set the visual data for the current 3D object (SSBO data)
 */ 
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