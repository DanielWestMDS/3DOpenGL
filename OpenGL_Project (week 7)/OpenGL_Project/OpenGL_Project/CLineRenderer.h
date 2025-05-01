/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2025 Media Design School
File Name : CLineRenderer.h
Description : Class used for rendering collision
Author : Daniel West
Mail : daniel.west@mds.ac.nz
**************************************************************************/

#pragma once
#include <vector>
#include <glm/glm.hpp>

class CLineRenderer
{
private:
    unsigned int shaderProgram;
    unsigned int VBO, VAO;
    std::vector<glm::vec3> vertices;

public:
    CLineRenderer();
    ~CLineRenderer();

    void SetLines(const std::vector<std::pair<glm::vec3, glm::vec3>>& lines);
    void Draw(const glm::mat4& view, const glm::mat4& projection);
};
