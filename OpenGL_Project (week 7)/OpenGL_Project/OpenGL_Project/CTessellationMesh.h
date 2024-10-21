// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CTessellationMesh.h
// Description : creates a triangles mesh which has vertices set in the tcs and tes shaders. 
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#pragma once
#include "CCamera.h"

/// <summary>
/// Holds vertex information for each point on the model.
/// </summary>
struct VertexPoint
{
public:
	glm::vec3 position;
	glm::vec2 texcoord;

	VertexPoint()
	{
		position = glm::vec3(0.0f);
		texcoord = glm::vec2(0.0f);
	};
	VertexPoint(glm::vec3 _pos, glm::vec2 _texcoord)
	{
		position = _pos;
		texcoord = _texcoord;
	}
};

class CTessellationMesh
{
public:
	/// <summary>
	/// constructor. creates mesh and sets texture.
	/// </summary>
	/// <param name="_texture"></param>
	CTessellationMesh(GLuint _texture);

	/// <summary>
	/// destructor
	/// </summary>
	~CTessellationMesh();

	/// <summary>
	/// Sends info to the shaders for tessellation. 
	/// </summary>
	/// <param name="_program"></param>
	/// <param name="_PVM"></param>
	void Render(GLint _program, glm::mat4 _PVM);

private:
	GLuint m_DrawCount;
	int m_DrawType;

	GLuint m_Texture;

	GLuint VAO;
};

