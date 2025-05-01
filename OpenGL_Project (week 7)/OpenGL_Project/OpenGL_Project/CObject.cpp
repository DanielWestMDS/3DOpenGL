#include "CObject.h"
#include "CModel.h"
#include "CPlayer.h"
#include <iostream>

CObject::CObject(std::string _filePath, GLint _program, GLint _texture, glm::vec3 _position,
    rp3d::PhysicsWorld* physicsWorld, rp3d::PhysicsCommon& physicsCommon,
    CollisionShapeType shapeType, glm::vec3 shapeDimensions)
    : m_PhysicsWorld(physicsWorld), m_PhysicsCommon(&physicsCommon), m_CollisionDimensions(shapeDimensions)
{
    // Create the rigid body
    rp3d::Vector3 rpPosition(_position.x, _position.y, _position.z);
    rp3d::Transform transform(rpPosition, rp3d::Quaternion::identity());
    m_RigidBody = physicsWorld->createRigidBody(transform);

    m_Model = new CModel(_filePath, _program, _texture, _position);

    // Create collision shape
    CreateCollisionShape(shapeType);
    SetPosition(_position);
    SetGravityEnabled(false);
}

CObject::~CObject()
{
    if (m_RigidBody && m_PhysicsWorld)
    {
        if (m_Collider) 
        {
            m_RigidBody->removeCollider(m_Collider);
            m_Collider = nullptr;
        }

        if (m_CollisionShape) 
        {
            // destroy based on shape type
            switch (m_ShapeType) 
            {
            case CollisionShapeType::BOX:
                m_PhysicsCommon->destroyBoxShape(static_cast<rp3d::BoxShape*>(m_CollisionShape));
                break;
            case CollisionShapeType::SPHERE:
                m_PhysicsCommon->destroySphereShape(static_cast<rp3d::SphereShape*>(m_CollisionShape));
                break;
            case CollisionShapeType::CAPSULE:
                m_PhysicsCommon->destroyCapsuleShape(static_cast<rp3d::CapsuleShape*>(m_CollisionShape));
                break;
            }
            m_CollisionShape = nullptr;
        }

        m_PhysicsWorld->destroyRigidBody(m_RigidBody);
        m_RigidBody = nullptr;
    }

    delete m_Model;
    m_Model = nullptr;
}


void CObject::SetPhysicsBodyType(rp3d::BodyType _bodyType)
{
    m_RigidBody->setType(_bodyType);
}

rp3d::BodyType CObject::GetPhysicsBodyType()
{
    return m_RigidBody->getType();
}

void CObject::SetCollisionDraw(bool _bIsEnabled)
{
    m_RigidBody->setIsDebugEnabled(_bIsEnabled);
}

void CObject::CreateCollisionShape(CollisionShapeType shapeType)
{
    if (!m_RigidBody) return;
    if (GetScale() == 0) return;

    // Remove previous collider and shape
    if (m_Collider) 
    {
        m_RigidBody->removeCollider(m_Collider);
        m_Collider = nullptr;
    }

    if (m_CollisionShape) 
    {
        RemoveCollision();
    }

    // make sure to not divide 0
    if (m_CollisionDimensions.x != 0 && m_CollisionDimensions.y != 0 && m_CollisionDimensions.z != 0)
    {
        // Create new collision shape
        switch (shapeType)
        {
        case CollisionShapeType::BOX:
            m_CollisionShape = m_PhysicsCommon->createBoxShape(
                rp3d::Vector3(m_CollisionDimensions.x / 2.0f * GetScale(),
                    m_CollisionDimensions.y / 2.0f * GetScale(),
                    m_CollisionDimensions.z / 2.0f * GetScale()));
            break;

        case CollisionShapeType::SPHERE:
            m_CollisionShape = m_PhysicsCommon->createSphereShape(m_CollisionDimensions.x);
            break;

        case CollisionShapeType::CAPSULE:
            m_CollisionShape = m_PhysicsCommon->createCapsuleShape(m_CollisionDimensions.x, m_CollisionDimensions.y);
            break;
        }
    }

    m_ShapeType = shapeType;

    // Add collider to the rigid body
    m_Collider = m_RigidBody->addCollider(m_CollisionShape, rp3d::Transform::identity());

    // Update mass properties
    m_RigidBody->updateMassPropertiesFromColliders();
}

void CObject::Update(float dt, GLFWwindow* _window)
{
    if (m_RigidBody) 
    {
        // Sync model with physics body
        const rp3d::Transform& transform = m_RigidBody->getTransform();
        const rp3d::Vector3& position = transform.getPosition();
        m_Model->SetPosition(glm::vec3(position.x, position.y, position.z));

        // TODO: sync rotation
        //rp3d::Quaternion rotation = transform.getOrientation();
        //m_Model->SetRotation(glm::quat(rotation.w, rotation.x, rotation.y, rotation.z));
    }
}

