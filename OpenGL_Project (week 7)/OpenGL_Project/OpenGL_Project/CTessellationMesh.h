#pragma once
#include "CCamera.h"

/// <summary>
/// Holds vertex information for each point on the model.
/// </summary>
struct VertexPoint
{
public:
	glm::vec3 position;

	VertexPoint()
	{
		position = glm::vec3(0.0f);
	};
	VertexPoint(glm::vec3 pos)
	{
		position = pos;
	}
};

class CTessellationMesh
{
public:
	CTessellationMesh();
	~CTessellationMesh();

	void Update();

	void Render(GLint _program, glm::mat4 _PVM);

private:
	GLuint m_DrawCount;
	int m_DrawType;

	GLuint VAO;
};

