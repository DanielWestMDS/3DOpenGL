// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CButton.h
// Description : header file for button. Encapsulates the VBO, VAO and EBO in private and runs the Render function in main. 
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#pragma once
#include "CCamera.h"

class CButton {
public:
    CButton();
    void Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime, glm::mat4 _projMat, glm::mat4 _viewMat);

private:
    GLuint VBO, VAO, EBO;
};

