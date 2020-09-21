#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const float YAW					= -90.0f;
const float PITCH				= 0.0f;

const float SPEED				= 5.0f;
const float SENSITIVITY			= 0.1f;
const float SCROLL_SENSITIVITY	= 1.0f;
const float FOV					= 45.0f;

const float RADIUS				= 7.5f;

class Camera {
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	glm::vec3 focal;

	float radius = RADIUS;

	float yaw;
	float pitch;
	
	float movementSpeed;
	float mouseSensitivity;
	float scrollSensitivity;
	float fov;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	glm::mat4 getViewMatrix();

	void updatePosition(Camera_Movement direction, float deltaTime, glm::vec3 objFront);
	void updateDirection(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void updateFov(float yoffset);

private:
	void updateCameraVectors();
	void updateCameraOrbit();
};

#endif