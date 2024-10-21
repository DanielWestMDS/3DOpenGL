#include "CParticleSystem.h"

CParticleSystem::CParticleSystem(CCamera* _camera, GLuint _renderProgram, GLuint _computeProgram, glm::vec3 _origin, glm::vec3 _color)
{
    this->m_ActiveCamera = _camera;
    this->m_Program_Render = _renderProgram;
    this->m_Program_Compute = _computeProgram;
    this->m_EmitterOrigin = _origin;

    m_iGroupCountX = 1000;
    m_iWorkGroupSizeX = 128;
    m_iNumParticles = m_iWorkGroupSizeX * m_iGroupCountX;

    // Store position and lifespan information
    glGenBuffers(1, &VBO_PositionLife);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO_PositionLife);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
        sizeof(glm::vec4) * m_iNumParticles,
        NULL,
        GL_DYNAMIC_DRAW);

    // store velocity info
    glGenBuffers(1, &VBO_Velocity);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO_Velocity);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
        sizeof(glm::vec4) * m_iNumParticles,
        NULL,
        GL_DYNAMIC_DRAW);

    // VAO for standard pipeline render
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO for the standard pipeline render
    glBindBuffer(GL_ARRAY_BUFFER, VBO_PositionLife);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);
    glEnableVertexAttribArray(0);

    // unbinding
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_iSeedLife = (int)rand();
    m_iSeedX = (int)rand();
    m_iSeedY = (int)rand();
    m_iSeedZ = (int)rand();
    m_VelocityLifeChange = glm::vec4(0.0f);

    m_Color = _color;
    m_bFireworkActive = false;
    m_bKeyPressed = false;
}

CParticleSystem::~CParticleSystem()
{
    glDeleteBuffers(1, &VBO_PositionLife);
    glDeleteBuffers(1, &VBO_Velocity);
    glDeleteVertexArrays(1, &VAO);
}

// Function to trigger the firework
void CParticleSystem::TriggerFirework()
{
    m_bFireworkActive = true;
    m_iSeedLife = (int)rand();
    m_iSeedX = (int)rand();
    m_iSeedY = (int)rand();
    m_iSeedZ = (int)rand();
}

// Check if all particles have expired (i.e., lifespan <= 0.0)
bool CParticleSystem::CheckAllParticlesExpired()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO_PositionLife);
    glm::vec4* positions = (glm::vec4*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

    bool allExpired = true;
    for (int i = 0; i < m_iNumParticles; ++i)
    {
        if (positions[i].w > 0.0f)  // any particle still has life left
        {
            allExpired = false;
            break;
        }
    }

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    return allExpired;
}

void CParticleSystem::Update(float _dt, bool _keypressed)
{
    glm::vec3 Gravity = glm::vec3(0.0f, -9.8f, 0.0f) * _dt;
    m_VelocityLifeChange = glm::vec4(Gravity, _dt);

    // Check if the 'F' key is pressed (pseudo-code, replace with actual input handling logic)
    if (_keypressed && !m_bFireworkActive && !m_bKeyPressed)
    {
        // avoid multiple triggers on the same press
        m_bKeyPressed = true;
        TriggerFirework();
    }

    if (m_bFireworkActive && CheckAllParticlesExpired())
    {
        m_bFireworkActive = false;
    }
}

void CParticleSystem::Render()
{
    glUseProgram(m_Program_Compute);
    glBindVertexArray(VAO);

    // pass in seeds
    glUniform1i(glGetUniformLocation(m_Program_Compute, "SeedLife"), m_iSeedLife);
    glUniform1i(glGetUniformLocation(m_Program_Compute, "SeedX"), m_iSeedX);
    glUniform1i(glGetUniformLocation(m_Program_Compute, "SeedY"), m_iSeedY);
    glUniform1i(glGetUniformLocation(m_Program_Compute, "SeedZ"), m_iSeedZ);

    // pass in velocity life change
    glUniform4fv(glGetUniformLocation(m_Program_Compute, "VelocityLifeChange"), 1, glm::value_ptr(m_VelocityLifeChange));

    // pass in emitter origin
    glUniform3fv(glGetUniformLocation(m_Program_Compute, "EmitterOrigin"), 1, glm::value_ptr(m_EmitterOrigin));

    // pass the boolean _pointlight into the shader
    glUniform1i(glGetUniformLocation(m_Program_Compute, "bExplode"), m_bKeyPressed);

    if (m_bKeyPressed) m_bKeyPressed = false; // reset key press flag

    // bind the storage buffers for compute shader manipulations
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO_PositionLife);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, VBO_Velocity);

    // dispatch compute shader
    glDispatchCompute(m_iGroupCountX, 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glUseProgram(m_Program_Render);

    // update color
    GLint colorLocation = glGetUniformLocation(m_Program_Render, "Color");
    glUniform3fv(colorLocation, 1, glm::value_ptr(m_Color));

    // render pass
    glUseProgram(m_Program_Render);
    glBindVertexArray(VAO);
    glUniformMatrix4fv(glGetUniformLocation(m_Program_Render, "VP"), 1, GL_FALSE, glm::value_ptr(m_ActiveCamera->GetVP()));
    glDrawArrays(GL_POINTS, 0, m_iNumParticles);

    // unbinding
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
