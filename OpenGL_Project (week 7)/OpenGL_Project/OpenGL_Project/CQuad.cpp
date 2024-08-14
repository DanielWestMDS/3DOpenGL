// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CButton.cpp
// Description : Contains vertex information for UI button. Passes in uniforms for MVP for the shader.
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#include "CQuad.h"

CQuad::CQuad()
{
    {
        // Define vertices for two triangles
        GLfloat m_vertices[] = {
            // position		   // color			  // texture coords
            0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // top left
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, // bottom left
            1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f, // bottom right
            1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 1.0f,  1.0f, 1.0f, // top right
        };

        // Define indices for the triangle vertices
        GLuint m_indices[] = {
            0, 2, 3, // second triangle
            0, 1, 2, // first triangle
        };

        // Generate and bind a VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind an EBO
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

        // Generate and bind a VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

        // Set vertex attribute pointers
        // Position 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Color 
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Texture
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Unbind the VAO
        glBindVertexArray(0);
    }
}

CQuad::~CQuad()
{
}

void CQuad::Update(GLint _program, GLint _texture, glm::mat4 _matrix, glm::mat4 _projMat, glm::mat4 _viewMat)
{
    m_program = _program;
    m_texture = _texture;
    m_matrix = _matrix;
    m_projMat = _projMat;
    m_viewMat = _viewMat;
}

void CQuad::Render()
{
    // bind program and VAO
    glUseProgram(m_program);
    glBindVertexArray(VAO);

    // send variables to shader via uniform
    GLint ProjectionMatLoc = glGetUniformLocation(m_program, "ProjectionMat");
    glUniformMatrix4fv(ProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(m_projMat));
    GLint ViewMatLoc = glGetUniformLocation(m_program, "ViewMat");
    glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(m_viewMat));

    //Model matrix
    GLint ModelMatrix = glGetUniformLocation(m_program, "QuadModelMat");
    glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(m_matrix));

    // render
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void CQuad::FrameBufferRender()
{
    // bind program and VAO
    glUseProgram(m_program);
    glBindVertexArray(VAO);

    // send variables to shader via uniform
    GLint ProjectionMatLoc = glGetUniformLocation(m_program, "ProjectionMat");
    glUniformMatrix4fv(ProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(m_projMat));
    GLint ViewMatLoc = glGetUniformLocation(m_program, "ViewMat");
    glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(m_viewMat));

    //Model matrix
    GLint ModelMatrix = glGetUniformLocation(m_program, "QuadModelMat");
    glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(m_matrix));

    // render
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
