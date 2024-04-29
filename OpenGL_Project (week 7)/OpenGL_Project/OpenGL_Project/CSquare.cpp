#include "CSquare.h"

CSquare::CSquare()
{
    // Define vertices for two triangles
    GLfloat m_vertices[] = {
    	// position		   // color			  // texture coords
    -1.0f, 1.0f, 1.0f,        1.0f,1.0f,1.0f,       0.0f, 1.0f,//front quad
    -1.0f, -1.0f, 1.0f,        1.0f,1.0f,1.0f,       0.0f, 0.0f,
    1.0f, -1.0f, 1.0f,        1.0f,1.0f,1.0f,       1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,        1.0f,1.0f,1.0f,       1.0f, 1.0f,
                                                  
    1.0f, 1.0f, -1.0f,        1.0f,1.0f,1.0f,       0.0f, 1.0f,    //back qd
    1.0f, -1.0f, -1.0f,        1.0f,1.0f,1.0f,       0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,    1.0f,1.0f,1.0f,       1.0f, 0.0f,
    -1.0f,    1.0f, -1.0f,    1.0f,1.0f,1.0f,       1.0f, 1.0f,
                                                  
    1.0f, 1.0f, 1.0f,        1.0f,1.0f,1.0f,       0.0f, 1.0f, //right qd
    1.0f, -1.0f, 1.0f,        1.0f,1.0f,1.0f,       0.0f, 0.0f,
    1.0f, -1.0f, -1.0f,        1.0f,1.0f,1.0f,       1.0f, 0.0f,
    1.0f, 1.0f, -1.0f,        1.0f,1.0f,1.0f,       1.0f, 1.0f,
                                                  
    -1.0f, 1.0f, -1.0f,        1.0f,1.0f,1.0f,       0.0f, 1.0f, //left qd
    -1.0f, -1.0f, -1.0f,    1.0f,1.0f,1.0f,       0.0f, 0.0f,
    -1.0f, -1.0f, 1.0f,        1.0f,1.0f,1.0f,       1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,        1.0f,1.0f,1.0f,       1.0f, 1.0f,
                                                  
    -1.0f, 1.0f, -1.0f,        1.0f,1.0f,1.0f,       0.0f, 1.0f, //top qd
    -1.0f, 1.0f, 1.0f,        1.0f,1.0f,1.0f,       0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,        1.0f,1.0f,1.0f,       1.0f, 0.0f,
    1.0f, 1.0f, -1.0f,        1.0f,1.0f,1.0f,       1.0f, 1.0f,
                                                  
    -1.0f, -1.0f, 1.0f,        1.0f,1.0f,1.0f,       0.0f, 1.0f, //btm qd
    -1.0f, -1.0f, -1.0f,    1.0f,1.0f,1.0f,       0.0f, 0.0f,
    1.0f, -1.0f, -1.0f,        1.0f,1.0f,1.0f,       1.0f, 0.0f,
    1.0f, -1.0f, 1.0f,        1.0f,1.0f,1.0f,       1.0f, 1.0f
    };                                            

    // Define indices for the triangle vertices
    GLuint m_indices[] = {
        0,  1,  2,
        0,  2,  3,
        4,  5,  6,
        4,  6,  7,
        8,  9,  10,
        8,  10, 11,
        12, 13, 14,
        12, 14, 15,
        16, 17, 18,
        16, 18, 19,
        20, 21, 22,
        20, 22, 23
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

void CSquare::Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime)
{
    // bind program and VAO
    glUseProgram(_program);
    glBindVertexArray(VAO);

    //// send variables to shader via uniform
    //GLint ProjectionMatLoc = glGetUniformLocation(_program, "ProjectionMat");
    //glUniformMatrix4fv(ProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(_projMat));
    //GLint ViewMatLoc = glGetUniformLocation(_program, "ViewMat");
    //glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(_viewMat));
    //Model matrix
    GLint ModelMatrix = glGetUniformLocation(_program, "QuadModelMat");
    glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(_matrix));

    // send variables to the shader with uniform for color change
    GLint CurrentTimeLoc = glGetUniformLocation(_program, "CurrentTime");
    glUniform1f(CurrentTimeLoc, CurrentTime + 1);

    // Activate and bind the textures
    // texture 1
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glUniform1i(glGetUniformLocation(_program, "Texture0"), 0);

    // set the filtering and mipmap parameters for this texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // render
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}