#pragma once
#include "CCamera.h"

class CQuad
{
public:
	CQuad();
	~CQuad();

	void Update(GLint _program, GLint _texture, glm::mat4 _matrix, glm::mat4 _projMat, glm::mat4 _viewMat);
	void Render();
	void FrameBufferRender();

private:

	GLuint VBO, VAO, EBO = 0;
	GLint m_program, m_texture = {};
	glm::mat4 m_matrix, m_projMat, m_viewMat = {};
};

