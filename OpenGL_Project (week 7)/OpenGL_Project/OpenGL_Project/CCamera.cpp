#include "CCamera.h"

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

void CCamera::Update(float _currentTime, int _iWindowSize)
{
	float HalfWindow = (float)_iWindowSize * 0.5;
	m_projMat = glm::perspective(glm::radians(45.0f), (HalfWindow * 2) / (HalfWindow * 2), 0.1f, 100.0f);

	//m_projMat = glm::perspective(glm::radians(45.0f), (HalfWindow * 2) / (HalfWindow * 2), 0.1f, 100.0f);

	m_lookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	m_upDir = glm::vec3(0.0f, 1.0f, 0.0f);
	m_position = glm::vec3(1.0f, 1.0f, 10.0f);

	m_viewMat = glm::lookAt(m_position, m_position + m_lookDir, m_upDir);
}

glm::mat4 CCamera::GetViewMat()
{
    return m_viewMat;
}

glm::mat4 CCamera::GetProjMat()
{
	return m_projMat;
}
