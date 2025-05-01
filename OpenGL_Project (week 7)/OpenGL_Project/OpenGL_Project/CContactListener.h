#pragma once
#include <reactphysics3d/reactphysics3d.h>

class CObject;

class CContactListener : public rp3d::EventListener 
{
public:
    //void registerObject(rp3d::CollisionBody* body, CObject* object);
    //void unregisterObject(rp3d::CollisionBody* body);

    virtual void onContact(const CollisionCallback::CallbackData& callbackData) override;

    virtual void onTrigger(const reactphysics3d::OverlapCallback::CallbackData& _callbackData) override;

private:
};

