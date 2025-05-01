/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2025 Media Design School
File Name : CContactListener.h
Description : Child of reactPhysics3d EventListener. Notifies when two bodies overlap or make contact.
Author : Daniel West
Mail : daniel.west@mds.ac.nz
**************************************************************************/

#pragma once
#include <reactphysics3d/reactphysics3d.h>

class CObject;

class CContactListener : public rp3d::EventListener 
{
public:
    //void registerObject(rp3d::CollisionBody* body, CObject* object);
    //void unregisterObject(rp3d::CollisionBody* body);

    /// <summary>
    /// For two objects colliding
    /// </summary>
    /// <param name="callbackData"></param>
    virtual void onContact(const CollisionCallback::CallbackData& callbackData) override;

    /// <summary>
    /// For two objects overlapping
    /// </summary>
    /// <param name="_callbackData"></param>
    virtual void onTrigger(const reactphysics3d::OverlapCallback::CallbackData& _callbackData) override;

private:
};

