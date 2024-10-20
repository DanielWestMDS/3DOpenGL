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
	CTessellationMesh(GLuint _texture);
	~CTessellationMesh();

	void Update();

	void Render(GLint _program, glm::mat4 _PVM);

private:
	GLuint m_DrawCount;
	int m_DrawType;

	GLuint m_Texture;

	GLuint VAO;
};

