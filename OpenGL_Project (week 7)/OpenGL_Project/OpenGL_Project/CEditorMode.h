/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2025 Media Design School
File Name : CEditorMode.h
Description : A singleton class
Author : Daniel West
Mail : daniel.west@mds.ac.nz
**************************************************************************/

#pragma once
class CEditorMode
{
public:
	static CEditorMode& GetInstance();

	bool GetInEditor();

	void SetInEditor(bool _inEditor);


private:
	// private singleton constructor
	CEditorMode() = default;
	// disable copy constructor
	CEditorMode(const CEditorMode&) = delete;
	CEditorMode& operator = (const CEditorMode&) = delete;

	bool m_bInEditor = true;
};

