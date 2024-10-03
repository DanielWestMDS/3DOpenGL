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

/// <summary>
/// Information for point lights to be passed into the lighting program
/// </summary>
struct PointLight
{
    glm::vec3 Position;
    glm::vec3 Color;
    float SpecularStrength;

    float AttenuationConstant;
    float AttenuationLinear;
    float AttenuationExponent;
};

/// <summary>
/// information for the directional lighting to be passed into the lighting program
/// </summary>
struct DirectionalLight
{
    glm::vec3 Direction;
    glm::vec3 Color;
    float SpecularStrength;
};

class CLightManager
{
public:

    /// <summary>
    /// constructor. Initialises ambient and directional light
    /// </summary>
    CLightManager();

    /// <summary>
    /// destructor
    /// </summary>
    ~CLightManager();

    /// <summary>
    /// Adds a point light into pointlights vector. 
    /// </summary>
    /// <param name="position"></param>
    /// <param name="color"></param>
    /// <param name="specularStrength"></param>
    /// <param name="_Constant"></param>
    /// <param name="_Linear"></param>
    /// <param name="_Exponent"></param>
    void AddPointLight(const glm::vec3& position, const glm::vec3& color, float specularStrength, float _Constant, float _Linear, float _Exponent);

    /// <summary>
    /// Updates the shader with point light info. Also updates boolean for if pointlights are active or not.
    /// </summary>
    /// <param name="program"></param>
    /// <param name="_bPointLight"></param>
    void UpdateShader(GLuint program, bool _bPointLight);
    
    /// <summary>
    /// Returns the VP matrix for shadow mapping
    /// </summary>
    /// <returns></returns>
    glm::mat4 GetVP() { return m_VPMat; };

private:
    // lights
    float AmbientStrength;
    glm::vec3 AmbientColor;
    static const int MAX_POINT_LIGHTS = 10;

    // vector to store all the point lights
    std::vector<PointLight> m_PointLights;

    // direction light (only one needed)
    DirectionalLight m_DirectionLight;

    glm::mat4 m_LightProjMat = glm::ortho(-24.0f, -14.0f, -24.0f, -14.0f, 0.1f, 2000.0f);
    glm::mat4 m_LightViewMat;

    glm::mat4 m_VPMat;

    float m_fDirScalar = -1.0f * 500.0f;
};
