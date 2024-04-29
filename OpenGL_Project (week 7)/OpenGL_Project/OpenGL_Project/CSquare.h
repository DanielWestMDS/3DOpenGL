#pragma once
#include "CShape.h"
//#include <glew.h>
//#include <glfw3.h>
//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
//#include <gtc/type_ptr.hpp>
//#include <stb_image.h>

class CSquare {
public:
    CSquare();
    void Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime);

private:
    GLuint VBO, VAO, EBO;
};