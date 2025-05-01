#pragma once

#include <glm/glm.hpp>
#include "reactphysics3d/reactphysics3d.h"
#include "CModel.h"
#include "json.hpp"

using json = nlohmann::json;

class CModel;

class CObject {
public:
    enum class CollisionShapeType
    {
        BOX,
        SPHERE,
        CAPSULE
    };

    CObject(std::string FilePath, GLint _program, GLint _texture, glm::vec3 position,
        rp3d::PhysicsWorld* physicsWorld, rp3d::PhysicsCommon& physicsCommon,
        CollisionShapeType shapeType = CollisionShapeType::BOX,
        glm::vec3 shapeDimensions = glm::vec3(2.f));
    ~CObject();

    CModel* GetModel() { return m_Model; };

    virtual void Update(float dt, GLFWwindow* _window);
    void SetPosition(glm::vec3 _position);
    glm::vec3 GetPosition();

    void SetRotation(glm::vec3 _rotationAxis);
    glm::vec3 GetRotation();

    void SetGravityEnabled(bool _enabled);
    bool IsGravityEnabled() const;

    void SetScale(float _newScale);
    float GetScale();

    void SetMeshFilePath(std::string _FilePath);
    std::string GetMeshFilePath();

    void SetProgram(GLint _Program);
    GLint GetProgram();

    void SetCollisionShape(CollisionShapeType _shapeType);
    void RemoveCollision();
    rp3d::RigidBody* GetRigidBody() const { return m_RigidBody; };
    rp3d::Collider* GetCollider() const { return m_Collider; };

    void SetPhysicsBodyType(rp3d::BodyType _bodyType);
    rp3d::BodyType GetPhysicsBodyType();

    void SetCollisionDraw(bool _bIsEnabled);

    void SetCollisionDimensions(glm::vec3 _newDimensions);
    glm::vec3 GetDimensions();

    virtual json ToJson() const;

    static CObject* FromJson(const json& j,
        rp3d::PhysicsWorld* _physicsWorld,
        rp3d::PhysicsCommon& _physicsCommon);

protected:
    void CreateCollisionShape(CollisionShapeType shapeType);

    CModel* m_Model;
    rp3d::PhysicsWorld* m_PhysicsWorld;
    rp3d::PhysicsCommon* m_PhysicsCommon;
    rp3d::RigidBody* m_RigidBody = nullptr;
    rp3d::CollisionShape* m_CollisionShape = nullptr;
    CollisionShapeType m_ShapeType = CollisionShapeType::BOX; // Default to BOX
    rp3d::Collider* m_Collider = nullptr;
    glm::vec3 m_CollisionDimensions;

    // do trigger stuff 
    
    bool m_bPhysicsEnabled = false;
};