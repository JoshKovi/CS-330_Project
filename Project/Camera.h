#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


class Camera
{
public:

	//Used to determine movement direction, makes code more readable
	enum class Camera_Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	//Default camera variables, later used to hold current values
	float yaw = 90.0f; //Left and Right Angle
	float pitch = 0.0f; //Up and Down Angle
	float speed = 2.5f; //Speed Constant for Camera movement
	float sensitivity = 0.1f; //Sensitivity is used for camera sensitivity to movement
	float zoom = 45.0f; //Used for Field of view

	//Global Camera Variables aka attributes
	glm::vec3 Position; //Assigned in constructor
	glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f); //Does not need to be in constructor at current stage
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f); //Does not need to be in constructor at current stage
	glm::vec3 Right; //Defined when camera vectors are updated
	glm::vec3 WorldUp = Up; //Initially same as up, updated in camera vectors update function

	//Constructor
	Camera(glm::vec3 position);

	//Returns view vector
	glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Front, Up);}; //Defined in header as it is a 1 line function

	//Camera functions for input and changing vectors
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseInput(float xOffset, float yOffset, GLboolean constraintPitch = true);
	
	//Not Used as speed is used to control speed not zoom per rubric
	//void ProcessScroll(float yOffset);

private:
	//Updates vectors relating to angle of view/position
	void updateCameraVectors();

};

#endif // !CAMERA_H
