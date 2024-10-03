// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CScene.cpp
// Description : Loads objects into a scene to render everything cleaner 
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#include "CScene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::Render()
{
	for (auto Model : m_Objects)
	{
		Model->Render();
	}

	if (m_HeightMap != nullptr)
	{
		m_HeightMap->Render();
	}
}

void CScene::RenderShadow(GLuint _ShadowProgram, glm::mat4 _LightVP)
{
	for (auto Model : m_Objects)
	{
		Model->RenderShadow(_ShadowProgram, _LightVP);
	}

	if (m_HeightMap != nullptr)
	{
		m_HeightMap->RenderShadow(_ShadowProgram);
	}
}

void CScene::AddObject(CModel* _Model)
{
	m_Objects.push_back(_Model);
}

void CScene::AddHeightMap(CHeightMap* _Heightmap)
{
	m_HeightMap = _Heightmap;
}