void CObject::SetPosition(glm::vec3 position)
{
    if (m_RigidBody) 
    {
        rp3d::Vector3 rpPosition(position.x, position.y, position.z);
        rp3d::Transform transform(rpPosition, m_RigidBody->getTransform().getOrientation());
        m_RigidBody->setTransform(transform);
    }
    m_Model->SetPosition(position);
}

glm::vec3 CObject::GetPosition()
{
    return m_Model->GetPosition();
}

void CObject::SetRotation(glm::vec3 _rotationAxis)
{
    m_Model->SetRotation(_rotationAxis, 360.0f);
}

glm::vec3 CObject::GetRotation()
{
    return m_Model->GetRotation();
}

void CObject::SetGravityEnabled(bool enabled)
{
    if (m_RigidBody) 
    {
        m_RigidBody->enableGravity(enabled);
    }
}

bool CObject::IsGravityEnabled() const
{
    if (m_RigidBody) 
    {
        return m_RigidBody->isGravityEnabled();
    }
    return false;
}

void CObject::SetScale(float _newScale)
{
    m_Model->SetScale(_newScale);
    // Create collision automatically destroys existing collision
    CreateCollisionShape(m_ShapeType);
}

float CObject::GetScale()
{
    return m_Model->GetScale();
}

void CObject::SetMeshFilePath(std::string _FilePath)
{
    m_Model->SetMeshFilePath(_FilePath);
}

std::string CObject::GetMeshFilePath()
{
    return m_Model->GetMeshFilePath();
}

void CObject::SetProgram(GLint _Program)
{
    m_Model->SetProgram(_Program);
}

GLint CObject::GetProgram()
{
    return m_Model->GetProgram();
}

void CObject::SetCollisionShape(CollisionShapeType shapeType)
{
    CreateCollisionShape(shapeType);
}

void CObject::RemoveCollision()
{
    if (!m_RigidBody || !m_CollisionShape) return;

    if (m_Collider) 
    {
        m_RigidBody->removeCollider(m_Collider);
        m_Collider = nullptr;
    }

    switch (m_ShapeType)
    {
    case CollisionShapeType::BOX:
        m_PhysicsCommon->destroyBoxShape(static_cast<rp3d::BoxShape*>(m_CollisionShape));
        break;
    case CollisionShapeType::SPHERE:
        m_PhysicsCommon->destroySphereShape(static_cast<rp3d::SphereShape*>(m_CollisionShape));
        break;
    case CollisionShapeType::CAPSULE:
        m_PhysicsCommon->destroyCapsuleShape(static_cast<rp3d::CapsuleShape*>(m_CollisionShape));
        break;
    default:
        std::cerr << "Unknown shape type, cannot destroy shape.\n";
        break;
    }

    m_CollisionShape = nullptr;
}

void CObject::SetCollisionDimensions(glm::vec3 _newDimensions)
{
    m_CollisionDimensions = _newDimensions;
    SetCollisionShape(m_ShapeType);
}

glm::vec3 CObject::GetDimensions()
{
    return m_CollisionDimensions;
}

json CObject::ToJson() const 
{
    // add object data to a json file
    return 
    {
        // base object
        {"type", "CObject"},
        {"position", {m_Model->GetPosition().x, m_Model->GetPosition().y, m_Model->GetPosition().z}},
        {"rotation", {m_Model->GetRotation().x, m_Model->GetRotation().y, m_Model->GetRotation().z}},
        {"scale", m_Model->GetScale()},
        {"modelPath", m_Model->GetMeshFilePath()},
        {"program", m_Model->GetProgram() },
        {"texture", m_Model->GetTexture() },
        // static kinematic dynamic
        {"bodyType", m_RigidBody->getType() },
        // box sphere capsule
        {"shapeType", m_ShapeType },
        {"collisionDimensions", {m_CollisionDimensions.x, m_CollisionDimensions.y, m_CollisionDimensions.z}}
    };
}

CObject* CObject::FromJson(const json& j, rp3d::PhysicsWorld* _physicsWorld, rp3d::PhysicsCommon& _physicsCommon)
{
    std::string type = j.value("type", "CObject");

    if (type == "CPlayer") 
    {
        return CPlayer::FromJson(j, _physicsWorld, _physicsCommon);
    }

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
     CObject* LoadedObject = new CObject(filePath, program, texture, position,
        _physicsWorld, _physicsCommon, shapeType, shapeDimensions);

     // update rotation and scale
     LoadedObject->SetRotation(glm::vec3(j["rotation"][0], j["rotation"][1], j["rotation"][2]));
     LoadedObject->SetScale(fScale);
     LoadedObject->SetPhysicsBodyType(j["bodyType"]);
     LoadedObject->SetCollisionDimensions(shapeDimensions);

     return LoadedObject;
}