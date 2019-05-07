#pragma once
#include <GL/glew.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startup, GLfloat startYaw, GLfloat startPitch, GLfloat startSpeed, GLfloat startTurnSpeed);
	
	void KeyControl(bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yCHange);

	glm::vec3 GetCameraPosition() const { return position;}
	glm::vec3 GetCameraDirection() const { return glm::normalize(front); }
	glm::mat4 CalculateViewMatrix();


	~Camera();
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	const GLfloat PITCH_LIMIT = 89.0f;

	void update();
};

