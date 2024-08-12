#include "CScene.h"

CScene::CScene()
{
}

void CScene::Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime, glm::mat4 _projMat, glm::mat4 _viewMat, glm::vec3 _cameraPos)
{
	for (auto Model : m_Objects)
	{
		Model->Render();
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
