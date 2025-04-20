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

#include "CCamera.h"
//#include "CModel.h"
#include "CHeightMap.h"
#include "CObject.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::Render()
{
	for (auto Object : m_Objects)
	{
		Object->GetModel()->Render();
	}

	if (m_HeightMap != nullptr)
	{
		m_HeightMap->Render();
	}
}

void CScene::Update(CCamera* Camera, float dt)
{
	int i = 0;
	for (auto Object : m_Objects)
	{
		// update model in relation to camera
		Object->GetModel()->Update(Camera->GetProjMat(), Camera->GetViewMat(), Camera->GetPosition());

		// update object
		Object->Update(dt);
		//std::cout << "should have just updated object" << i << std::endl;

		i++;
	}
}

//void CScene::RenderShadow(GLuint _ShadowProgram, glm::mat4 _LightVP)
//{
//	for (auto Object : m_Objects)
//	{
//		Object->GetModel()->RenderShadow(_ShadowProgram, _LightVP);
//	}
//
//	if (m_HeightMap != nullptr)
//	{
//		m_HeightMap->RenderShadow(_ShadowProgram);
//	}
//}

//void CScene::RenderGeometry(GLuint _GeometryProgram)
//{
//	for (auto Model : m_Objects)
//	{
//		Model->RenderGeometry(_GeometryProgram);
//	}
//}

void CScene::AddObject(CObject* _Model)
{
	m_Objects.push_back(_Model);
}

void CScene::AddHeightMap(CHeightMap* _Heightmap)
{
	m_HeightMap = _Heightmap;
}

void CScene::MoveObjects()
{
	for (auto Object : m_Objects)
	{
		Object->SetPosition(glm::vec3(Object->GetPosition().x + 100));
	}
}
