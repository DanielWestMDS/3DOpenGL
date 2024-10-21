// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CShadowMap.h
// Description : uses framebuffers to create a shadow texture to be applied to objects in the scene.
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#pragma once
#include "CCamera.h"

class CShadowMap
{
public:
	/// <summary>
	/// constructor. Initialises a framebuffer and shadow texture.
	/// </summary>
	/// <param name="_windowWidth"></param>
	/// <param name="_windowHeight"></param>
	CShadowMap(int _windowWidth, int _windowHeight);

	/// <summary>
	/// destructor
	/// </summary>
	~CShadowMap();

	/// <summary>
	/// Binds
	/// No color buffer bound, only depth buffer
	/// </summary>
	virtual void Bind();

	/// <summary>
	/// unbinds
	/// </summary>
	virtual void Unbind();

	/// <summary>
	/// returns the shadow texture to be used on objects
	/// </summary>
	/// <returns></returns>
	const GLuint GetShadowTexture() const { return m_ShadowTexture; };

protected:
	GLuint FBO, m_ShadowTexture;
};

