// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CHeightMap.h
// Description : Header file for height map. Holds variables such as the texture array which is used for making each height a different texture. 
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#pragma once
#include "CCamera.h"
#include <vector>
#include <fstream>
#include <string>

struct VertexStandardHeightMap
{
public:
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
	float Height;

	VertexStandardHeightMap()
	{
		Position = glm::vec3(0.0f);
		TexCoord = glm::vec2(0.0f);
		Normal = glm::vec3(0.0f);
		Height = 0.0f;
	};
	VertexStandardHeightMap(glm::vec3 pos, glm::vec2 texc)
	{
		Position = pos;
		TexCoord = texc;
		Normal = glm::vec3(0.0f);
		Height = 0.0f;
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
	CHeightMap(HeightMapInfo& _BuildInfo, GLint _program, GLint _textureArray[4]);
	~CHeightMap();

	void Update(glm::mat4 _projMat, glm::mat4 _viewMat, glm::vec3 _cameraPos, glm::mat4 _matrix);
	void Render();

private:
	bool LoadHeightMap(HeightMapInfo& _BuildInfo);
	void BuildVertexData(HeightMapInfo& _BuildInfo);
	float Average(unsigned int _row, unsigned int _col, HeightMapInfo& _BuildInfo);
	void SmoothHeights(HeightMapInfo& _BuildInfo);

	std::vector<float> m_fHeightMap;
	std::vector<float> m_fHeightMapSmooth;
	float m_fHeightLevels[4];
	GLint m_textureArray[4] = {};
	int m_DrawType;
	int m_DrawCount;
	GLuint m_VAO;
	GLuint m_EBO;

	GLint m_program = 0;
	GLint m_texture = 0;
	glm::mat4 m_matrix = glm::mat4();
	glm::mat4 m_projMat = glm::mat4();
	glm::mat4 m_viewMat = glm::mat4();
	glm::vec3 m_cameraPos = glm::vec3();
};

