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

	/// <summary>
	/// Gets an instance of the singleton
	/// </summary>
	/// <returns></returns>
	static CEditorMode& GetInstance();

	/// <summary>
	/// Returns whether the game engine is in editing mode
	/// </summary>
	/// <returns></returns>
	bool GetInEditor();

	/// <summary>
	/// Changes the mode from play to editor / vice versa
	/// </summary>
	/// <param name="_inEditor"></param>
	void SetInEditor(bool _inEditor);

	/// <summary>
	/// returns a pointer to the camera
	/// </summary>
	/// <returns></returns>
	CCamera* GetCamera();

	/// <summary>
	/// Sets the pointer to the camera
	/// </summary>
	/// <param name="_camera"></param>
	void SetCamera(CCamera* _camera);

	/// <summary>
	/// Returns the player
	/// </summary>
	/// <returns></returns>
	class CPlayer* GetPlayer();

	/// <summary>
	/// Sets the current player
	/// </summary>
	/// <param name="_player"></param>
	void SetPlayer(CPlayer* _player);

	/// <summary>
	/// Returns whether the game has been finished
	/// </summary>
	/// <returns></returns>
	bool IsGameWon();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_isWon"></param>
	void SetGameWon(bool _isWon);

	// TODO: when in editor mode be able to save a scene to a text file and load a scene


private:
	// private singleton constructor
	CEditorMode() = default;
	// disable copy constructor
	CEditorMode(const CEditorMode&) = delete;
	CEditorMode& operator = (const CEditorMode&) = delete;

	bool m_bInEditor = false;

	CCamera* m_Camera = nullptr;

	CPlayer* m_Player = nullptr;

	bool m_bGameWon = false;
};

