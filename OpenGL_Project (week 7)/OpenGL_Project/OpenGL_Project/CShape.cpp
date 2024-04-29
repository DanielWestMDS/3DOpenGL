#include "CShape.h"

CShape::CShape()
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

void CShape::Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime, glm::mat4 _projMat, glm::mat4 _viewMat, glm::vec3 _secondColor)
{
    // bind program and VAO
    glUseProgram(_program);
    glBindVertexArray(VAO);

    // send variables to shader via uniform
    GLint ProjectionMatLoc = glGetUniformLocation(_program, "ProjectionMat");
    glUniformMatrix4fv(ProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(_projMat));
    GLint ViewMatLoc = glGetUniformLocation(_program, "ViewMat");
    glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(_viewMat));
    //Model matrix
    GLint ModelMatrix = glGetUniformLocation(_program, "QuadModelMat");
    glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(_matrix));
    // Colour fade
    GLint SecondColorLoc = glGetUniformLocation(_program, "SecondColor");
    glUniform3fv(SecondColorLoc, 1, glm::value_ptr(_secondColor));

    // send variables to the shader with uniform for color change
    GLint CurrentTimeLoc = glGetUniformLocation(_program, "CurrentTime");
    glUniform1f(CurrentTimeLoc, CurrentTime + 1);

    // render
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
