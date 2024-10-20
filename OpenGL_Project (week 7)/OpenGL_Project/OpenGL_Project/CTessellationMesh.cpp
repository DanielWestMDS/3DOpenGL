#include "CTessellationMesh.h"
#include "vector"

CTessellationMesh::CTessellationMesh(GLuint _Texture)
{
	std::vector<VertexPoint> Vertices;
	Vertices.push_back(glm::vec3(glm::sin(glm::radians(0.0f)), glm::cos(glm::radians(0.0f)), 0.0f));
	Vertices.push_back(glm::vec3(glm::sin(glm::radians(240.0f)), glm::cos(glm::radians(240.0f)), 0.0f));
	Vertices.push_back(glm::vec3(glm::sin(glm::radians(120.0f)), glm::cos(glm::radians(120.0f)), 0.0f));

	// sets quad patch
	glPatchParameteri(GL_PATCH_VERTICES, 3);

    //glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, glm::value_ptr(glm::vec4(4.0f, 4.0f, 4.0f, 4.0f)));
    //glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, glm::value_ptr(glm::vec2(4.0f, 4.0f)));

	m_DrawCount = (unsigned int)Vertices.size();
	m_DrawType = GL_PATCHES;

    // vertex array and buffers
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPoint) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

    // attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)(offsetof(VertexPoint, VertexPoint::position)));
    glEnableVertexAttribArray(0);

    // Unbind the VAO to avoid accidental modifications
    glBindVertexArray(0);

    // set texture
    m_Texture = _Texture;
}

CTessellationMesh::~CTessellationMesh()
{
}

void CTessellationMesh::Render(GLint _program, glm::mat4 _PVM)
{
    glUseProgram(_program);
    glBindVertexArray(VAO);

    // pass in texture
    GLint PositionTexture = glGetUniformLocation(_program, "Texture_Position");
    glUniform1i(PositionTexture, 0);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    // pass in PVM
    GLint ModelMatrix = glGetUniformLocation(_program, "PVM");
    glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(_PVM));

    glDrawArrays(m_DrawType, 0, m_DrawCount);
    glBindVertexArray(0);
}
