#include "CEditorMode.h"

#include "CPlayer.h"

/// <summary>
/// returns an instance of this singleton
/// </summary>
/// <returns></returns>
CEditorMode& CEditorMode::GetInstance()
{
	static CEditorMode instance;
	return instance;

	//TODO: make mouse disappear in play mode
}

bool CEditorMode::GetInEditor()
{
	return m_bInEditor;
}

void CEditorMode::SetInEditor(bool _inEditor)
{
	m_bInEditor = _inEditor;
}

CCamera* CEditorMode::GetCamera()
{
	return m_Camera;
}

void CEditorMode::SetCamera(CCamera* _camera)
{
	m_Camera = _camera;
}

CPlayer* CEditorMode::GetPlayer()
{
	return m_Player;
}

void CEditorMode::SetPlayer(CPlayer* _player)
{
	m_Player = _player;
}

bool CEditorMode::IsGameWon()
{
	return m_bGameWon;
}

void CEditorMode::SetGameWon(bool _isWon)
{
	m_bGameWon = _isWon;
}
