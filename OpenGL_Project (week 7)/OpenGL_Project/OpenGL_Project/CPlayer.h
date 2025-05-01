/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2025 Media Design School
File Name : CPlayer.h
Description : Child of Object class. first person player that the camera is attached to
Author : Daniel West
Mail : daniel.west@mds.ac.nz
**************************************************************************/

#pragma once
#include "CObject.h"

class CPlayer : public CObject
{
public:

	CPlayer(std::string FilePath, GLint _program, GLint _texture, glm::vec3 position, rp3d::PhysicsWorld* physicsWorld, rp3d::PhysicsCommon& physicsCommon);

	~CPlayer();

	json ToJson() const override;

	static CPlayer* FromJson(const json& j, rp3d::PhysicsWorld* _physicsWorld, rp3d::PhysicsCommon& _physicsCommon);

	void Update(float dt, GLFWwindow* _window) override;

protected:

private:




};

