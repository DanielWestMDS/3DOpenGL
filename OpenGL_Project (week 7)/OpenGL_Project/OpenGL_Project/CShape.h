#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>

class CShape {
public:
    CShape();
    void Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime, glm::mat4 _projMat, glm::mat4 _viewMat, glm::vec3 _secondColor);

    inline GLuint GetVAO() { return VAO; };
private:
    GLuint VBO, VAO, EBO;
};