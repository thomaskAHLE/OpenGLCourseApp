#pragma once
#include <GL/glew.h>
#include<glm/glm.hpp>
#include <GLFW/glfw3.h>
#include<glm/gtc/matrix_transform.hpp>



class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startup, GLfloat startYaw, GLfloat startPitch, GLfloat startSpeed, GLfloat startTurnSpeed);
	
	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yCHange);

	glm::vec3 getCameraPosition() const { return m_position;}
	glm::vec3 getCameraDirection() const { return glm::normalize(m_front); }
	glm::mat4 calculateViewMatrix();


	~Camera();
private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	GLfloat m_yaw;
	GLfloat m_pitch;

	GLfloat m_movementSpeed;
	GLfloat m_turnSpeed;

	const GLfloat PITCH_LIMIT = 89.0f;

	void update();
};

