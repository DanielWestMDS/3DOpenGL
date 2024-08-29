// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CFrameBufferQuad.h
// Description : Quad for the FBO. No camera or anything just renders straight to the screen 
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#pragma once
#include "CCamera.h"

class CFrameBufferQuad
{
public:
    CFrameBufferQuad(GLuint _texture, GLuint _rainTexture, GLuint _program);
    ~CFrameBufferQuad();
    void Render();
    void UpdateTexture(GLuint textureID);
    void SetProgram(GLuint programID);

private:
    GLuint m_texture, m_rainTexture;
    GLuint quadVAO, quadVBO;
    GLuint m_program;

    void initQuad();
};

