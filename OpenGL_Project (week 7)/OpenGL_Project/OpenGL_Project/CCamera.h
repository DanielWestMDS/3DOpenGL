#pragma once
#include "CSquare.h"

class CCamera
{
public:
	CCamera();
	~CCamera();

	void Update(float _deltaTime, int _iWindowSize);
	glm::mat4 GetViewMat();
	glm::mat4 GetProjMat();

private:
	glm::mat4 m_viewMat;
	glm::mat4 m_projMat;

	glm::vec3 m_position;
	glm::vec3 m_lookDir;
	glm::vec3 m_upDir;
};

