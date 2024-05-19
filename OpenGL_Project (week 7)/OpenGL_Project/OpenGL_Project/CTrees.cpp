#include "CTrees.h"

CTrees::CTrees()
{
	MakeMatrices();
	LoadTree();


}

CTrees::~CTrees()
{
}

void CTrees::LoadTree()
{
	Tree = new CModel("Resources/Models/SM_Env_Tree_Palm_01.obj");
}

void CTrees::Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime, glm::mat4 _projMat, glm::mat4 _viewMat)
{
	//glDrawArraysInstanced(Tree->DrawType, Tree->DrawCount, GL_UNSIGNED_INT, 0, m_CountInstanced);

	//glUniformMatrix4fv(glGetUniformLocation(_program, "MVPs[0]"), 256, GL_FALSE, glm::value_ptr(m_InstancedMVPs[0]));
	//for (int i = 0; i < 100; i++)
	//{
	//	Tree->Render(_program, _texture, m_InstancedMVPs[i], CurrentTime, _projMat, _viewMat);
	//}

	//Tree->RenderInstanced(_program, _texture, _matrix, CurrentTime, _projMat, _viewMat, m_InstancedMVPs);

}

void CTrees::MakeMatrices()
{
	// randomize the position of trees
	//for (int i = 0; i < 100; i++)
	//{
	//	// calculate model matrix
	//	glm::mat4 TranslationMat = glm::translate(glm::mat4(1.0f), m_Position);
	//	glm::mat4 RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians((m_Rotation) * 10), glm::vec3(1.0f, 1.0f, 1.0f));
	//	glm::mat4 ScaleMat = glm::scale(glm::mat4(1.0f), m_Scale);
	//	// add new matrix to the vector
	//	m_InstancedMVPs.push_back(TranslationMat * RotationMat * ScaleMat);
	//}
}
