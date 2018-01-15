//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.2
*
*	Header for the definition of a generic cube object
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

#define MESH Cube::meshes[cube_type]
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Class static fields:
*/
//========================================================================================
//Default texture for a cube object
string Cube::Default_Texture = "Assets/Textures/white_cube.png";

//Global arrays for the cube class, used to store mesh and texture information of future
//cube classes that inherit this class
vector<Mesh*> Cube::meshes = vector<Mesh*>(10);
vector<Texture*> Cube::textures = vector<Texture*>(10);
//########################################################################################

//========================================================================================
/*
*	Class implementation:
*/
//========================================================================================

/*
*   Free memory used by the cube class
*/
void Cube::cleanup()
{
    for(Texture * t: Cube::textures)
       if(t!=NULL) {DestroyTexture(*t);delete(t);}
    for(Mesh * m: Cube::meshes)
        if(m!=NULL) {delete(m);}
}

/*
*   Parametrized constructor of the cube class
*   Type refers to a cube ID 
*/
Cube::Cube(vec3 p, uint type)
{
    //Create class texture if needed
    if(textures[cube_type] == NULL)
    {
        textures[cube_type] = new Texture();
        createTexture(*(textures[cube_type]), (Default_Texture.c_str()), GL_TEXTURE_2D);
    }
    //Create rendring information of the class
    if(meshes[cube_type] == NULL)
    {
        meshes[cube_type] = new Mesh();

        MESH->vertices = new vector<vec3>();
        MESH->normals = new vector<vec3>();
        MESH->uvs = new vector<vec2>();
        MESH->indices = new vector<uint>();

        load_obj("Assets/Objs/cube.obj", (vector<float>*)MESH->vertices, 
        (vector<float>*)MESH->normals, (vector<float>*)MESH->uvs);

        MESH->indices = new vector<uint>();

        for(uint i=0; i<MESH->vertices->size(); i++)
            MESH->indices->push_back(i);
    }
    //set world offset
    position = p;
}

//List initializers
Cube::Cube(vec3 p) : Cube(p,0){}

Cube::Cube() : Cube(vec3(0)){}

Cube::~Cube(){}

void Cube::update(vec3 offset)
{
    transparent = false;
    position = offset;
}
//########################################################################################