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

void CScene::Update(CCamera* Camera, float dt, GLFWwindow* _window)
{
	

	int i = 0;
	for (auto Object : m_Objects)
	{
		// update model in relation to camera
		Object->GetModel()->Update(Camera->GetProjMat(), Camera->GetViewMat(), Camera->GetPosition());

		// update object
		Object->Update(dt, _window);
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
	//_Model.SetID(m_Objects.size());
	m_Objects.push_back(_Model);
}

void CScene::AddHeightMap(CHeightMap* _Heightmap)
{
	m_HeightMap = _Heightmap;
}

std::vector<CObject*> CScene::GetObjects()
{
	return m_Objects;
}

void CScene::RemoveObject(CObject* _Object)
{
	// remove the selected object to avoid referencing deleted object
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), _Object), m_Objects.end());
}

void CScene::MoveObjects()
{
	for (auto Object : m_Objects)
	{
		Object->SetPosition(glm::vec3(Object->GetPosition().x + 100));
	}
}

void CScene::LoadLevel()
{
	//TODO: reset everything to original position and 0 momentum
}

void CScene::SaveSceneToJson(const std::string& _FileName) 
{
	json sceneJson;

	for (const auto& obj : m_Objects)
	{
		sceneJson["objects"].push_back(obj->ToJson());
	}

	std::ofstream file(_FileName);
	if (file.is_open()) 
	{
		// 4 space indentation
		file << sceneJson.dump(4); 
		file.close();
	}
	else 
	{
		std::cerr << "Failed to open " << _FileName << " for writing!" << std::endl;
	}
}

void CScene::LoadSceneFromJson(const std::string& _filename,
	rp3d::PhysicsWorld* _physicsWorld,
	rp3d::PhysicsCommon& _physicsCommon)
{
	std::ifstream file(_filename);
	if (!file.is_open())
	{
		std::cerr << "Failed to open scene file: " << _filename << std::endl;
		return;
	}

	json sceneJson;
	file >> sceneJson;
	file.close();

	m_Objects.clear();

	// could prolly multithread this
	for (const auto& objJson : sceneJson["objects"])
	{
		CObject* obj = CObject::FromJson(objJson, _physicsWorld, _physicsCommon);
		m_Objects.push_back(obj);
	}
}


