//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file       Cube.cpp
 *	@author 	Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief Definition of a generic cube object
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "system-libraries.hpp"

#include "Cube.hpp"
#include "cout-definitions.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Class static fields:
*/
//========================================================================================
//Default texture for a cube object
//string Cube::Default_Texture = "Assets/Textures/white_cube.png";

//Global arrays for the cube class, used to store mesh and texture information of future
//cube classes that inherit this class
vector<Mesh*> Cube::meshes;
vector<Texture*> Cube::textures;
//########################################################################################

//========================================================================================
/*
*	Class implementation:
*/
//========================================================================================

//!
//! Global texture and mesh source file strings
//! 
vector<string> texture_source_files = {"Assets/Textures/white_cube.png"};
vector<string> obj_source_files = {"Assets/Objs/cube.obj"};

/**
 * Initialize the static memebrs of the Cube class
 */ 
void Cube::initialize()
{
    for(uint i=0; i<cube_types; i++)
    {
        //Create a new texture from file
        textures.push_back(new Texture(texture_source_files[i].c_str(), GL_TEXTURE_2D));

        //Create a new empty mesh
        meshes.push_back(new Mesh());

        //load mesh data from file
        load_obj("Assets/Objs/cube.obj", (vector<float>*)&meshes[i]->vertices, 
        (vector<float>*)&meshes[i]->normals, (vector<float>*)&meshes[i]->uvs);

        //Initialize mesh indices data
        meshes[i]->indices = vector<uint>();
        for(uint j=0; j<meshes[i]->vertices.size(); j++)
            meshes[i]->indices.push_back(j);
    }
    //TODO: turn this into a list of textures
    //Load texture information to GPU (currently single, change to array)
    textures[0]->load_to_GPU(Rendering_Handler->current_program);
}

/**
 *   Free memory used by the cube class
 */
void Cube::cleanup()
{
    //clear texture and mesh data
    for(Texture * t: Cube::textures)
       if(t!=NULL) {t->clear(); delete(t);}
    for(Mesh * m: Cube::meshes)
        if(m!=NULL) {delete(m);}
}

/**
 *   Parametrized constructor of the cube class
 *   Type refers to a cube ID 
 */
Cube::Cube(vec3 p, CubeID type)
{
    cube_type = type;

    //set world offset
    position = p;
}

//List initializers

/**
 * Constructor for a cube, creates a default cube at position \a p \a
 */ 
Cube::Cube(vec3 p) : Cube(p,DEFAULT){}

/**
 * Default constructor for a cube
 */
Cube::Cube() : Cube(vec3(0)){}

/**
 * Destructor for a cube
 */ 
Cube::~Cube(){}

/**
 * Update the position of the current cube and reset its values
 */ 
void Cube::update(vec3 offset)
{
    transparent = false;
    position = offset;
}

/**
 * Get the value of the mesh of this cube
 */ 
Mesh Cube::getMesh()
{
    return *meshes[cube_type];
}
//########################################################################################