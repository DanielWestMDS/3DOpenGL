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

    /// <summary>
    /// Constructor. Initialises anything passed into the uniform 
    /// </summary>
    /// <param name="_texture"></param>
    /// <param name="_rainTexture"></param>
    /// <param name="_program"></param>
    CFrameBufferQuad(GLuint _texture, GLuint _rainTexture, GLuint _program);

    /// <summary>
    /// destructor
    /// </summary>
    ~CFrameBufferQuad();

    /// <summary>
    /// Passes needed variables into the program via uniform. Binds VAO
    /// </summary>
    void Render();

    /// <summary>
    /// Updates the texture being displayed to the quad
    /// </summary>
    /// <param name="textureID"></param>
    void UpdateTexture(GLuint textureID);

    /// <summary>
    /// Updates the program being used to change the effect on screen
    /// </summary>
    /// <param name="programID"></param>
    void SetProgram(GLuint programID);

private:
    GLuint m_texture, m_rainTexture;
    GLuint quadVAO, quadVBO;
    GLuint m_program;

    /// <summary>
    /// creates a manual quad and initialises VAO and VBO
    /// </summary>
    void initQuad();
};

