// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CLightManager.h
// Description : Manages point lights for the shader
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#pragma once

#include "CCamera.h"
#include <vector>
#include <string>

#define MAX_POINT_LIGHTS 4

struct PointLight
{
    glm::vec3 Position;
    glm::vec3 Color;
    float SpecularStrength;
};

class CLightManager
{
public:
    CLightManager();
    ~CLightManager();

    void AddPointLight(const glm::vec3& position, const glm::vec3& color, float specularStrength);
    void UpdateShader(GLuint program);

private:
    std::vector<PointLight> m_PointLights;
};
