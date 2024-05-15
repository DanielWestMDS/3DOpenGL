#pragma once
#include "CCube.h"
#include <iostream>

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
	void PrintCamPos();
	void SetAutoCircle();

private:
	glm::mat4 m_viewMat;
	glm::mat4 m_projMat;

	glm::vec3 m_position;
	glm::vec3 m_lookDir;
	glm::vec3 m_upDir;

	// how fast WASDQE moves camera
	float m_moveSpeed;

	// obrital camera
	bool m_orbiting;
	bool m_automaticOrbit;
	float m_radius;
	float m_angle;
	glm::vec3 m_lookPos;

	signed char TriHoriz(GLFWwindow* _Window);
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

