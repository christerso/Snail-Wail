/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "body.h"
#include "coord_system2.h"

Physics::Body::Body(Physics::Subsystem &subsystem)
   : subsystem(subsystem)
   , velocity(vec2::Zero())
{
   
}

void Physics::Body::setOwner(const Ref<Object>::WeakPtr &owner) {
   this->owner = owner;
}

Ref<Object>::WeakPtr Physics::Body::getOwner() const {
   return owner;
}

void Physics::Body::update(float dt) {
  if (Ref<CoordSystem2>::SharedPtr lockedOrigin = origin.lock()) {
    CoordSystemData2 transform = lockedOrigin->getTransform();
    transform.position += velocity * dt;
    lockedOrigin->setTransform(transform);
  }
}

void Physics::Body::addImpulse(const vec2 &vel) {
	velocity += vel;
}

vec2 Physics::Body::getVelocity() const {
	return velocity;
}
