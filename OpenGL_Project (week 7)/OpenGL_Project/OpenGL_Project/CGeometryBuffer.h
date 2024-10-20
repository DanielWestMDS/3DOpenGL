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

	/// <summary>
	/// Send generated textures from the geometry buffer to the lighting pass
	/// </summary>
	/// <param name="_Program"></param>
	/// <param name="_cameraPos"></param>
	void PopulateProgram(GLuint _Program, glm::vec3 _cameraPos);

private:
	GLuint m_FBO;

	// texture stuff
	GLuint m_TexturePosition, m_TextureNormal, m_TextureAlbedoShininess, m_TextureDepth;
};

