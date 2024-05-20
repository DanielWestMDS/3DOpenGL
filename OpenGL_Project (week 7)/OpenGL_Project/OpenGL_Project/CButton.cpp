#include "CButton.h"

CButton::CButton()
{
    {
        // Define vertices for two triangles
        GLfloat m_vertices[] = {
            //	// position		   // color			  
                 -0.5f, 0.875f, 0.0f,   1.0f, 0.0f, 0.0f, // A
                -1.0f, 0.0f, 0.0f,      0.0f, 1.0f, 0.0f, // B
                -0.5f, -0.875f,         0.0f, 0.0f, 0.0f, 1.0f, // C
                0.5f, -0.875f, 0.0f,    0.0f, 1.0f, 1.0f, // D
                1.0f, 0.0f, 0.0f,       1.0f, 1.0f, 0.0f, // E
                 0.5f, 0.875f, 0.0f,    1.0f, 0.0f, 1.0f, // F

                 /*            //	// position		   // color			  // texture coords
                 -0.5f, 0.75f, 0.0f,  1.0f, 0.0f, 0.0f,    -0.5f, 0.75f,  // A
                 0.5f, 0.75f, 0.0f,   0.0f, 1.0f, 0.0f,    0.5f, 0.75f,   // B
                 1.0f, -0.125f, 0.0f,  1.0f, 0.0f, 1.0f,   1.0f, -0.125f, // C
                 0.5f, -0.75f, 0.0f,   0.0f, 1.0f, 1.0f,   0.5f, -0.75f,  // D
                 - 0.5f, -0.75f, 0.0f,   0.0f, 1.0f, 1.0f, -0.5f, -0.75f, // E
                 -1.0f, -0.125f, 0.0f,   0.0f, 1.0f, 1.0f, -1.0f, -0.125f,// F*/
        };

        // Define indices for the triangle vertices
        GLuint m_indices[] = {
            0, 2, 4, // Triangle 1
            0, 1, 2,  // Triangle 2
            2, 4, 3, // Triangle 3
            0, 4, 5, // Triangle 4
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Color 
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //// Texture
        //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        //glEnableVertexAttribArray(2);

        // Unbind the VAO
        glBindVertexArray(0);
    }
}

void CButton::Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime)
{
    // bind program and VAO
    glUseProgram(_program);
    glBindVertexArray(VAO);

    //Model matrix
    GLint ModelMatrix = glGetUniformLocation(_program, "QuadModelMat");
    glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(_matrix));

    // send variables to the shader with uniform for color change
    GLint CurrentTimeLoc = glGetUniformLocation(_program, "CurrentTime");
    glUniform1f(CurrentTimeLoc, CurrentTime + 1);

    // Activate and bind the textures
    //// texture 1
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, _texture);
    //glUniform1i(glGetUniformLocation(_program, "Texture0"), 0);

    //// set the filtering and mipmap parameters for this texture
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // render
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
