/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GEOM_H_R5WJNYU0
#define GEOM_H_R5WJNYU0

#include "math/rect.h"
#include "ref.h"

#include <bitset>

class Object;
class CoordSystem2;

namespace Graphics {
class RenderList;
class Renderer;
}

namespace Physics {
class Body;
class Geom;
	
class GeomEventHandler {
public:
  virtual ~GeomEventHandler() {}
  virtual void collided(const Ref<Geom>::SharedPtr & with) {}
};

class Geom {
public:
  friend class Subsystem; // TODO: get rid of this
  
  Geom(const rect & size);
  
  void setBody(const Ref<Physics::Body> & body);
  void setEventHandler(const Ref<GeomEventHandler> & eventHandler);
  void setOffset(const vec2 & offset);
  void setPriority(int prio);
  int getPriority() const;
  
  rect getSize() const;
  void enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList);
  
  void setCollisionId(unsigned int collisionId);
  void setCollisionMask(const std::bitset<32> & mask);
  void collided(const Ref<Geom>::SharedPtr & with);
  
  Ref<Object>::WeakPtr getOwner() const;
  
  Ref<CoordSystem2> origin;
  Ref<Physics::Body> linkedBody;   
private:
  
  Ref<GeomEventHandler> eventHandler;
  
  rect size;
  int priority;
      
  std::bitset<32> collisionMask;
  unsigned int collisionId;
  
  vec2 position;
};
}

#endif /* end of include guard: GEOM_H_R5WJNYU0 */
