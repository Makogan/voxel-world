//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       Renderer.hpp
 *	@author	    Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	 @brief Header defining the rendering handler class
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
#include "OpenGL-Wrappers.hpp"
#include "cout-definitions.hpp"

using namespace std;
using namespace glm;

class Shader;
class Shading_Program;
class Texture;
class Shadow_Map;
class Mesh;
class Light;
class Object_3D;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Structure definitions
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*
* General rendering manager class
*/
class Renderer
{
    private:
        vector<Shading_Program> shading_programs;        //!< Shading programs IDs
        vector<Object_3D*> render_queue;        //!< Queue of objects to render 
                                                //!< in the current frame
        vector<GLuint> FBOs;
        vector<Shadow_Map> shadow_maps;

        void draw();       
        void draw_shadow_maps(vector<Light>&);                    

        GLint inline get_uniform_location(string name);
        void inline load_uniform(mat4 matrix, string name);
        void inline load_uniform(vec3 vector, string name);
        void inline load_uniform(float num, string name);

        void setup_light_perspectives(GLuint program, Light &light);
     
    public:
        mutex busy_queue;           //!< Lock to synchronize queue W/R
        Camera *cam;                //!< Main (player) camera object
        GLuint current_program;     //!< Current shading program (program used to render)

        Renderer();                         //Default Constructor
        Renderer(int width, int height);    //Parametrized Constructor
        ~Renderer();                        //Destructor
        
        void set_camera(Camera *new_cam);               //Set the main camera
        
        void add_data(Object_3D*);                          //Add rendering info to the render queue

        void multi_render(GLuint VAO, vector<GLuint> *VBOs, //Render mutliple instances of a mesh
            vector<GLuint> *buffer_types, GLuint layout_num, 
            GLuint index_num, GLuint instances);

        void make_program(vector<uint> *shaders);           //Make a shading program
        void change_active_program(GLuint newProgram);      //Change the current shading program

        void update(GLFWwindow* window);                    //Update window (swap buffers, poll events, clear)
        void render();                                      //render
        void clear();                                       //clear render queue
};

extern Renderer *Rendering_Handler;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~