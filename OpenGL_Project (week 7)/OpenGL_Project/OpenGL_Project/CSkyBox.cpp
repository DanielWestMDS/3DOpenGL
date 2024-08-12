// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CSkyBox.cpp
// Description : Skybox class inheriting from CModel
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#include "CSkyBox.h"
#include <stb_image.h>

CSkyBox::CSkyBox(std::vector<std::string> sFaces, std::string _FilePath, GLint _program) : CModel(_FilePath, _program, 0, glm::mat4())
{
    m_cubeMapTexture = LoadCubeMap(sFaces);
}

CSkyBox::~CSkyBox()
{
    glDeleteTextures(1, &m_cubeMapTexture);
}

void CSkyBox::Update(glm::mat4 _viewMat, glm::mat4 _projMat)
{
    m_viewMat = _viewMat;
    m_projMat = _projMat;
}

void CSkyBox::Render()
{
    glDepthMask(GL_FALSE);

    // bind program and VAO
    glUseProgram(m_program);
    glBindVertexArray(VAO);

    // remove translation from the view matrix
    glm::mat4 viewMat = glm::mat4(glm::mat3(m_viewMat));

    // view and projection matrices
    GLint viewLoc = glGetUniformLocation(m_program, "view");
    GLint projLoc = glGetUniformLocation(m_program, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_projMat));

    // bind the cube map texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexture);
    glUniform1i(glGetUniformLocation(m_program, "skybox"), 0);

    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);

    // Unbind
    glBindVertexArray(0);
    glUseProgram(0);

    // enable depth mask
    glDepthMask(GL_TRUE);
}

GLuint CSkyBox::LoadCubeMap(std::vector<std::string> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    stbi_set_flip_vertically_on_load(false);

    // storage variables
    int iWidth, iHeight, iComponents;

    for (int i = 0; i < 6; i++)
    {
        // load each image seperately
        unsigned char* imageData = stbi_load(faces[i].c_str(), &iWidth, &iHeight, &iComponents, 0);

        // cubemap should be full color
        GLint LoadedComponents = (iComponents == 4) ? GL_RGBA : GL_RGB;

        if (imageData)
        {
            // enum for texture side
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, LoadedComponents, iWidth, iHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, imageData);
            stbi_image_free(imageData);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(imageData);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}
