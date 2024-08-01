#pragma once
// camera has all the glfw stuff
#include "CCamera.h"
#include <vector>
#include <fstream>

struct VertexStandardHeightMap
{
public:
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
	glm::vec3 Color;

	VertexStandardHeightMap()
	{
		Position = glm::vec3(0.0f);
		TexCoord = glm::vec2(0.0f);
		Normal = glm::vec3(0.0f);
		Color = glm::vec3(0.0f);
	};
	VertexStandardHeightMap(glm::vec3 pos, glm::vec2 texc)
	{
		Position = pos;
		TexCoord = texc;
		Normal = glm::vec3(0.0f);
		Color = glm::vec3(0.0f);
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

	void Render(GLint _program, GLint _texture, glm::mat4 _projMat, glm::mat4 _viewMat, glm::vec3 _cameraPos, glm::mat4 _matrix);

private:
	bool LoadHeightMap(HeightMapInfo& _BuildInfo);
	void BuildVertexData(HeightMapInfo& _BuildInfo);
	float Average(unsigned int _row, unsigned int _col, HeightMapInfo& _BuildInfo);
	void SmoothHeights(HeightMapInfo& _BuildInfo);
	glm::vec3 CalculateColor(float height);

	std::vector<float> m_fHeightMap;
	std::vector<float> m_fHeightMapSmooth;
	int m_DrawType;
	int m_DrawCount;
	GLuint m_VAO;
	GLuint m_EBO;
};

