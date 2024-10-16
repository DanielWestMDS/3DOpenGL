#include "CParticleSystem.h"

CParticleSystem::CParticleSystem(CCamera* _camera, GLuint _renderProgram, GLuint _computeProgram, glm::vec3 _origin)
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
}

CParticleSystem::~CParticleSystem()
{
	glDeleteBuffers(1, &VBO_PositionLife);
	glDeleteBuffers(1, &VBO_Velocity);
	glDeleteVertexArrays(1, &VAO);
}

void CParticleSystem::Update(float _dt)
{
	glm::vec3 Gravity = glm::vec3(0.0f, -9.8f, 0.0f) * _dt;
	m_VelocityLifeChange = glm::vec4(Gravity, _dt); // gravity and delta time for velocity update
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

	// bind the storage buffers for compute shader manipulations
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO_PositionLife);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, VBO_Velocity);

	// dispatch compute shader
	glDispatchCompute(m_iGroupCountX, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

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
