#pragma once
#include "CCamera.h"

class CInput
{
public:
	CInput(GLFWwindow* _Window);
	~CInput();



	void KeyInput(GLFWwindow* _Window, int _iKey, int _iScanCode, int _iAction, int _iMods);
};

