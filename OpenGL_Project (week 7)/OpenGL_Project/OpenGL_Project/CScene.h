// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CScene
// Description : Holds objects for a scene to all be rendered in one function call
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#pragma once

#include <vector>
#include "reactphysics3d/reactphysics3d.h"

#include "CCamera.h"

class CScene
{
public:
	/// <summary>
	/// Constructor. nothing contained inside
	/// </summary>
	CScene();

	/// <summary>
	/// destructor
	/// </summary>
	~CScene();

	/// <summary>
	/// Goes through every object in the scene and renders it
	/// </summary>
	void Render();

	void Update(CCamera* Camera, float dt);

	/// <summary>
	/// Render for shadowmap buffer
	/// </summary>
	/// <param name="_ShadowProgram"></param>
	/// <param name="_LightVP"></param>
	//void RenderShadow(GLuint _ShadowProgram, glm::mat4 _LightVP);

	/// <summary>
	/// adds an object to be rendered to the scene
	/// </summary>
	/// <param name="_Model"></param>
	void AddObject(class CObject* _Object);

	/// <summary>
	/// Adds the height map as it has a seperate render function (and typically will only be added once)
	/// </summary>
	/// <param name="_Heightmap"></param>
	void AddHeightMap(class CHeightMap* _Heightmap);

	std::vector<CObject*> GetObjects();

	void RemoveObject(CObject* _Object);

	void MoveObjects();

	void LoadLevel();

	void SaveSceneToJson(const std::string& _FileName);

	void LoadSceneFromJson(const std::string& filename, rp3d::PhysicsWorld* physicsWorld, rp3d::PhysicsCommon& physicsCommon);


private:
	std::vector<CObject*> m_Objects = {};
	CHeightMap* m_HeightMap = nullptr;
};

