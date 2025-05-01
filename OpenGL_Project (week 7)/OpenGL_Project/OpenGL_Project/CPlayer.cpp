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

CPlayer* CPlayer::FromJson(const json& j, rp3d::PhysicsWorld* _physicsWorld, rp3d::PhysicsCommon& _physicsCommon)
{
	std::string filePath = j["modelPath"];
	glm::vec3 position(j["position"][0], j["position"][1], j["position"][2]);

	// collision dimensions
	glm::vec3 shapeDimensions(1.0f); // default
	if (j.contains("collisionDimensions"))
	{
		shapeDimensions = glm::vec3(j["collisionDimensions"][0],
			j["collisionDimensions"][1],
			j["collisionDimensions"][2]);
	}

	// collision shape
	CollisionShapeType shapeType = CollisionShapeType::BOX;
	if (j.contains("shapeType"))
	{
		shapeType = j["shapeType"];
	}

	// dawg I hope this works
	GLint program = (GLint)j["program"];

	GLint texture = (GLint)j["texture"];

	float fScale = j["scale"];

	// create the object with parameters from json
	CPlayer* LoadedPlayer = new CPlayer(filePath, program, texture, position, _physicsWorld, _physicsCommon);

	// update rotation and scale
	LoadedPlayer->SetRotation(glm::vec3(j["rotation"][0], j["rotation"][1], j["rotation"][2]));
	LoadedPlayer->SetScale(fScale);
	LoadedPlayer->SetPhysicsBodyType(j["bodyType"]);
	LoadedPlayer->SetCollisionDimensions(shapeDimensions);

	return LoadedPlayer;
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


