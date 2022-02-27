#include "Camera.h"



//Constructor
Camera::Camera(glm::vec3 position) {
	//Sets position then updates view angles
	Position = position;
	updateCameraVectors();
}


//Camera functions for input and changing vectors
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
	//Modifies the deltatime by speed, (deltaTime is also increased before being sent to this function)
	float velocity = speed * deltaTime;

	//Dependent on direction to move, multiplies speed by proper vector then adds or subtracts from position
	if (direction == Camera_Movement::FORWARD) {
		Position += Front * velocity;
	}
	if (direction == Camera_Movement::BACKWARD) {
		Position -= Front * velocity;
	}	
	if (direction == Camera_Movement::LEFT) {
		Position -= Right * velocity;
	}
	if (direction == Camera_Movement::RIGHT) {
		Position += Right * velocity;
	}

	//Special case as in UP/DOWN only z value is changed
	if (direction == Camera_Movement::UP) {
		Position.y += velocity;
	}
	if (direction == Camera_Movement::DOWN) {
		Position.y -= velocity;
	}	
}

void Camera::ProcessMouseInput(float xOffset, float yOffset, GLboolean constraintPitch) {

	//multiplies mouse offset by sensitivity
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	//Adds the offset to pitch and yaw appropriately
	yaw += xOffset;
	pitch += yOffset;

	//Constraints pitch to < 90 degrees (UP?DOWN)
	if (constraintPitch) {
		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;
		}
	}
	//Updates Camera vectors
	updateCameraVectors();
}

//Not needed due to scroll wheel controlling speed and not zoom
//void Camera::ProcessScroll(float yOffset) {
//	zoom -= (float)yOffset;
//	if (zoom > 45.0f) {
//		zoom = 45.0f;
//	}
//	if (zoom < 1.0f) {
//		zoom = 1.0f;
//	}
//
//}

void Camera::updateCameraVectors() {

	//Declares a vec3 then assigns x,y, and z based off Euler's equations
	//For yaw, pitch, and roll
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	//Assigns a normalized version of created vector to Front vector;
	Front = glm::normalize(front);

	//Sets the appropriate Up and Right vectors as normalized crossproducts of appropriate vectors
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}