#include "CPlayer.h"

CPlayer::CPlayer(std::string _filePath, GLint _program, GLint _texture, glm::vec3 _position, rp3d::PhysicsWorld* _physicsWorld, rp3d::PhysicsCommon& _physicsCommon, CCamera& _camera, GLFWwindow* _Window) : m_Camera(_camera), CObject(_filePath, _program, _texture, _position, _physicsWorld, _physicsCommon)
{
}

CPlayer::~CPlayer()
{
	// only inherited destructor necessary
}

void CPlayer::Update(float dt)
{
	// retain object update functionality
	CObject::Update(dt);

	// set the camera to the player's position
	m_Camera.SetPosition(GetPosition());

	// movement

}


