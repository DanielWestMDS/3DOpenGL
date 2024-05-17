#include "CTrees.h"

CTrees::CTrees()
{
	LoadTree();
}

CTrees::~CTrees()
{
}

void CTrees::LoadTree()
{
	Tree = new CModel("Resources/Models/SM_Env_Tree_Palm_01.obj", m_InstancedMVPs);
}

void CTrees::Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime, glm::mat4 _projMat, glm::mat4 _viewMat)
{
	//glDrawArraysInstanced(Tree->DrawType, Tree->DrawCount, GL_UNSIGNED_INT, 0, m_CountInstanced);

	glUniformMatrix4fv(glGetUniformLocation(_program, "MVPs[0]"), 256, GL_FALSE, glm::value_ptr(m_InstancedMVPs[0]));
	//for (int i = 0; i < 100; i++)
	//{
	//	Tree->Render(_program, _texture, m_InstancedMVPs[i], CurrentTime, _projMat, _viewMat);
	//}

	Tree->Render(_program, _texture, _matrix, CurrentTime, _projMat, _viewMat);

}
