//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.1
*
*	Header for the definition of a generic cube object
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <glm/gtc/matrix_transform.hpp>

#include "Cube.hpp"
#include "Rendering.hpp"
#include "cout-definitions.hpp"

#define MESH Cube::meshes[cube_type]
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Class static fields:
*/
//========================================================================================
//TODO:Document this
string Cube::Default_Texture = "Assets/Textures/Face_Orientation.png";

vector<Mesh*> Cube::meshes = vector<Mesh*>(10);
vector<Texture*> Cube::textures = vector<Texture*>(10);
//########################################################################################

//========================================================================================
/*
*	Class implementation:
*/
//========================================================================================
//TODO: comment this section

void Cube::cleanup()
{
    for(Texture * t: Cube::textures)
        delete(t);
    for(Mesh * m: Cube::meshes)
        delete(m);
}

Cube::Cube(vec3 p, uint type)
{
    if(textures[cube_type] == NULL)
    {
        textures[cube_type] = new Texture();
        createTexture(*(textures[cube_type]), (Default_Texture.c_str()), GL_TEXTURE_2D);
    }
    if(meshes[cube_type] == NULL)
    {
        meshes[cube_type] = new Mesh();

        MESH->vertices = new vector<vec3>({vec3(-0.5,0.5,-0.5), vec3(-0.5,0.5,0.5), vec3(0.5,0.5,0.5), vec3(0.5,0.5,-0.5)});
        MESH->normals = new vector<vec3>({vec3(0,1,0),vec3(0,1,0),vec3(0,1,0),vec3(0,1,0)});
        MESH->uvs = new vector<vec2>({vec2(0,0), vec2(0,1), vec2(1/6.f,1), vec2(1/6.f,0)});
        MESH->indices = new vector<uint>({0,1,2,2,3,0});
    }
    position = p;
}

Cube::Cube(vec3 p) : Cube(p,0){}

Cube::Cube() : Cube(vec3(0)){}

Cube::~Cube()
{

}
//########################################################################################