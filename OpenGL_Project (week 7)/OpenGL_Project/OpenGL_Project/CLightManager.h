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

//#define MAX_POINT_LIGHTS 4

struct PointLight
{
    glm::vec3 Position;
    glm::vec3 Color;
    float SpecularStrength;

    float AttenuationConstant;
    float AttenuationLinear;
    float AttenuationExponent;
};

class CLightManager
{
public:
    CLightManager();
    ~CLightManager();

    void AddPointLight(const glm::vec3& position, const glm::vec3& color, float specularStrength, float _Constant, float _Linear, float _Exponent);
    void UpdateShader(GLuint program);

private:
    // lights
    float AmbientStrength;
    glm::vec3 AmbientColor;
    static const int MAX_POINT_LIGHTS = 10;
    PointLight PointLightArray[MAX_POINT_LIGHTS];
    unsigned int PointLightCount;

    std::vector<PointLight> m_PointLights;
};
