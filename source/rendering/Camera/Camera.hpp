//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/*
*	Author:	Camilo Talero
*
*
*	Version: 0.0.1
*
*	Header declaration of functions and memebers for a generic camera class
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Camera Function and Members Declarations:
*/
//========================================================================================
class Camera
{
	private:
		vec3 forward;
		vec3 up;
		vec3 side;
		vec3 position;
		float fov, width, height, zNear, zFar;

	public:
		Camera (mat3 frame, vec3 pos, float w, float h);
		Camera();
		~Camera();
		mat4 getViewMatrix();
		mat4 getPerspectiveMatrix();
		void setLookDirection(vec3 v);
		void move(vec3 v);
		void setPosition(vec3 p);
		void turnH(float angle);
		void turnV(float angle);
		void incline(float angle);
		void resetView();
		void resetCamera();
		vec3 getPosition();

};
//########################################################################################
