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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Class static fields:
*/
//========================================================================================
//TODO:Document this
string Cube::Default_Mesh = "Assets/Objs/cube.obj";
string Cube::Default_Texture = "Assets/Textures/Cube-map.png";

Texture* Cube::texture = NULL;
Geometry* Cube::mesh = NULL;
//########################################################################################

//========================================================================================
/*
*	Class implementation:
*/
//========================================================================================
//TODO: comment this section

void Cube::make_template(string wavefront_file, string t)
{
    texture = new Texture();
    mesh = new Geometry();

    createTexture(*texture, (t.c_str()), GL_TEXTURE_2D);
    createGeometry(*mesh);
 
    load_obj(wavefront_file, (vector<float>*) &mesh->vertices, 
      (vector<float>*) &mesh->normals, (vector<float>*) &mesh->uvs);
}

void Cube::cleanup()
{
    delete(texture);
    delete(mesh);
}

Cube::Cube(vec3 p)
{
    if(texture == NULL)
    {
        texture = new Texture();
        createTexture(*texture, (Default_Texture.c_str()), GL_TEXTURE_2D);
    }
    if(mesh == NULL)
    {
        mesh = new Geometry();
        createGeometry(*mesh);
    
        load_obj(Default_Mesh, (vector<float>*) &mesh->vertices, 
            (vector<float>*) &mesh->normals, (vector<float>*) &mesh->uvs);
    }

    position = p;
}

Cube::Cube() : Cube(vec3(0))
{
    
}

Cube::~Cube()
{

}

//TODO: correct this
//Warning this is innefficient, all cubes should be rendered in 1 call
void Cube::render_cube()
{
    /*loadModelMatrix(programs[0], glm::translate(mat4(1), position));
    loadGeometryArrays(programs[0], *mesh);
    loadTexture(programs[0], *texture);

    render(programs[0], *mesh, GL_TRIANGLES);*/
}
//########################################################################################