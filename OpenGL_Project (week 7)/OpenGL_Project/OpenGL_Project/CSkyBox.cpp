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

CSkyBox::CSkyBox(std::vector<std::string> sFaces, std::string _FilePath) : CModel(_FilePath)
{
    m_cubeMapTexture = LoadCubeMap(sFaces);
}

CSkyBox::~CSkyBox()
{
    glDeleteTextures(1, &m_cubeMapTexture);
}

void CSkyBox::Render(GLint _program, glm::mat4 _viewMat, glm::mat4 _projMat)
{
    glDepthMask(GL_FALSE);

    // bind program and VAO
    glUseProgram(_program);
    glBindVertexArray(VAO);

    // remove translation from the view matrix
    glm::mat4 viewMat = glm::mat4(glm::mat3(_viewMat));

    // view and projection matrices
    GLint viewLoc = glGetUniformLocation(_program, "view");
    GLint projLoc = glGetUniformLocation(_program, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(_projMat));

    // bind the cube map texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexture);
    glUniform1i(glGetUniformLocation(_program, "skybox"), 0);

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
