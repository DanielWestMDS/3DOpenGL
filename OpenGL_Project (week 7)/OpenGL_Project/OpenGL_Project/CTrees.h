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
	void MakeMatrices();

	CModel* Tree;
	// vector that holds all tree matrices
	std::vector<glm::mat4> m_InstancedMVPs;
	// tree matrices values
	glm::vec3 m_Position;
	float m_Rotation;
	glm::vec3 m_Scale;
	// number of models
	int m_CountInstanced;
};

