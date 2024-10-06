#include "CParticleSystem.h"

CParticleSystem::CParticleSystem(CCamera* _camera, GLuint _renderProgram, GLuint _computeProgram, glm::vec3 _origin)
{
	this->m_ActiveCamera = _camera;
	this->m_Program_Render = _renderProgram;
	this->m_Program_Compute = _computeProgram;
	this->m_EmitterOrigin = _origin;

	int m_iSeedLife = 1;
	int m_iSeedX = 2;
	int m_iSeedY = 3;
	int m_iSeedZ = 4;

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
}

CParticleSystem::~CParticleSystem()
{
}

void CParticleSystem::Update(float _dt)
{
	glm::vec3 Gravity = glm::vec3(0.0f, -9.8f, 0.0f) * _dt;
	m_VelocityLifeChange = glm::vec4(Gravity, _dt);
}

void CParticleSystem::Render()
{
	// bind program
	glUseProgram(m_Program_Compute);
	glBindVertexArray(VAO);

	// pass in seeds
	GLint SeedLifeLoc = glGetUniformLocation(m_Program_Compute, "SeedLife");
	glUniform1i(SeedLifeLoc, m_iSeedLife);

	GLint SeedXLoc = glGetUniformLocation(m_Program_Compute, "SeedX");
	glUniform1i(SeedXLoc, m_iSeedX);

	GLint SeedYLoc = glGetUniformLocation(m_Program_Compute, "SeedY");
	glUniform1i(SeedYLoc, m_iSeedY);

	GLint SeedZLoc = glGetUniformLocation(m_Program_Compute, "SeedZ");
	glUniform1i(SeedZLoc, m_iSeedZ);

	// pass in velocity life change
	GLint VelocityLifeLoc = glGetUniformLocation(m_Program_Compute, "VelocityLifeChange");
	glUniform4fv(VelocityLifeLoc, 1, glm::value_ptr(m_VelocityLifeChange));

	// pass in emitter origin
	GLint EmitterLoc = glGetUniformLocation(m_Program_Compute, "EmitterOrigin");
	glUniform3fv(EmitterLoc, 1, glm::value_ptr(m_EmitterOrigin));

	// bind the storage buffers for compute shader manipulations
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO_PositionLife);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, VBO_Velocity);

	//set the compute shader going
	glDispatchCompute(	m_iGroupCountX, // X
						1,				// Y
						1);				// Z

	// wait for the compute shader completion and sync all threads
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	// second pass - standard render pipeline
	glUseProgram(m_Program_Render);

	// uniforms
	glUniformMatrix4fv(glGetUniformLocation(m_Program_Render, "VP"),
		1, GL_FALSE, glm::value_ptr(m_ActiveCamera->GetVP()));

	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, m_iNumParticles);

	// unbinding
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
