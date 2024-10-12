// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CLightManager.cpp
// Description : Manages point lights and directional light for the shader
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#include "CLightManager.h"

CLightManager::CLightManager()
{
    // lights
    AmbientStrength = 0.3;
    AmbientColor = glm::vec3(1.0f, 1.0f, 1.0f);

    std::vector<PointLight> m_PointLights;

    // directional light
    m_DirectionLight.Direction = glm::vec3(-1.0f, -1.0f, 0.0f);
    m_DirectionLight.Color = glm::vec3(0.3f, 0.3f, 0.3f);
    m_DirectionLight.SpecularStrength = 0.0f;

    m_ShadowDir = glm::vec3(10.0f, -20.0f, -20.0f);

    m_ShadowProjMat = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 1000.0f);
    m_ShadowViewMat = glm::lookAt((m_ShadowDir) * m_fDirScalar, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_VPMat = m_ShadowProjMat * m_ShadowViewMat;
}

CLightManager::~CLightManager()
{
}

void CLightManager::AddPointLight(const glm::vec3& position, const glm::vec3& color, float specularStrength, float _Constant, float _Linear, float _Exponent)
{
    if (m_PointLights.size() < MAX_POINT_LIGHTS)
    {
        PointLight pointLight;
        pointLight.Position = position;
        pointLight.Color = color;
        pointLight.SpecularStrength = specularStrength;
        pointLight.AttenuationConstant = _Constant;
        pointLight.AttenuationExponent = _Exponent;
        pointLight.AttenuationLinear = _Linear;
        m_PointLights.push_back(pointLight);
    }
    else
    {
        std::cerr << "Maximum number of point lights reached." << std::endl;
    }
}

void CLightManager::UpdateShader(GLuint program, bool _pointlight)
{
    glUseProgram(program);

    for (unsigned int i = 0; i < m_PointLights.size(); i++)
    {
        std::string baseName = "PointLightArray[" + std::to_string(i) + "]";
        glUniform3fv(glGetUniformLocation(program, (baseName + ".Position").c_str()), 1, &m_PointLights[i].Position[0]);
        glUniform3fv(glGetUniformLocation(program, (baseName + ".Color").c_str()), 1, &m_PointLights[i].Color[0]);
        glUniform1f(glGetUniformLocation(program, (baseName + ".SpecularStrength").c_str()), m_PointLights[i].SpecularStrength);
        glUniform1f(glGetUniformLocation(program, (baseName + ".AttenuationConstant").c_str()), m_PointLights[i].AttenuationConstant);
        glUniform1f(glGetUniformLocation(program, (baseName + ".AttenuationLinear").c_str()), m_PointLights[i].AttenuationLinear);
        glUniform1f(glGetUniformLocation(program, (baseName + ".AttenuationExponent").c_str()), m_PointLights[i].AttenuationExponent);

    }
    // pass the boolean _pointlight into the shader
    glUniform1i(glGetUniformLocation(program, "bPointLightOn"), _pointlight);

    // pass point light count into shader
    glUniform1i(glGetUniformLocation(program, "PointLightCount"), m_PointLights.size());

    std::string sDirection = "DirectionLight";
    glUniform3fv(glGetUniformLocation(program, (sDirection + ".Direction").c_str()), 1, &m_DirectionLight.Direction[0]);
    glUniform3fv(glGetUniformLocation(program, (sDirection + ".Color").c_str()), 1, &m_DirectionLight.Color[0]);
    glUniform1f(glGetUniformLocation(program, (sDirection + ".SpecularStrength").c_str()), m_DirectionLight.SpecularStrength);
}
