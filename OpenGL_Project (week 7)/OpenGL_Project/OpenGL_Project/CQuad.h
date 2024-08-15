#pragma once
#include "CCamera.h"

class CQuad
{
public:
	CQuad();
	~CQuad();

	//void Update();
	void Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime, glm::mat4 _projMat, glm::mat4 _viewMat);
	//void FrameBufferRender();

private:

	GLuint VBO, VAO, EBO = 0;
	GLint m_program, m_texture = {};
	glm::mat4 m_matrix, m_projMat, m_viewMat = {};
};

