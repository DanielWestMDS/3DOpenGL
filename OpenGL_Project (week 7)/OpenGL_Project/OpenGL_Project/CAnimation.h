#pragma once
#include "CSquare.h"
//#include <glew.h>
//#include <glfw3.h>
//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
//#include <gtc/type_ptr.hpp>
//#include <stb_image.h>

class CAnimation {
public:
    CAnimation();
    void Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime, glm::mat4 _projMat, glm::mat4 _viewMat);

private:
    GLuint VBO, VAO, EBO;
};