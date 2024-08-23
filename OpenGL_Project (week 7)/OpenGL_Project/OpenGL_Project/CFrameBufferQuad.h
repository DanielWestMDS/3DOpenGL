#pragma once
#include "CCamera.h"

class CFrameBufferQuad
{
public:
    CFrameBufferQuad(GLuint _texture, GLuint _program);
    ~CFrameBufferQuad();
    void Render();
    void UpdateTexture(GLuint textureID);
    void SetProgram(GLuint programID);

private:
    GLuint m_texture;
    GLuint quadVAO, quadVBO;
    GLuint m_program;

    void initQuad();
};

