// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CFrameBufferQuad.cpp
// Description : Creates a quad manually and passes in uniforms for the FBO texture and the rain noise as well as resolution and time. 
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#include "CFrameBufferQuad.h"

CFrameBufferQuad::CFrameBufferQuad(GLuint _texture, GLuint _textureNoise, GLuint _program) : m_program(_program), m_texture(_texture), m_rainTexture(_textureNoise)
{
    initQuad();
}

CFrameBufferQuad::~CFrameBufferQuad() 
{
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}

void CFrameBufferQuad::initQuad() 
{
    GLfloat quadVertices[] = {
        // Positions        // Texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
}

void CFrameBufferQuad::Render() 
{
    if (m_program) 
    {
        glUseProgram(m_program);
    }

    // Activate and bind the textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glUniform1i(glGetUniformLocation(m_program, "Texture0"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_rainTexture);
    glUniform1i(glGetUniformLocation(m_program, "iChannel1"), 1);

    // for the effect
    glUseProgram(m_program);
    glUniform2f(glGetUniformLocation(m_program, "iResolution"), 800.0f, 800.0f);

    float timeValue = glfwGetTime();
    glUniform1f(glGetUniformLocation(m_program, "iTime"), timeValue);



    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    if (m_program) 
    {
        glUseProgram(0);
    }
}

void CFrameBufferQuad::RenderLightingPass()
{
    if (m_program)
    {
        glUseProgram(m_program);
    }
    else
    {
        return;
    }

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    if (m_program)
    {
        glUseProgram(0);
    }
}



void CFrameBufferQuad::UpdateTexture(GLuint textureID) 
{
    m_texture = textureID;
}

void CFrameBufferQuad::SetProgram(GLuint programID) 
{
    m_program = programID;
}
