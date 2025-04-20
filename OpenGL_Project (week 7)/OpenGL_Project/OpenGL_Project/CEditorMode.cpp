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
