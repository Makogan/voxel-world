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
		//current camera frame
		vec3 forward; 	//vector indicating forward direction of the camera
		vec3 up;  		//vector indicating up direction of the camera
		vec3 side; 		//vector indicating side direction of the camera
		vec3 position; 	//vector indicating the position of the camera

		//ooriginal camera frame
		vec3 orig_forward; 	//vector indicating forward direction of the camera
		vec3 orig_up;  		//vector indicating up direction of the camera
		vec3 orig_side; 		//vector indicating side direction of the camera
		vec3 orig_position; 	//vector indicating the position of the camera

		//camera parameters
		//field of view, dimensions, near clipping angle, far clipping angle
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
		vec3 getForward();
		vec3 getUp();
		vec3 getSide();

};
//########################################################################################
