// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CModel.h
// Description : Loads a model from an obj file. Contains Render for a single object and Render instanced for instanced model loading.
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#pragma once
#include <tiny_obj_loader.h>
#include "CCamera.h"
#include <iostream>

struct VertexStandard
{
public:
	glm::vec3 position;
	glm::vec2 texcoord;

	VertexStandard()
	{
		position = glm::vec3(0.0f);
		texcoord = glm::vec2(0.0f);
	};
	VertexStandard(glm::vec3 pos, glm::vec2 texc)
	{
		position = pos;
		texcoord = texc;
	}
};

class CModel
{

public:
	CModel(std::string FilePath);
	~CModel();

	virtual void Update(float DeltaTime);
	virtual void Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime, glm::mat4 _projMat, glm::mat4 _viewMat);
	virtual void RenderInstanced(GLint _program, GLint _texture, std::vector<glm::mat4> _matrixVec, float CurrentTime, glm::mat4 _ModelMat);

	GLuint GetVAO() { return VAO; };

protected:
	GLuint VAO;
	GLuint DrawCount;
	GLuint InstanceBuffer;
	int DrawType;
	int m_CountInstanced;
};

