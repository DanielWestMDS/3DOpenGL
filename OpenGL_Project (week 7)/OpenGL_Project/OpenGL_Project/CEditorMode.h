/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2025 Media Design School
File Name : CEditorMode.h
Description : A singleton class. Setting window and camera in this class is probably not the correct thing but the way I was doing it before, the player could not be saved to a json file.
Author : Daniel West
Mail : daniel.west@mds.ac.nz
**************************************************************************/

#pragma once

#include "CCamera.h"

class CEditorMode
{
public:
	static CEditorMode& GetInstance();

	bool GetInEditor();

	void SetInEditor(bool _inEditor);

	CCamera* GetCamera();

	void SetCamera(CCamera* _camera);

	class CPlayer* GetPlayer();

	void SetPlayer(CPlayer* _player);

	// TODO: when in editor mode be able to save a scene to a text file and load a scene


private:
	// private singleton constructor
	CEditorMode() = default;
	// disable copy constructor
	CEditorMode(const CEditorMode&) = delete;
	CEditorMode& operator = (const CEditorMode&) = delete;

	bool m_bInEditor = true;

	CCamera* m_Camera = nullptr;

	CPlayer* m_Player = nullptr;
};

