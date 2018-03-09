//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       OpenGL-Wrappers.hpp
 *	@author	    Camilo Talero
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
#include "Renderer.hpp"
#include "Window-Management.hpp"
#include "cout-definitions.hpp"

using namespace std;
using namespace glm;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Structures
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class Shader;
class Shading_Program;
class Texture;
class Mesh;
class Object_3D;
class Renderer;
class Voxel_Map;
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
    Shader* vertex = NULL; 
    Shader* geometry = NULL;
    Shader* tesselation = NULL;
    Shader* fragment = NULL;
    Shader* compute = NULL;

    Shading_Program(string*, string*, string*, string*);
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

    Texture() {};
    Texture(const char* filename, GLuint target = GL_TEXTURE_2D);
    ~Texture();

    void load_to_GPU(GLuint);
    void clear();
};

class Voxel_Map : public Texture
{
public:
    Voxel_Map(float, float, float);
    void load_to_GPU(GLuint);
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
    vec4 position;        //< Global position of the light
    vec4 color;           //< Color of the light
};

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

void GL_error_callback(GLenum source, GLenum type, GLuint id,
   GLenum severity, GLsizei length, const GLchar* message, const void* userParam);