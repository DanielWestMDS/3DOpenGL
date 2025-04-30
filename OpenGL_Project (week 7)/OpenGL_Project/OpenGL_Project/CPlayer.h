#pragma once
#include "CObject.h"

class CPlayer : public CObject
{
public:

	CPlayer(std::string FilePath, GLint _program, GLint _texture, glm::vec3 position, rp3d::PhysicsWorld* physicsWorld, rp3d::PhysicsCommon& physicsCommon, CCamera& _camera, GLFWwindow* _Window);

	~CPlayer();

	void Update(float dt) override;

protected:

	CCamera& m_Camera;

private:


};

