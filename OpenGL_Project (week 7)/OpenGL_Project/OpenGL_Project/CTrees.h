#pragma once
#include "CModel.h"

class CTrees
{
public:
	CTrees();
	~CTrees();

	void LoadTree();

	void Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime, glm::mat4 _projMat, glm::mat4 _viewMat);

private:
	CModel* Tree;
	glm::mat4 m_InstancedMVPs[256];
	// number of models
	int m_CountInstanced;
};

