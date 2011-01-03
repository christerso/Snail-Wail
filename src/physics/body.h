/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef BODY_H_JI764YY5
#define BODY_H_JI764YY5

#include "ref.h"
#include "math/vec2.h"
#include "math/mat2.h"

class Object;
class CoordSystem2;

namespace Physics {
class Subsystem;
   
/*
 * A physical body that can be acted upon through forces
 */
class Body {
private:
  Body(Physics::Subsystem &subsystem); 
  
public:
  
  friend class Physics::Subsystem;
  
  void update(float dt);
  void addImpulse(const vec2 &vel);
  vec2 getVelocity() const;
  
  void setOwner(const Ref<Object>::WeakPtr &owner);
  Ref<Object>::WeakPtr getOwner() const;

  Ref<CoordSystem2> origin;

private:
  Physics::Subsystem &subsystem;
  Ref<Object>::WeakPtr owner;
  vec2 velocity;
};
   
}

#endif /* end of include guard: BODY_H_JI764YY5 */
