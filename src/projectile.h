/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "object.h"
#include "graphics/sprite.h"
#include "ref.h"

//namespace Graphics {class Sprite; }
namespace Physics {
class Body;
class Geom;
}

class World;
class Snail;
class CoordSystem2;
  
class Projectile : public Object, public Graphics::SpriteEventHandler {
public:
  virtual ~Projectile() {}
    
  void leftView();
  
  Ref<Snail>::WeakPtr shooter;
  Ref<Graphics::Sprite> sprite;
  Ref<Physics::Body> body;
  Ref<Physics::Geom> geom;
  Ref<CoordSystem2> origin;
};


#endif // !PROJECTILE_H
