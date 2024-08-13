#pragma once
#include "CModel.h"
#include "CHeightMap.h"
#include <vector>

class CScene
{
private:
	std::vector<CModel*> m_Objects = {};
	CHeightMap* m_HeightMap = nullptr;

public:
	CScene();
	~CScene();

	void Render();
	void AddObject(CModel* _Model);
	void AddHeightMap(CHeightMap* _Heightmap);
};

