// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CCamera.cpp
// Description : Sets view and projection matrices
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#include "CCamera.h"

CCamera::CCamera()
{
	m_position = glm::vec3(1.0f, 4.0f, 10.0f);

	// set movespeed
	m_moveSpeed = 5.0f;

	// initialise stuff to stop warnings 
	m_viewMat = glm::mat4(1.0f);
	m_projMat = glm::mat4(1.0f);

	m_position = glm::vec3(-17, 5, -23);
	m_lookDir = glm::vec3(0.208, -0.223, 0.952);
	m_upDir = glm::vec3(0.0f, 1.0f, 0.0f);

	m_UIprojMat = glm::mat4(1.0f);
	m_UIviewMat = glm::mat4(1.0f);

	// mouse values
	m_lastMouse = glm::vec2(0.0f, 0.0f);
	m_mouseSpeed = 100.0f;

	// look at terrain
	m_yaw = 70.0f;
}

CCamera::~CCamera()
{
}

void CCamera::Update(int _iWindowSize, GLFWwindow* _Window, glm::vec2 _MousePos, float _dt)
{
	float  HalfWindow = (float)_iWindowSize * 0.5f;
	m_projMat = glm::perspective(glm::radians(45.0f), (HalfWindow * 2) / (HalfWindow * 2), 0.1f, 1000.0f);
	// orthographic
	m_UIprojMat = glm::ortho(0.0f, (float)_iWindowSize, (float)_iWindowSize, 0.0f, 0.0f, 1000.0f);

		// for mouse movement
	glm::vec2 mouseDelta = _MousePos - m_lastMouse;
	mouseDelta.y = -mouseDelta.y;
	m_lastMouse = _MousePos;

	// only update lookDir if left mouse pressed
	if (m_bMousePressed)
	{
		mouseDelta *= _dt;
		m_yaw += mouseDelta.x * m_mouseSpeed;
		m_pitch += mouseDelta.y * m_mouseSpeed;

		if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		}
		else if (m_pitch < -89.0f)
		{
			m_pitch = -89.0f;
		}
	}

	// change look dir based on mouse
	glm::vec3 dir;

	// float radians yaw i fink
	float fRyaw = glm::radians(m_yaw);
	float fRpitch = glm::radians(m_pitch);

	dir.x = (cos(fRyaw) * cos(fRpitch));
	dir.y = (sin(fRpitch));
	dir.z = (sin(fRyaw) * cos(fRpitch));
	m_lookDir = glm::normalize(dir);

	m_upDir = glm::normalize(glm::cross(GetRight(), GetForward()));

	// view matrix for 3d objects
	m_viewMat = glm::lookAt(m_position, m_position + m_lookDir, m_upDir);

	// view matrix for UI button
	m_UIviewMat = glm::lookAt(m_position, m_position + m_lookDir, m_upDir);

	// update input for movement every frame
	Input(_Window, _dt);
	ChangeHeight(_Window, _dt);
	m_position += (GetMove(_Window, _dt) * _dt * m_moveSpeed);
}

glm::mat4 CCamera::GetViewMat()
{
    return m_viewMat;
}

glm::mat4 CCamera::GetProjMat()
{
	return m_projMat;
}

glm::mat4 CCamera::GetVP()
{
	return m_projMat * m_viewMat;
}

void CCamera::Input(GLFWwindow* _Window, float _dt)
{
	// get mouse input
	if (glfwGetMouseButton(_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		m_bMousePressed = true;
	}
	else
	{
		m_bMousePressed = false;
	}

	// alter the camera move speed if shift is being held
	if (glfwGetKey(_Window, GLFW_KEY_LEFT_SHIFT))
	{
		m_moveSpeed = 10.0f;
	}
	else
	{
		m_moveSpeed = 5.0f;
	}
	//if (glfwGetKey(_Window, GLFW_KEY_1))
	//{
	//	m_radius += 0.1f;
	//}
}

void CCamera::PrintCamPos()
{
	std::cout << "X: " << m_position.x << "Y: " << m_position.y << "Z: " << m_position.z << std::endl;
}

glm::vec3 CCamera::GetMove(GLFWwindow* _Window, float _dt)
{
	return ((float)(TriHoriz(_Window)) * -GetRight()) + ((float)(-TriVerti(_Window)) * GetForward());
}

signed char CCamera::TriHoriz(GLFWwindow* _Window)
{
	signed char retVal = 0;

	// go left with A key
	if (glfwGetKey(_Window, GLFW_KEY_A))
	{
		retVal -= 1;
	}

	// go right with d key
	if (glfwGetKey(_Window, GLFW_KEY_D))
	{
		retVal += 1;
	}

	return retVal;
}

signed char CCamera::TriVerti(GLFWwindow* _Window)
{
	signed char retVal = 0;

	// move forward with w key
	if (glfwGetKey(_Window, GLFW_KEY_W))
	{
		retVal += 1;
	}

	// move backwards with s key
	if (glfwGetKey(_Window, GLFW_KEY_S))
	{
		retVal -= 1;
	}

	return retVal;
}

void CCamera::ChangeHeight(GLFWwindow* _Window, float _dt)
{
	// go down with e key
	if (glfwGetKey(_Window, GLFW_KEY_E))
	{
		m_position += glm::vec3(0.0f, -1.0f, 0.0f) * _dt * m_moveSpeed;
	}

	// go up with Q key
	if (glfwGetKey(_Window, GLFW_KEY_Q))
	{
		m_position += glm::vec3(0.0f, 1.0f, 0.0f) * _dt * m_moveSpeed;
	}
}
