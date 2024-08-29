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
	glm::vec3 normal;

	VertexStandard()
	{
		normal = glm::vec3(0.0f);
		position = glm::vec3(0.0f);
		texcoord = glm::vec2(0.0f);
	};
	VertexStandard(glm::vec3 pos, glm::vec2 texc)
	{
		normal = glm::vec3(0.0f);
		position = pos;
		texcoord = texc;
	}
};

class CModel
{

public:
	CModel(std::string FilePath, GLint _program, GLint _texture, glm::mat4 _matrix);
	~CModel();

	virtual void Update(glm::mat4 _projMat, glm::mat4 _viewMat, glm::vec3 _cameraPos);
	virtual void Render();
	//virtual void RenderInstanced(GLint _program, GLint _texture, std::vector<glm::vec3> _instancePositions, glm::mat4 _modelMat, glm::vec3 _cameraPos, glm::mat4 _VP);

	GLuint GetVAO() { return VAO; };

protected:
	GLuint VAO;
	GLuint DrawCount;
	GLuint InstanceBuffer;
	int DrawType;
	int m_CountInstanced;

	GLint m_program = 0;
	GLint m_texture = 0;
	glm::mat4 m_matrix = glm::mat4();
	glm::mat4 m_projMat = glm::mat4();
	glm::mat4 m_viewMat = glm::mat4();
	glm::vec3 m_cameraPos = glm::vec3();
};

