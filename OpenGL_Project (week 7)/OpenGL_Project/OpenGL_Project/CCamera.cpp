#include "CCamera.h"

CCamera::CCamera()
{
	m_position = glm::vec3(1.0f, 1.0f, 10.0f);

	// orbit values
	m_orbiting = false;
	m_automaticOrbit = false;
	m_radius = 60.0f;
	m_angle = 90.0f;
	m_lookPos = glm::vec3(0.0f, 0.0f, 0.0f);

	// set movespeed
	m_moveSpeed = 5.0f;
}

CCamera::~CCamera()
{
}

void CCamera::Update(float _currentTime, int _iWindowSize, GLFWwindow* _Window, float _dt)
{
	float HalfWindow = (float)_iWindowSize * 0.5;
	m_projMat = glm::perspective(glm::radians(45.0f), (HalfWindow * 2) / (HalfWindow * 2), 0.1f, 1000.0f);
	m_UIprojMat = glm::ortho(0.0f, (float)_iWindowSize, (float)_iWindowSize, 0.0f, 0.1f, 100.0f);

	//m_projMat = glm::perspective(glm::radians(45.0f), (HalfWindow * 2) / (HalfWindow * 2), 0.1f, 100.0f);

	m_lookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	m_upDir = glm::vec3(0.0f, 1.0f, 0.0f);
	//m_position = glm::vec3(1.0f, 1.0f, 10.0f);

	m_viewMat = glm::lookAt(m_position, m_lookPos + m_lookDir, m_upDir);

	// update input for movement every frame
	Input(_Window, _dt);

	// orbit around a point
	if (m_orbiting)
	{
		CameraOrbit(_Window, _dt);
	}
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

void CCamera::CameraOrbit(GLFWwindow* _Window, float _dt)
{
	if (m_automaticOrbit)
	{
		m_angle += m_moveSpeed * _dt;
	}
	else
	{
		m_angle += TriHoriz(_Window) * m_moveSpeed * _dt;
	}

	m_position = glm::vec3(m_radius * cosf(m_angle), 2, m_radius * sinf(m_angle));

	m_position += m_lookPos;
}

void CCamera::PrintCamPos()
{
	std::cout << "X: " << m_position.x << "Y: " << m_position.y << "Z: " << m_position.z << std::endl;
}

void CCamera::SetAutoCircle()
{
	m_automaticOrbit = !m_automaticOrbit;
}


signed char CCamera::TriHoriz(GLFWwindow* _Window)
{
	signed char retVal = 0;

	// zoom in with up key
	if (glfwGetKey(_Window, GLFW_KEY_UP))
	{
		m_radius -= 1;
	}

	// zoom out with down key
	if (glfwGetKey(_Window, GLFW_KEY_DOWN))
	{
		m_radius += 1;
	}

	// go left with left key
	if (glfwGetKey(_Window, GLFW_KEY_LEFT))
	{
		retVal += 1;
	}

	// go right with right key
	if (glfwGetKey(_Window, GLFW_KEY_RIGHT))
	{
		retVal -= 1;
	}

	return retVal;
}
