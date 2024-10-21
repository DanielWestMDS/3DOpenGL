// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CGeometryBuffer.h
// Description : uses framebuffer to create 3 textures for normal position and shininess so that lighting calculations can be applied as a whole. 
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

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

	/// <summary>
	/// Begins reading info to put into the framebuffer
	/// </summary>
	void Bind();

	/// <summary>
	/// finished reading info for the framebuffer
	/// </summary>
	void Unbind();

	/// <summary>
	/// Send generated textures from the geometry buffer to the lighting pass
	/// </summary>
	/// <param name="_Program"></param>
	/// <param name="_cameraPos"></param>
	void PopulateProgram(GLuint _Program, glm::vec3 _cameraPos);

	/// <summary>
	/// allow a block of pixels to be copied frome one framebuffer to another
	/// </summary>
	void WriteDepth();

private:
	GLuint m_FBO;

	// texture stuff
	GLuint m_TexturePosition, m_TextureNormal, m_TextureAlbedoShininess, m_TextureDepth;
};

