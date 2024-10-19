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
#include "CModel.h"
#include "CHeightMap.h"
#include <vector>

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

	/// <summary>
	/// Render for shadowmap buffer
	/// </summary>
	/// <param name="_ShadowProgram"></param>
	/// <param name="_LightVP"></param>
	void RenderShadow(GLuint _ShadowProgram, glm::mat4 _LightVP);

	/// <summary>
	/// Render for deferred rendering/geometry buffer
	/// </summary>
	/// <param name="_GeometryProgram"></param>
	void RenderGeometry(GLuint _GeometryProgram);

	/// <summary>
	/// adds an object to be rendered to the scene
	/// </summary>
	/// <param name="_Model"></param>
	void AddObject(CModel* _Model);

	/// <summary>
	/// Adds the height map as it has a seperate render function (and typically will only be added once)
	/// </summary>
	/// <param name="_Heightmap"></param>
	void AddHeightMap(CHeightMap* _Heightmap);

private:
	std::vector<CModel*> m_Objects = {};
	CHeightMap* m_HeightMap = nullptr;
};

