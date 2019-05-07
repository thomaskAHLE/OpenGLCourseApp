#include "Camera.h"



Camera::Camera(glm::vec3 startPosition, glm::vec3 startup, GLfloat startYaw, GLfloat startPitch, GLfloat startSpeed, GLfloat startTurnSpeed)
{
	m_position = startPosition;
	m_worldUp = startup;
	m_yaw = startYaw;
	m_pitch = startPitch;
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_movementSpeed = startSpeed;
	m_turnSpeed = startTurnSpeed;
	update();
	
}

void Camera::keyControl(bool * keys, GLfloat deltaTime)
{
	GLfloat velocity = m_movementSpeed * deltaTime;
	if (keys[GLFW_KEY_W])
	{
		m_position += m_front * velocity;
	}
	if (keys[GLFW_KEY_D])
	{
		m_position += m_right * velocity;
	}
	if (keys[GLFW_KEY_A])
	{
		m_position -= m_right * velocity;
	}
	if (keys[GLFW_KEY_S])
	{
		m_position -= m_front * velocity;
	}

	if (keys[GLFW_KEY_SPACE])
	{
		m_position += m_up * velocity;
	}

	if (keys[GLFW_KEY_LEFT_CONTROL])
	{
		m_position -= m_up * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= m_turnSpeed;
	yChange *= m_turnSpeed;
	
	m_yaw += xChange;
	m_pitch += yChange;

	if (m_pitch > PITCH_LIMIT)
	{
		m_pitch = PITCH_LIMIT;
	}

	if (m_pitch < -PITCH_LIMIT)
	{
		m_pitch = -PITCH_LIMIT;
	}
	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}



void Camera::update()
{
	m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(m_front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

Camera::~Camera()
{
}