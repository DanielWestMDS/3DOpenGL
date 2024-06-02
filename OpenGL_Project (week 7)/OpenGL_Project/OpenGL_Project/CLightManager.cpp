// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CLightManager.cpp
// Description : Manages point lights for the shader
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#include "CLightManager.h"

CLightManager::CLightManager()
{
}

CLightManager::~CLightManager()
{
}

void CLightManager::AddPointLight(const glm::vec3& position, const glm::vec3& color, float specularStrength)
{
    if (m_PointLights.size() < MAX_POINT_LIGHTS)
    {
        PointLight pointLight;
        pointLight.Position = position;
        pointLight.Color = color;
        pointLight.SpecularStrength = specularStrength;
        m_PointLights.push_back(pointLight);
    }
    else
    {
        std::cerr << "Maximum number of point lights reached." << std::endl;
    }
}

void CLightManager::UpdateShader(GLuint program)
{
    glUseProgram(program);

    for (unsigned int i = 0; i < m_PointLights.size(); ++i)
    {
        std::string baseName = "PointLightArray[" + std::to_string(i) + "]";
        glUniform3fv(glGetUniformLocation(program, (baseName + ".Position").c_str()), 1, &m_PointLights[i].Position[0]);
        glUniform3fv(glGetUniformLocation(program, (baseName + ".Color").c_str()), 1, &m_PointLights[i].Color[0]);
        glUniform1f(glGetUniformLocation(program, (baseName + ".SpecularStrength").c_str()), m_PointLights[i].SpecularStrength);
    }

    glUniform1i(glGetUniformLocation(program, "PointLightCount"), m_PointLights.size());
}
