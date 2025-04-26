#include "CObject.h"
#include "CModel.h"
#include <iostream>

CObject::CObject(CModel* model, glm::vec3 position,
    rp3d::PhysicsWorld* physicsWorld, rp3d::PhysicsCommon& physicsCommon,
    CollisionShapeType shapeType, glm::vec3 shapeDimensions)
    : m_Model(model), m_PhysicsWorld(physicsWorld), m_PhysicsCommon(&physicsCommon)
{
    // Create the rigid body
    rp3d::Vector3 rpPosition(position.x, position.y, position.z);
    rp3d::Transform transform(rpPosition, rp3d::Quaternion::identity());
    m_RigidBody = physicsWorld->createRigidBody(transform);

    // Create collision shape
    CreateCollisionShape(shapeType, shapeDimensions);
    SetPosition(position);
    SetGravityEnabled(false);
}

CObject::~CObject()
{
    if (m_RigidBody && m_PhysicsWorld) 
    {
        // Remove collider first
        if (m_Collider) 
        {
            m_RigidBody->removeCollider(m_Collider);
        }
        if (m_CollisionShape) 
        {
            //TODO: fix this (destroyCollisionShape isn't a thing)
            //m_PhysicsCommon->destroyCollisionShape(m_CollisionShape);
        }
        m_PhysicsWorld->destroyRigidBody(m_RigidBody);
    }
}

int CObject::GetID()
{
    return m_ID;
}

void CObject::SetID(int _ID)
{
    m_ID = _ID;
}

void CObject::CreateCollisionShape(CollisionShapeType shapeType, glm::vec3 dimensions)
{
    if (!m_RigidBody) return;

    // Remove previous collider if exists
    if (m_Collider) 
    {
        m_RigidBody->removeCollider(m_Collider);
    }
    if (m_CollisionShape) 
    {
        //TODO: this too
        //m_PhysicsCommon->destroyCollisionShape(m_CollisionShape);
    }

    // Create new collision shape
    switch (shapeType) 
    {
    case CollisionShapeType::BOX:
        m_CollisionShape = m_PhysicsCommon->createBoxShape(
            rp3d::Vector3(dimensions.x / 2.0f, dimensions.y / 2.0f, dimensions.z / 2.0f));
        break;

    case CollisionShapeType::SPHERE:
        m_CollisionShape = m_PhysicsCommon->createSphereShape(dimensions.x);
        break;

    case CollisionShapeType::CAPSULE:
        m_CollisionShape = m_PhysicsCommon->createCapsuleShape(
            dimensions.x, dimensions.y);
        break;
    }

    // Add collider to the rigid body
    m_Collider = m_RigidBody->addCollider(m_CollisionShape, rp3d::Transform::identity());

    // Update mass properties (important for dynamic bodies)
    m_RigidBody->updateMassPropertiesFromColliders();
}


void CObject::Update(float dt)
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

void CObject::SetCollisionShape(CollisionShapeType shapeType, glm::vec3 dimensions)
{
    CreateCollisionShape(shapeType, dimensions);
}