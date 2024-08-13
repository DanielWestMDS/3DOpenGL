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

void CScene::AddObject(CModel* _Model)
{
	m_Objects.push_back(_Model);
}

void CScene::AddHeightMap(CHeightMap* _Heightmap)
{
	m_HeightMap = _Heightmap;
}
