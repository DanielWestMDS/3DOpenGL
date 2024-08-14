#pragma once
#include "CCamera.h"

class CFramebuffer
{
public:
	CFramebuffer(int _windowWidth, int _windowHeight);
	~CFramebuffer();

	void Bind();
	void Unbind();

private:
	void RenderBuffer();

	GLuint m_FBO;
	GLuint m_RenderTexture;

	int m_WindowWidth, m_WindowHeight;
};

