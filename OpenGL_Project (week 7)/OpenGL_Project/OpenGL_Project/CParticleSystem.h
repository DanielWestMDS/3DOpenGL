#pragma once
#include "CCamera.h"

class CParticleSystem
{
public:
	/// <summary>
	/// constructor. Sets member variables, shaderstorage buffer objects, and the compute to standard render link. 
	/// </summary>
	/// <param name="_camera"></param>
	/// <param name="_renderProgram"></param>
	/// <param name="_computeProgram"></param>
	/// <param name="_origin"></param>
	CParticleSystem(CCamera* _camera, GLuint _renderProgram, GLuint _computeProgram, glm::vec3 _origin, glm::vec4 _color);

	/// <summary>
	/// Destructor
	/// </summary>
	~CParticleSystem();

	void Update(float _dt, bool _keypressed);

	void Render();

private:
	// for triggering fireworks when f pressed
	void TriggerFirework();
	bool CheckAllParticlesExpired();

	CCamera* m_ActiveCamera;
	GLuint m_Program_Render;
	GLuint m_Program_Compute;

	GLuint VAO;
	// xyz is position and w is remaining life
	GLuint VBO_PositionLife;
	// xyz tracks each particle velocity. w unused
	GLuint VBO_Velocity;

	// initial spawn location for all particles
	glm::vec3 m_EmitterOrigin;
	// changes to affect each particle during the compute shader run
	glm::vec4 m_VelocityLifeChange;

	// amount of groups to create (1D)
	int m_iGroupCountX;
	// match compute shader for number of threads in work group
	int m_iWorkGroupSizeX;
	// overall number of particles
	int m_iNumParticles;

	// random seeds
	int m_iSeedLife;
	int m_iSeedX;
	int m_iSeedY;
	int m_iSeedZ;

	// each firework a different colour
	glm::vec4 m_Color;
	bool m_bFireworkActive;
	bool m_bKeyPressed;
};

