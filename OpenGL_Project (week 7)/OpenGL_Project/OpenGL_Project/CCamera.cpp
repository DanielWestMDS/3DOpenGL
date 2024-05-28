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

	// orbit values
	m_orbiting = true;
	m_automaticOrbit = false;
	m_radius = 60.0f;
	m_angle = 90.0f;
	m_lookPos = glm::vec3(0.0f, 0.0f, 0.0f);

	// set movespeed
	m_moveSpeed = 5.0f;

	// initialise stuff to stop warnings 
	m_viewMat = glm::mat4(1.0f);
	m_projMat = glm::mat4(1.0f);

	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_lookDir = glm::vec3(0.0f, 0.0f, 0.0f);
	m_upDir = glm::vec3(0.0f, 0.0f, 0.0f);

	m_UIprojMat = glm::mat4(1.0f);
	m_UIviewMat = glm::mat4(1.0f);
}

CCamera::~CCamera()
{
}

void CCamera::Update(float _currentTime, int _iWindowSize, GLFWwindow* _Window, float _dt)
{
	float HalfWindow = (float)_iWindowSize * 0.5;
	m_projMat = glm::perspective(glm::radians(45.0f), (HalfWindow * 2) / (HalfWindow * 2), 0.1f, 1000.0f);
	// orthographic
	m_UIprojMat = glm::ortho(0.0f, (float)_iWindowSize, (float)_iWindowSize, 0.0f, 0.0f, 1000.0f);

	//m_projMat = glm::perspective(glm::radians(45.0f), (HalfWindow * 2) / (HalfWindow * 2), 0.1f, 100.0f);

	m_lookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	m_upDir = glm::vec3(0.0f, 1.0f, 0.0f);
	//m_position = glm::vec3(1.0f, 1.0f, 10.0f);

	// view matrix for 3d objects
	m_viewMat = glm::lookAt(m_position, m_lookDir, m_upDir);

	// view matrix for UI button
	m_UIviewMat = glm::lookAt(m_position, m_position + m_lookDir, m_upDir);

	// update input for movement every frame
	Input(_Window, _dt);
	// WASD
	TriHoriz(_Window, _dt);
	// QE
	TriVerti(_Window, _dt);
	//m_position += (GetMove(_Window, _dt) * _dt * m_moveSpeed);
}

glm::mat4 CCamera::GetViewMat()
{
    return m_viewMat;
}

glm::mat4 CCamera::GetProjMat()
{
	return m_projMat;
}

void CCamera::Input(GLFWwindow* _Window, float _dt)
{

	// switch to orbiting for arrow keys
	if (glfwGetKey(_Window, GLFW_KEY_UP))
	{
		m_orbiting = true;
	}

	if (glfwGetKey(_Window, GLFW_KEY_DOWN))
	{
		m_orbiting = true;
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
	return ((float)(TriHoriz(_Window, _dt)) * -GetRight()) + ((float)(-TriVerti(_Window, _dt)) * GetForward());
}

signed char CCamera::TriHoriz(GLFWwindow* _Window, float _dt)
{
	signed char retVal = 0;

	// move forward with w key
	if (glfwGetKey(_Window, GLFW_KEY_W))
	{
		m_position -= GetForward() * m_moveSpeed * _dt;
	}

	// move backwards with s key
	if (glfwGetKey(_Window, GLFW_KEY_S))
	{
		m_position += GetForward() * m_moveSpeed * _dt;
	}

	// go left with A key
	if (glfwGetKey(_Window, GLFW_KEY_A))
	{
		m_position += GetRight() * _dt * m_moveSpeed;
	}

	// go right with d key
	if (glfwGetKey(_Window, GLFW_KEY_D))
	{
		m_position -= GetRight() * _dt * m_moveSpeed;
	}

	return retVal;
}

signed char CCamera::TriVerti(GLFWwindow* _Window, float _dt)
{
	signed char retVal = 0;

	// go up with q key
	if (glfwGetKey(_Window, GLFW_KEY_Q))
	{
		m_position += glm::vec3(0.0f, 1.0f, 0.0f) * _dt;
	}

	// go down with e key
	if (glfwGetKey(_Window, GLFW_KEY_E))
	{
		m_position += glm::vec3(0.0f, -1.0f, 0.0f) * _dt;
	}

	return retVal;
}
