#pragma once
#include "CCamera.h"

class CGeometryBuffer
{
public:
	/// <summary>
	/// Constructor. Generates and binds the FBO. 
	/// </summary>
	CGeometryBuffer();
	~CGeometryBuffer();

	void Bind();
	void Unbind();

	void PopulateProgram(GLuint _Program);

private:
	GLuint m_FBO;

	// texture stuff
	GLuint m_TexturePosition, m_TextureNormal, m_TextureAlbedoShininess, m_TextureDepth;
};

