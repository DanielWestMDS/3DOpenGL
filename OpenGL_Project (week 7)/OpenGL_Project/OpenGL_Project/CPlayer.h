#pragma once
#include "CObject.h"

class CPlayer : public CObject
{
public:

	CPlayer(std::string FilePath, GLint _program, GLint _texture, glm::vec3 position, rp3d::PhysicsWorld* physicsWorld, rp3d::PhysicsCommon& physicsCommon);

	~CPlayer();

protected:

private:


};

