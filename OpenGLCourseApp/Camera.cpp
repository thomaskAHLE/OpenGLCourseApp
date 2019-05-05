#include "Camera.h"



Camera::Camera(glm::vec3 startPosition, glm::vec3 startup, GLfloat startYaw, GLfloat startPitch, GLfloat startSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startup;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	movementSpeed = startSpeed;
	turnSpeed = startTurnSpeed;
	update();
	
}

void Camera::KeyControl(bool * keys, GLfloat deltaTime)
{
	GLfloat velocity = movementSpeed * deltaTime;
	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_SPACE])
	{
		position += up * velocity;
	}

	if (keys[GLFW_KEY_LEFT_CONTROL])
	{
		position -= up * velocity;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;
	
	yaw += xChange;
	pitch += yChange;

	if (pitch > PITCH_LIMIT)
	{
		pitch = PITCH_LIMIT;
	}

	if (pitch < -PITCH_LIMIT)
	{
		pitch = -PITCH_LIMIT;
	}
	update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}



void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera()
{
}