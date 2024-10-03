#pragma once
#include "CCamera.h"

class CShadowMap
{
public:
	CShadowMap(int _windowWidth, int _windowHeight);
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

	const GLuint GetShadowTexture() const { return m_ShadowTexture; };

protected:
	GLuint FBO, m_ShadowTexture;
};

