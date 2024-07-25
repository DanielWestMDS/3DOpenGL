#pragma once
// camera has all the glfw stuff
#include "CCamera.h"
#include <vector>
#include <fstream>

struct VertexStandard
{
public:
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
	;
	VertexStandard()
	{
		Position = glm::vec3(0.0f);
		TexCoord = glm::vec2(0.0f);
		Normal = glm::vec3(0.0f);
	};
	VertexStandard(glm::vec3 pos, glm::vec2 texc)
	{
		Position = pos;
		TexCoord = texc;
		Normal = glm::vec3(0.0f);
	}
};

struct HeightMapInfo
{
	std::string FilePath = "";
	unsigned int Width = 0;
	unsigned int Depth = 0;
	float CellSpacing = 1.0f;
};

class CHeightMap
{
public:
	CHeightMap(HeightMapInfo& _BuildInfo);
	~CHeightMap();

	void Render(GLint _program, GLint _texture, glm::mat4 _VP);

private:
	bool LoadHeightMap(HeightMapInfo& _BuildInfo);
	void BuildVertexData(HeightMapInfo& _BuildInfo);
	void BuildEBO(HeightMapInfo& _BuildInfo);

	std::vector<float> m_fHeightMap;
	int m_DrawType;
	int m_DrawCount;
	GLuint m_VAO;
	GLuint m_EBO;
};

