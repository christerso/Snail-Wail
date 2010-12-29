
/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef CACTUS_H
#define CACTUS_H

#include "object.h"
#include "graphics/sprite.h"
#include "ref.h"
#include "physics/geom.h"

//namespace Graphics {class Sprite; }
namespace Physics {
class Body;
class Geom;
}

class World;

class Cactus : public Object, public Graphics::SpriteEventHandler, public Physics::GeomEventHandler {
public:
  Cactus();
   
  void collided(const Ref<Physics::Geom>::SharedPtr & with);
  void leftView();

  Ref<Graphics::Sprite> sprite;
  Ref<Physics::Body> body;
  Ref<Physics::Geom> geom;
  Ref<CoordSystem2> origin;
  
private:
  void increaseHealth(float by);
  
  float health;
};

#endif /* end of include guard: CACTUS_H */
