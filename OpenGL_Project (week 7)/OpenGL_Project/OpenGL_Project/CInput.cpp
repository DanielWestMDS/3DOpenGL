#include "CInput.h"

CInput::CInput(GLFWwindow* _Window)
{
	//glfwSetKeyCallback(_Window, KeyInput);
}

void CInput::KeyInput(GLFWwindow* _Window, int _iKey, int _iScanCode, int _iAction, int _iMods)
{
	if (glfwGetKey(_Window, GLFW_KEY_1))
	{
		glfwSetWindowShouldClose(_Window, true);
	}
}
