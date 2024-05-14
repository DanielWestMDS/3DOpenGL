#pragma once
#include <tiny_obj_loader.h>
//#include <glew.h>
//#include <glfw3.h>
//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
//#include <gtc/type_ptr.hpp>
//#include <stb_image.h>
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

protected:
	GLuint VAO;
	GLuint DrawCount;
	int DrawType;
};

