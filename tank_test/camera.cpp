#include "camera.h"
#include <iostream>
#include <algorithm>

Camera::Camera(glm::vec3 focal, glm::vec3 up, float yaw, float pitch) :
	worldUp(up), yaw(yaw), pitch(pitch), front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), scrollSensitivity(SCROLL_SENSITIVITY), fov(FOV), focal(focal)
{
	position = focal + glm::vec3(0.0f, 0.0f, radius);
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
	worldUp(glm::vec3(upX, upY, upZ)), yaw(yaw), pitch(pitch), front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), scrollSensitivity(SCROLL_SENSITIVITY), fov(FOV), focal(focal)
{
	position = glm::vec3(posX, posY, posZ) + glm::vec3(0.0f, 0.0f, radius);
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::updatePosition(Camera_Movement direction, float speedAdjust, glm::vec3 objFront) {
	float velocity = movementSpeed * speedAdjust;
	if (direction == FORWARD) focal = focal + objFront * velocity;
	if (direction == BACKWARD) focal = focal - objFront * velocity;

	updateCameraVectors();
	updateCameraOrbit();
	//if (direction == LEFT) focal -= right * velocity;
	//if (direction == RIGHT) focal += right * velocity;
	//if (direction == UP) focal += worldUp * velocity;
	//if (direction == DOWN) focal -= worldUp * velocity;
}

void Camera::updateDirection(float xoffset, float yoffset, GLboolean constrainPitch) {
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch) {
		if (pitch > 89.0f) pitch = 89.0f;
		else if (pitch < -89.0f) pitch = -89.0f;
	}

	updateCameraVectors();
	updateCameraOrbit();
}

void Camera::updateFov(float yoffset) {
	fov -= scrollSensitivity * yoffset;
	if (fov < 1.0f) fov = 1.0f;
	else if (fov > 45.0f) fov = 45.0f;
}

void Camera::updateCameraVectors() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::updateCameraOrbit() {
	position = focal + glm::vec3(
		radius * cos(glm::radians(yaw + 180)) * cos(glm::radians(-pitch)),
		radius * sin(glm::radians(-pitch)),
		radius * sin(glm::radians(yaw + 180)) * cos(glm::radians(-pitch))
	);

	float limitFactor = std::max(
		(abs(position.x) - 10) / abs(cos(glm::radians(yaw + 180)) * cos(glm::radians(-pitch))),
		std::max(
			-position.y / abs(sin(glm::radians(-pitch))),
			(abs(position.z) - 10) / abs(sin(glm::radians(yaw + 180)) * cos(glm::radians(-pitch)))
		)
	);

	if (limitFactor > 0) {
		position -= glm::vec3(
			limitFactor * cos(glm::radians(yaw + 180)) * cos(glm::radians(-pitch)),
			limitFactor * sin(glm::radians(-pitch)),
			limitFactor * sin(glm::radians(yaw + 180)) * cos(glm::radians(-pitch))
		);
	}
}