// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CCamera.h
// Description : Camera object which contians projection and view matrices for perspective and orthographic. 
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>
#include <iostream>

class CCamera
{
public:
	CCamera();
	~CCamera();

	void Update(int _iWindowSize, GLFWwindow* _Window, glm::vec2 _MousePos, float _dt);
	// view matrix passed into object render functions for MVP matrixs
	glm::mat4 GetViewMat();
	// projection matrix passed into object render functions for MVP matrixs
	glm::mat4 GetProjMat();
	//gets projection times view matrix
	glm::mat4 GetVP();
	// projection matrix for UI
	glm::mat4 GetUIProjMat() { return m_UIprojMat; };
	// view matrix for UI
	glm::mat4 GetUIViewMat() { return m_UIviewMat; };
	// get position
	glm::vec3 GetPosition() { return m_position; };

	// Free camera
	void Input(GLFWwindow* _Window, float _dt);
	void PrintCamPos();

	// for model movement related to camera
	// forward vector
	glm::vec3 GetForward() { return glm::normalize(-m_lookDir); };
	// right vector
	glm::vec3 GetRight() { return glm::normalize(glm::cross(GetForward(), glm::vec3(0, 1, 0))); };
	// up vector
	glm::vec3 GetUp() { return glm::normalize(glm::cross(GetRight(), GetForward())); };
	// move vector 
	glm::vec3 GetMove(GLFWwindow* _Window, float _dt);


private:
	// for main camera
	glm::mat4 m_viewMat;
	glm::mat4 m_projMat;

	glm::vec3 m_position = {};
	glm::vec3 m_lookDir = {};
	glm::vec3 m_upDir;

	// for UI
	glm::mat4 m_UIprojMat;
	glm::mat4 m_UIviewMat;
	//glm::mat4 m_UIviewMat;
	//glm::vec3 m_UIposition;
	//glm::vec3 m_UIlookDir;
	//glm::vec3 m_UIupDir;

	// how fast WASDQE moves camera
	float m_moveSpeed;
	float m_mouseSpeed;

	// for free camera
	glm::vec2 m_lastMouse;
	float m_yaw = 0;
	float m_pitch = 0;
	bool m_bMousePressed = false;

	//glm::vec3 m_lookPos;

	// movement
	signed char TriHoriz(GLFWwindow* _Window);
	signed char TriVerti(GLFWwindow* _Window);
	void ChangeHeight(GLFWwindow* _Window, float _dt);
};

