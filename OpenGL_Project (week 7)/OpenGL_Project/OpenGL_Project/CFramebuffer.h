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
	/// <summary>
	/// Constructor, creates FBO and depth buffer
	/// </summary>
	/// <param name="_windowWidth"></param>
	/// <param name="_windowHeight"></param>
	CFramebuffer(int _windowWidth, int _windowHeight);

	/// <summary>
	/// destructor
	/// </summary>
	~CFramebuffer();

	/// <summary>
	/// Binds the framebuffer. ANything after this function call will be added to the FBO texture
	/// </summary>
	void Bind();
	/// <summary>
	/// unbinds the frame buffer. Anything after this function call will not be added to the FBO texture
	/// </summary>
	void Unbind();

	/// <summary>
	/// Getter for the render texture. Should be passed to a quad to display the scene
	/// </summary>
	/// <returns></returns>
	GLuint GetRenderTexture() { return m_RenderTexture; };


private:
	GLuint m_FBO;
	GLuint m_RenderTexture;

	int m_WindowWidth, m_WindowHeight;
};

