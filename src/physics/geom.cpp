/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "geom.h"

Physics::Geom::Geom(const rect & size)
   : size(size)
   , collisionMask(0xFFFFFFFFu)
{
   collisionId = 0;
}

void Physics::Geom::setBody(const Ref<Physics::Body> & body) {
   this->linkedBody = body;
}

void Physics::Geom::setRefFrame(const Ref<ReferenceFrame2> & refFrame) {
   this->refFrame = refFrame;
}

rect Physics::Geom::getSize() const {
   return size;
}

void Physics::Geom::setPosition(const vec2 & newPos) {
   this->position = newPos;
}

vec2 Physics::Geom::getPosition() const {
   return position;
}

void Physics::Geom::setOrientation(const mat2 & orient) {
   
}

mat2 Physics::Geom::getOrientation() const {
   return mat2::Identity;
}

void Physics::Geom::setCollisionId(unsigned int collisionId) {
   this->collisionId = collisionId;
}

void Physics::Geom::setCollisionMask(const std::bitset<32> & mask) {
   this->collisionMask = mask;
}
