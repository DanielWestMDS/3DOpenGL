#include "CTrees.h"

CTrees::CTrees()
{
}

CTrees::~CTrees()
{
}

void CTrees::Loadtree()
{
	Tree = new CModel("Resources/Models/SM_Prop_Statue_02.obj");
}

void CTrees::Render(GLint _program, GLint _texture)
{
	//glDrawArraysInstanced(Tree->DrawType, Tree->DrawCount, GL_UNSIGNED_INT, 0, m_CountInstanced);

	glUniformMatrix4fv(glGetUniformLocation(_program, "MVPs[0]"), 256, GL_FALSE, glm::value_ptr(m_InstancedMVPs[0]));
}
