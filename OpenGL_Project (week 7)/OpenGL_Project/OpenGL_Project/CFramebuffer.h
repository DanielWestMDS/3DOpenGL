// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CFrameBuffer.h
// Description : Converts the scene to a texture so that the scene can be altered in its entirety 
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#pragma once
#include "CCamera.h"

class CFramebuffer
{
public:
	CFramebuffer(int _windowWidth, int _windowHeight);
	~CFramebuffer();

	void Bind();
	void Unbind();

	GLuint GetRenderTexture() { return m_RenderTexture; };


private:
	void RenderBuffer();

	GLuint m_FBO;
	GLuint m_RenderTexture;

	int m_WindowWidth, m_WindowHeight;
};

