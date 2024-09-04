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

    /// <summary>
    /// load a skybox from the filepath
    /// </summary>
    /// <param name="sFaces"></param>
    /// <param name="_FilePath"></param>
    /// <param name="_program"></param>
    CSkyBox(std::vector<std::string> sFaces, std::string _FilePath, GLint _program);

    /// <summary>
    /// destructor
    /// </summary>
    ~CSkyBox();

    /// <summary>
    /// Updates matrices so background appears correctly when the camera moves
    /// </summary>
    /// <param name="_viewMat"></param>
    /// <param name="_projMat"></param>
    void Update(glm::mat4 _viewMat, glm::mat4 _projMat);

    /// <summary>
    /// Passes textures and matrices into the shader. Binds VAO
    /// </summary>
    void Render() override;

private:

    /// <summary>
    /// Loads imagedata into an opengl cubemap
    /// </summary>
    /// <param name="faces"></param>
    /// <returns></returns>
    GLuint LoadCubeMap(std::vector<std::string> faces);

    // texture
    GLuint m_cubeMapTexture;
};
