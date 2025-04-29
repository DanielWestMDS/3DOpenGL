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
