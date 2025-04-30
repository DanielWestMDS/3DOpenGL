#include "CPlayer.h"

CPlayer::CPlayer(std::string _filePath, GLint _program, GLint _texture, glm::vec3 _position, rp3d::PhysicsWorld* _physicsWorld, rp3d::PhysicsCommon& _physicsCommon) : CObject(_filePath, _program, _texture, _position, _physicsWorld, _physicsCommon)
{

}

CPlayer::~CPlayer()
{
	// only inherited destructor necessary
}


