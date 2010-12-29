/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef HELMET_H
#define HELMET_H

#include "object.h"
#include "physics/geom.h"
#include "ref.h"

// Forward declarations
class CoordSystem2;

namespace Physics {
class Geom;
class Body;
}

namespace Graphics {
class Sprite;
}

class Helmet : public Object, public Physics::GeomEventHandler {
public:
  Helmet();
  
  void collided(const Ref<Physics::Geom>::SharedPtr & with);
  
  // CoordSystem2
  //void setTransform(const CoordSystemData2 & cs);
  //CoordSystemData2 getTransform() const;
  
  Ref<Graphics::Sprite> sprite;
  Ref<Physics::Geom> geom;
  Ref<Physics::Body> body;
  Ref<Physics::Body> snailBody;
  Ref<CoordSystem2> origin;
  
private:
  int health;
};

#endif // !HELMET_H

