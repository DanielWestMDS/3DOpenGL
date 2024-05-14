#include "CCamera.h"

CCamera::CCamera()
{
	m_position = glm::vec3(1.0f, 1.0f, 10.0f);

	// orbit values
	m_radius = 60.0f;
	m_angle = 90.0f;
	m_lookPos = glm::vec3(0.0f, 0.0f, 0.0f);
}

CCamera::~CCamera()
{
}

void CCamera::Update(float _currentTime, int _iWindowSize, GLFWwindow* _Window, float _dt)
{
	float HalfWindow = (float)_iWindowSize * 0.5;
	m_projMat = glm::perspective(glm::radians(45.0f), (HalfWindow * 2) / (HalfWindow * 2), 0.1f, 1000.0f);

	//m_projMat = glm::perspective(glm::radians(45.0f), (HalfWindow * 2) / (HalfWindow * 2), 0.1f, 100.0f);

	m_lookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	m_upDir = glm::vec3(0.0f, 1.0f, 0.0f);
	//m_position = glm::vec3(1.0f, 1.0f, 10.0f);

	m_viewMat = glm::lookAt(m_position, m_position + m_lookDir, m_upDir);

	// update input for movement every frame
	Input(_Window, _dt);

	// orbit around a point
	CameraOrbit(_Window, _dt);
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
	if (glfwGetKey(_Window, GLFW_KEY_W))
	{
		m_position += glm::vec3(0.0f, 1.0f, 0.0f) * _dt;
	}

	if (glfwGetKey(_Window, GLFW_KEY_S))
	{
		m_position += glm::vec3(0.0f, -1.0f, 0.0f) * _dt;
	}

	if (glfwGetKey(_Window, GLFW_KEY_A))
	{
		m_position += glm::vec3(-1.0f, 0.0f, 0.0f) * _dt;
	}

	if (glfwGetKey(_Window, GLFW_KEY_D))
	{
		m_position += glm::vec3(1.0f, 0.0f, 0.0f) * _dt;
	}

	if (glfwGetKey(_Window, GLFW_KEY_1))
	{
		m_radius += 0.1f;
	}
}

void CCamera::CameraOrbit(GLFWwindow* _Window, float _dt)
{
	m_angle += TriHoriz(_Window) * _dt;

	m_position = glm::vec3(m_radius * cosf(m_angle), 2, m_radius * sinf(m_angle));

	m_position += m_lookPos;
}

signed char CCamera::TriHoriz(GLFWwindow* _Window)
{
	signed char retVal = 0;

	if (glfwGetKey(_Window, GLFW_KEY_UP))
	{
		retVal += 1;
	}

	if (glfwGetKey(_Window, GLFW_KEY_DOWN))
	{
		retVal -= 1;
	}

	return retVal;
}
