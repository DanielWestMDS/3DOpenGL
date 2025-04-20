#pragma once

#include <glm/glm.hpp>
#include "reactphysics3d/reactphysics3d.h"
#include "CModel.h"

class CModel;

class CObject {
public:
    enum class CollisionShapeType
    {
        BOX,
        SPHERE,
        CAPSULE
    };

    CObject(CModel* model, glm::vec3 position,
        rp3d::PhysicsWorld* physicsWorld, rp3d::PhysicsCommon& physicsCommon,
        CollisionShapeType shapeType = CollisionShapeType::BOX,
        glm::vec3 shapeDimensions = glm::vec3(1.0f));
    ~CObject();

    CModel* GetModel() { return m_Model; };

    void Update(float dt);
    void SetPosition(glm::vec3 position);
    glm::vec3 GetPosition();
    void SetGravityEnabled(bool enabled);
    bool IsGravityEnabled() const;

    void SetCollisionShape(CollisionShapeType shapeType, glm::vec3 dimensions);
    rp3d::RigidBody* GetRigidBody() const { return m_RigidBody; };
    rp3d::Collider* GetCollider() const { return m_Collider; };

private:
    void CreateCollisionShape(CollisionShapeType shapeType, glm::vec3 dimensions);

    CModel* m_Model;
    rp3d::PhysicsWorld* m_PhysicsWorld;
    rp3d::PhysicsCommon* m_PhysicsCommon;
    rp3d::RigidBody* m_RigidBody = nullptr;
    rp3d::CollisionShape* m_CollisionShape = nullptr;
    rp3d::Collider* m_Collider = nullptr;
};