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

	void Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime, glm::mat4 _projMat, glm::mat4 _viewMat, glm::vec3 _cameraPos);
	void AddObject(CModel* _Model);
	void AddHeightMap(CHeightMap* _Heightmap);
};

