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

/// <summary>
/// struct that holds data for every vertex in the height map
/// </summary>
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

/// <summary>
/// info defining how the height map is constructed
/// </summary>
/// <param name="_row"></param>
/// <param name="_col"></param>
/// <param name="_BuildInfo"></param>
/// <returns></returns>
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
	/// <summary>
	/// Constructor. Uses build info to create a height map. Initialises the program and textures to be passed into the program
	/// </summary>
	CHeightMap(HeightMapInfo& _BuildInfo, GLint _program, GLint _textureArray[4]);

	/// <summary>
	/// destructor.
	/// </summary>
	~CHeightMap();

	/// <summary>
	/// Called every frame. Updates matrices so moving the camera works properly
	/// </summary>
	void Update(glm::mat4 _projMat, glm::mat4 _viewMat, glm::vec3 _cameraPos, glm::mat4 _matrix);

	/// <summary>
	/// Binds VAO, passes all necessary matrices and textures in via uniform, renders vertices to screen.
	/// </summary>
	void Render();

private:

	/// <summary>
	/// Gets Height map info from a raw file
	/// </summary>
	bool LoadHeightMap(HeightMapInfo& _BuildInfo);

	/// <summary>
	/// Uses raw file data to create positions for all the vertices. Binds everything to VAO attribarray and VBO
	/// </summary>
	void BuildVertexData(HeightMapInfo& _BuildInfo);

	/// <summary>
	/// returns the average based on the surrounding 8 vertices
	/// </summary>
	float Average(unsigned int _row, unsigned int _col, HeightMapInfo& _BuildInfo);

	/// <summary>
	/// Uses the average to decrease the height differences between vertex neighbours. 
	/// </summary>
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

