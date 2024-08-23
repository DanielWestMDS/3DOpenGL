#include "CQuad.h"

CQuad::CQuad(float _x, float _y, float _width, float _height, GLuint _texture, GLuint _program) : x(_x), y(_y), m_fWidth(_width), m_fHeight(_height), m_texture(_texture), m_program(_program)
{
    GLfloat vertices[] = {
        // Positions          // Texture Coords
        _x, _y + _height, 0.0f,  0.0f, 1.0f, // Top-left
        _x + _width, _y + _height, 0.0f,  1.0f, 1.0f, // Top-right
        _x + _width, _y, 0.0f,  1.0f, 0.0f, // Bottom-right
        _x, _y, 0.0f,  0.0f, 0.0f  // Bottom-left
    };

    GLuint indices[] = {
        0, 1, 2, // First Triangle
        2, 3, 0  // Second Triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

CQuad::~CQuad()
{
    glDeleteTextures(1, &m_texture);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void CQuad::Render(CCamera& camera)
{
    glUseProgram(m_program);

    // Get the uniform locations
    GLuint projLoc = glGetUniformLocation(m_program, "projection");
    GLuint modelLoc = glGetUniformLocation(m_program, "model");

    // Set the orthographic projection matrix to the shader
    glm::mat4 projection = camera.GetUIProjMat(); 
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Create the model matrix for the quad (translation based on x, y)
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Activate and bind the textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glUniform1i(glGetUniformLocation(m_program, "Texture0"), 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CQuad::RenderFrameBuffer()
{
    GLuint projLoc = glGetUniformLocation(m_program, "projection");

    // Set the orthographic projection matrix to the shader
    glm::mat4 projection = glm::mat4(1.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void CQuad::UpdateTexture(GLuint _texture)
{
    m_texture = _texture;
}

void CQuad::SetPosition(float _newX, float _newY)
{
    x = _newX;
    y = _newY;
    UpdateVertices();
}

void CQuad::UpdateVertices()
{
    GLfloat vertices[] = {
        // Positions          // Texture Coords
        x, y + m_fHeight, 0.0f,  0.0f, 1.0f, // Top-left
        x + m_fWidth, y + m_fHeight, 0.0f,  1.0f, 1.0f, // Top-right
        x + m_fWidth, y, 0.0f,  1.0f, 0.0f, // Bottom-right
        x, y, 0.0f,  0.0f, 0.0f  // Bottom-left
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}
