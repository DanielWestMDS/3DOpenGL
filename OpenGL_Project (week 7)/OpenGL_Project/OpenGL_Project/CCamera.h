#pragma once
#include "CSquare.h"

class CCamera
{
public:
	CCamera();
	~CCamera();

	void Update(float _deltaTime, int _iWindowSize, GLFWwindow* _Window, float _dt);
	glm::mat4 GetViewMat();
	glm::mat4 GetProjMat();

	void Input(GLFWwindow* _Window, float _dt);
	void CameraOrbit(GLFWwindow* _Window, float _dt);

private:
	glm::mat4 m_viewMat;
	glm::mat4 m_projMat;

	glm::vec3 m_position;
	glm::vec3 m_lookDir;
	glm::vec3 m_upDir;

	// obrital camera
	float m_radius;
	float m_angle;
	glm::vec3 m_lookPos;

	signed char TriHoriz(GLFWwindow* _Window);
};

