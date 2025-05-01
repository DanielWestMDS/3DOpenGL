#include "CContactListener.h"

#include "CEditorMode.h"

#include <iostream>

void CContactListener::onContact(const CollisionCallback::CallbackData& callbackData)
{
    // For each contact pair
    for (unsigned int p = 0; p < callbackData.getNbContactPairs(); p++) 
    {

        // Get the contact pair
        CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);

        // For each contact point of the contact pair
        for (int c = 0; c < contactPair.getNbContactPoints(); c++) {

            // Get the contact point
            CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(c);

            // Get the contact point on the first collider and convert it in world-space
            rp3d::Vector3 worldPoint = contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();
        }
    }
}

void CContactListener::onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData)
{
    CEditorMode& Editor = CEditorMode::GetInstance();

    for (int i = 0; i < callbackData.getNbOverlappingPairs(); i++)
    {
        if (callbackData.getOverlappingPair(i).getCollider1()->getIsTrigger() ||
            callbackData.getOverlappingPair(i).getCollider2()->getIsTrigger())
        {
            Editor.SetGameWon(true);

        }
    }
}
