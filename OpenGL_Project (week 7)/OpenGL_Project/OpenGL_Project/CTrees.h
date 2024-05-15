#pragma once
#include "CModel.h"

class CTrees
{
public:
	CTrees();
	~CTrees();

	void Loadtree();

	void Render(GLint _program, GLint _texture);

private:
	CModel* Tree;
	glm::mat4 m_InstancedMVPs[256];
	// number of models
	int m_CountInstanced;
};

