#pragma once
#include "CCamera.h"

class CButton {
public:
    CButton();
    void Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime, glm::mat4 _projMat, glm::mat4 _viewMat);

private:
    GLuint VBO, VAO, EBO;
};

