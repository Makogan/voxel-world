//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.1
*
*	File to define methods that abstract and simplify the use of opengl for rendering.
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "Rendering.hpp"

#include "Cube.hpp" //TODO: this needs a major refactoring
#include "Chunk.hpp" //TODO: this needs a major refactoring
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Rendering Functions:
*/
//========================================================================================

//TODO: look into making other functions that also load geometry info into the shaders
//TODO: comment undocumented functions
void loadTexture(GLuint program, Texture &t)
{
	glUseProgram(program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t.textureID);
	GLint loc = glGetUniformLocation(program, "text");
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{
		cerr << "Error returned when trying to find texture uniform."
			<< "\nuniform: text"
			<< "Error num: " << loc
			<< endl;
		return;
	}
	
	glUniform1i(loc,0);
}

/*
*	Function to load geometry information into an OpenGL shader program
*
*	Params:
*		program: the ID of the shader program into which we will looad the Geometry info
*		g:	the Geometry structure containing the information to load into teh program
*/
void loadGeometryArrays(GLuint program, Geometry &g)
{
	glUseProgram(program);//Make program the current shading program

	glBindVertexArray(g.vertexArray);//bind g's vertex array to be OpenGL's vertex array

	glBindBuffer(GL_ARRAY_BUFFER, g.vertexBuffer);//set OpenGL's vertex buffer (vertex data)
	//Set the buffer data with, type, size of buffer in bytes, array pointer, drawing mode
	glBufferData(GL_ARRAY_BUFFER, g.vertices.size()*sizeof(vec3), 
		g.vertices.data(), GL_DYNAMIC_DRAW);

	//Wich layout in the shader to pass current information (layout 0 is vertex for us)
	glEnableVertexAttribArray(0);
	//Specify how to read the data: layout index, components per vertex attribute, 
	//Type of the data, normalization, byte offset between consecutive elements, 
	//offset of first component
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);

	//Check if there are normals to process for the geometry object
	if(g.normals.size()>0)
	{
		//Set attribute layout to 1 (layout 1 is normals for us)
		glEnableVertexAttribArray(1);
		//Set the buffer array
		glBindBuffer(GL_ARRAY_BUFFER, g.normalsBuffer);
		//As above specify how to read the information
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vec3), (void*)0);
		//Fill the buffer array with the data 
		glBufferData(GL_ARRAY_BUFFER, g.normals.size()*sizeof(vec3),
			g.normals.data(), GL_DYNAMIC_DRAW);
	}

	//Check if there are texture coordinates to process for the geometry object
	if(g.uvs.size()>0)
	{
		//Set attribute layout to 2 (layout 2 is uvs for us)
		glEnableVertexAttribArray(2);
		//Set the buffer array
		glBindBuffer(GL_ARRAY_BUFFER, g.uvBuffer);
		//As above specify how to read the information
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(vec2), (void*)0);
		//Fill the buffer array with the data 
		glBufferData(GL_ARRAY_BUFFER, g.uvs.size()*sizeof(vec2),
			g.uvs.data(), GL_DYNAMIC_DRAW);
	}

	//Check if there are any indices specified in g
	if(g.indices.size()>0)
	{
		//Set the element buffer (non linear reading/loading of data information)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.elmentBuffer);
		//Indicate how to read the element data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, g.indices.size()*sizeof(uint),
			g.indices.data(), GL_DYNAMIC_DRAW);
	}
}

/*
*	Fundamental rendering function, it assumes initialization of all global values 
*	used to organize the process of sending information to the GPU for rendering
*
*	Params:
*		program: OpenGL shading program used to render 
*		g: Geometry object containing mesh information
*		drawType: the drawing mode (Lines, triangles, points, strips...)
*/
void render(GLuint program, Geometry &g, GLenum drawType)
{
	//Set the OpenGL shading program
	glUseProgram(program);

	//Bind the VAO of the geometry
	glBindVertexArray(g.vertexArray);

	//Chose appropriate drawing method based on whether there are indices in the
	//geometry strucutre has explicit indices
	if(g.indices.size()>0)
		//non sequential vertex drawing
		glDrawElements(drawType, g.indices.size(), GL_UNSIGNED_INT, (void*)0);

	else
		//sequential vertex drawing
		glDrawArrays(drawType, 0, g.vertices.size());
}

/*
*	Load the color uniform for the fragment shader (Assumes existence of a color uniform)
*
*	Params:
*		color: a representation of the RGBA values of a color
*		program: the OpenGL shading program to use
*/
void loadColor(vec4 color, GLuint program)
{
	glUseProgram(program); 	//Active program
	GLint loc = glGetUniformLocation(program, "color");
	glUniform4f(loc, color[0], color[1], color[2], color[3]);
}

/*
*	Load camera parameters into a shading program for perspective projection
*	
*	Params:
*		c: the camera struct used to create the projection matrix
*		program: program in wich to load the projection matrix
*	
*	return: 0 on error or 1 on success
*/
int loadViewProjMatrix(Camera &c, GLuint &program)
{
	glUseProgram(program);
	GLint loc = glGetUniformLocation(program, "view");
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{
		cerr << "Error returned when trying to find view matrix."
			<< "\nuniform: view"
			<< "Error num: " << loc
			<< endl;
		return 0;
	}
	//Pass the calculated view matrix onto the shader
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(c.getViewMatrix()));

	loc = glGetUniformLocation(program, "proj");
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{

		cerr << "Error returned when trying to find projection matrix."
			<< "\nuniform: proj"
			<< "Error num: " << loc
			<< endl;
		return 0;
	}
	//Pass the calculated projection/perspective matrix onto the shader
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(c.getPerspectiveMatrix()));

	return 1;
}

int loadModelMatrix(GLuint program, mat4 model)
{
	glUseProgram(program);
	GLint loc = glGetUniformLocation(program, "model");
	if(loc == GL_INVALID_VALUE || loc==GL_INVALID_OPERATION)
	{
		cerr << "Error returned when trying to find model matrix."
			<< "\nuniform: model"
			<< "Error num: " << loc
			<< endl;
		return 0;
	}
	//Load model matrix
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(model));

	return 1;
}

/*
* Load camera position into the current shader program
*
*	Params:
*		c: the camera struct used to create teh projection matrix
*		program: program in wich to load the projection matrix
*	
*	return: 0 on error or 1 on success
*/
int loadCamera(vec3 cameraPos, GLuint program)
{
	glUseProgram(program);
	GLint loc = glGetUniformLocation(program, "cameraPos");
	if (loc == -1)
	{
		cerr << "Uniform: \"cameraPos\" not found." << endl;
		return 0;
	}
	glUniform3f(loc, cameraPos[0], cameraPos[1], cameraPos[2]);

	return 1;
}
//########################################################################################