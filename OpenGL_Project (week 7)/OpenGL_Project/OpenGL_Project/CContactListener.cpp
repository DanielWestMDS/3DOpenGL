//#include "CContactListener.h"
//
//void CContactListener::onContact(const CollisionCallback::CallbackData& callbackData)
//{
//    // For each contact pair
//    for (unsigned int p = 0; p < callbackData.getNbContactPairs(); p++) 
//    {
//
//        // Get the contact pair
//        CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);
//
//        // For each contact point of the contact pair
//        for (int c = 0; c < contactPair.getNbContactPoints(); c++) {
//
//            // Get the contact point
//            CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(c);
//
//            // Get the contact point on the first collider and convert it in world-space
//            rp3d::Vector3 worldPoint = contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();
//        }
//    }
//}
