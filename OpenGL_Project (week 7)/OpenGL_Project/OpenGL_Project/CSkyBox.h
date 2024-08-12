// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CSkyBox.h
// Description : Skybox class inheriting from CModel
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#pragma once

#include "CModel.h"

class CSkyBox : public CModel
{
public:
    // load a skybox from the filepath
    CSkyBox(std::vector<std::string> sFaces, std::string _FilePath, GLint _program);
    ~CSkyBox();

    void Update(glm::mat4 _viewMat, glm::mat4 _projMat);
    void Render() override;

private:

    GLuint LoadCubeMap(std::vector<std::string> faces);

    // texture
    GLuint m_cubeMapTexture;
};
