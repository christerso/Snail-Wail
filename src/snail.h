/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef SNAIL_H_7SGQH1KL
#define SNAIL_H_7SGQH1KL

#include "object.h"
#include "ref.h"
#include "physics/geom.h"
#include "physics/body.h"

class PlayerEntity;
class World;
class vec2;
class Helmet;
class HealthMeter;
class CoordSystem2;

namespace Graphics {
class Sprite;
class Subsystem;
}

namespace Physics {
class Geom;
}
  
class SnailEventHandler {
public:
  virtual ~SnailEventHandler() {}
   
  virtual void onHealthChange(float newValue, float diff) =0;
};


enum SnailType {
  SnailLeft = 1,
  SnailRight
};

class Snail : public Object, public Physics::GeomEventHandler {
public:
  Snail(SnailType type);
  
  void collided(const Ref<Physics::Geom>::SharedPtr & with);
  
  void setEventHandler(const Ref<SnailEventHandler> & newHandler);
  void increaseHealth(int add);
  
  Ref<SnailEventHandler> eventHandler;
  Ref<Graphics::Sprite> sprite;
  Ref<Helmet> helmet;
  Ref<PlayerEntity> logic;
  Ref<Physics::Geom> physGeom;
  Ref<Physics::Body> physBody;
  Ref<Snail>::WeakPtr enemy;
  Ref<CoordSystem2> origin;
  
private:
  int health;
};

#endif /* end of include guard: SNAIL_H_7SGQH1KL */
