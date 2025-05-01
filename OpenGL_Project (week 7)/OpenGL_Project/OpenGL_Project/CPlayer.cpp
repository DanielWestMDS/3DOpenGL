#include "CPlayer.h"
#include "CEditorMode.h"

CPlayer::CPlayer(std::string _filePath, GLint _program, GLint _texture, glm::vec3 _position, rp3d::PhysicsWorld* _physicsWorld, rp3d::PhysicsCommon& _physicsCommon) : CObject(_filePath, _program, _texture, _position, _physicsWorld, _physicsCommon)
{
	CEditorMode& Editor = CEditorMode::GetInstance();

	Editor.SetPlayer(this);
}

CPlayer::~CPlayer()
{
	CEditorMode& Editor = CEditorMode::GetInstance();

	// set player to null
	Editor.SetPlayer(nullptr);
}

json CPlayer::ToJson() const
{
	// normal data
	json j = CObject::ToJson(); 

	// player specific data
	j["type"] = "CPlayer"; 

	// I didn't have time but I would put health and stuff here

	return j;
}

void CPlayer::Update(float dt, GLFWwindow* _window)
{
	// retain object update functionality
	CObject::Update(dt, _window);

	CEditorMode& Editor = CEditorMode::GetInstance();

	// only do the rest of the stuff if in game
	if (Editor.GetInEditor()) return;

	// set the camera to the player's position
	Editor.GetCamera()->SetPosition(GetPosition());

	// movement
		// go right with D key
	if (glfwGetKey(_window, GLFW_KEY_D))
	{
		m_RigidBody->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(-Editor.GetCamera()->GetRight().x,
			-Editor.GetCamera()->GetRight().y,
			-Editor.GetCamera()->GetRight().z));
	}

	// go left with A key
	if (glfwGetKey(_window, GLFW_KEY_A))
	{
		m_RigidBody->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(Editor.GetCamera()->GetRight().x,
			Editor.GetCamera()->GetRight().y,
			Editor.GetCamera()->GetRight().z));
	}

	// go Backwards with S
	if (glfwGetKey(_window, GLFW_KEY_S))
	{
		m_RigidBody->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(Editor.GetCamera()->GetForward().x,
			Editor.GetCamera()->GetForward().y,
			Editor.GetCamera()->GetForward().z));
	}

	// go forwards with W
	if (glfwGetKey(_window, GLFW_KEY_W))
	{
		m_RigidBody->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(-Editor.GetCamera()->GetForward().x,
			-Editor.GetCamera()->GetForward().y,
			-Editor.GetCamera()->GetForward().z));
	}
}


