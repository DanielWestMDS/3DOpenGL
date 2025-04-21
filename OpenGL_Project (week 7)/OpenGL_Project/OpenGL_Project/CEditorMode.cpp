#include "CEditorMode.h"

/// <summary>
/// returns an instance of this singleton
/// </summary>
/// <returns></returns>
CEditorMode& CEditorMode::GetInstance()
{
	static CEditorMode instance;
	return instance;
}

bool CEditorMode::GetInEditor()
{
	return m_bInEditor;
}

void CEditorMode::SetInEditor(bool _inEditor)
{
	m_bInEditor = _inEditor;
}
