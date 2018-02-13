//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
/**
 *  @file 		Camera.cpp
 *	@author 	Camilo Talero
 *
 *
 *	Version: 0.0.3
 *
 *	@brief Implementation of the camera header. Defines the behaviour for a generic camera.
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*	Includes and macros
*/
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "Camera.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//========================================================================================
/*
*	Camera Functions:
*/
//========================================================================================

/**
*	Parameter constructor
*/
Camera::Camera(mat3 frame, vec3 pos, float w, float h)
{
	side = normalize(frame[0]);
	forward = normalize(frame[1]);
	up = normalize(frame[2]);
	position = pos;

	orig_side = normalize(frame[0]);
	orig_forward = normalize(frame[1]);
	orig_up = normalize(frame[2]);
	orig_position = pos;

	fov = 45;
	width = w;
	height = h;
	zNear = 0.01;
	zFar = 2000;
}

/**
*	Default constructor
*/
Camera::Camera()
{
	side = vec3(1,0,0);
	forward = vec3(0,1,0);
	up = vec3(0,0,1);
	position = vec3(0,0,0);

	orig_side = vec3(1,0,0);
	orig_forward = vec3(0,1,0);
	orig_up = vec3(0,0,1);
	orig_position = vec3(0,0,0);

	width = 1980;
	height = 1024;

	fov = 45;
	zNear = 0.01;
	zFar = 2000;
}


/**
*	Destructor
*/
Camera::~Camera()
{

}

/**
* Return the global camera position
*/
vec3 Camera::getPosition()
{
	return position;
}

/**
 * Return the diriection in which teh camera is looking
 */
vec3 Camera::getForward()
{
	return forward;
}

/**
 * Return the up direction of the camera
 */
vec3 Camera::getUp()
{
	return up;	
}

/**
 * Return the side direction of the camera
 */
vec3 Camera::getSide()
{
	return side;
}

/**
 * Return the field of view of the camera 
 */
float Camera::getFov()
{
	return fov;
}

/**
 *	Get the view matrix of the camera
 */
mat4 Camera::getViewMatrix()
{
	return lookAt(position, position+forward, up);
}

/**
 *	Get the perspective matrix of the camera
 */
mat4 Camera::getPerspectiveMatrix()
{
	return perspective(fov, width/height, zNear, zFar);
}

/**
 *	Orient the camera so that it looks in the direction of \a v \a
 */
void Camera::setLookDirection(vec3 v)
{
	forward = normalize(v);
	side = cross(forward, up);
}

/**
 *	Move the camera by an offset \a v \a
 */
void Camera::move(vec3 v)
{
	position += v;
}

/**
 *	Place the camera at specified position \a p \a
 */
void Camera::setPosition(vec3 p)
{
	position = p;
}

/**
 *	Rotate the camera around it's \a up \a direction
 */
void Camera::turnH(float angle)
{
	mat4 rotation(1);
	rotation = rotate(rotation, angle, up);
	vec4 newForward = vec4(forward, 1);

	newForward = rotation*newForward;

	forward = vec3(newForward);
	forward = normalize(forward);
	side = normalize(cross(forward, up));
}

/**
 * Rotate the camera around it's \a side \a direction 
 */
void Camera::turnV(float angle)
{
	mat4 rotation;
	if(length(side)>=1)
	{
		rotation = rotate(rotation, angle, side);

		vec4 newForward = vec4(forward, 1);

		newForward = rotation*newForward;

		forward = normalize(vec3(newForward));
	}
}

//TODO: the following may need to be removed
/**
 * Rotate the camera around it's forward direction 
 */
void Camera::incline(float angle)
{
	mat4 rotation;
	rotation = rotate(rotation, angle, forward);
	vec4 newUp = vec4(up, 1);

	newUp = rotation*newUp;

	up = vec3(newUp);
	up = normalize(up);
	side = cross(forward, up);
}


/**
 * Orient the camera to it's default looking direction and orientation
 */
void Camera::resetView()
{
	side = orig_side;
	forward = orig_forward;
	up = orig_up;
}

/**
 * Hard reset all camera values to defaults
 */
void Camera::resetCamera()
{
	resetView();
	position = orig_position;

	width = 1980;
	height = 1024;

	fov = 45;
	zNear = 0.01;
	zFar = 2000;
}

//########################################################################################
