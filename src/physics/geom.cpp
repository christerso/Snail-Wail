/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "geom.h"
#include "body.h"
#include "coord_system2.h"
#include "graphics/vertex.h"
#include "graphics/render_list.h"
#include "graphics/mesh.h"
#include <vector>

namespace {
/// Helper function for getting the transformation of a node. When it can't lock
/// the reference, the "fallback" value will be returned.
CoordSystemData2 DerefTransform(const Ref<CoordSystem2> &ref,

              const CoordSystemData2 &fallback = CoordSystemData2::Identity()) {
  CoordSystemData2 ret = fallback;

  if (Ref<CoordSystem2>::SharedPtr lockedRef = ref.lock()) {
    ret = lockedRef->getTransform();
  }
  
  return ret;
}

}

Physics::Geom::Geom(const rect & size)
   : size(size)
   , collisionMask(0xFFFFFFFFU)
{
   collisionId = 0;
   priority = 0;
}

void Physics::Geom::setBody(const Ref<Physics::Body> & body) {
   this->linkedBody = body;
}

rect Physics::Geom::getSize() const {
   return size;
}

void Physics::Geom::setOffset(const vec2 & offset) {
   size.origin = offset;
}


void Physics::Geom::setCollisionId(unsigned int collisionId) {
   this->collisionId = collisionId;
}

void Physics::Geom::setCollisionMask(const std::bitset<32> & mask) {
   this->collisionMask = mask;
}

void Physics::Geom::setEventHandler(const Ref<GeomEventHandler> & eventHandler) {
	this->eventHandler = eventHandler;
}

Ref<Object>::WeakPtr Physics::Geom::getOwner() const {
   if (Ref<Physics::Body>::SharedPtr lockedPtr = linkedBody.lock())
	  return lockedPtr->getOwner();

   throw std::runtime_error("no body linked to geom");
}

void Physics::Geom::collided(const Ref<Geom>::SharedPtr & with) {
	if (Ref<GeomEventHandler>::SharedPtr lockedPtr = eventHandler.lock())
       lockedPtr->collided(with);
}

void Physics::Geom::enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList) {
  vec2 position = DerefTransform(origin).transform(size.origin);
  
  // TODO: get vertices from size instead
  std::vector<Vertex2T2> vertices;
  vertices.reserve(4);
  vertices.push_back(Vertex2T2(size.halfSize * vec2(-1.0f, -1.0f) + position, vec2(0.0f, 0.0f)));
  vertices.push_back(Vertex2T2(size.halfSize * vec2(1.0f, -1.0f) + position, vec2(1.0f, 0.0f)));
  vertices.push_back(Vertex2T2(size.halfSize * vec2(1.0f, 1.0f) + position, vec2(1.0f, 1.0f)));
  vertices.push_back(Vertex2T2(size.halfSize * vec2(-1.0f, 1.0f) + position, vec2(0.0f, 1.0f)));
  
  Ref<Graphics::Mesh>::SharedPtr mesh(new Graphics::Mesh); // TODO: not good to allocate every time
  mesh->vertices = vertices;
  renderList->insert(Ref<Graphics::Renderer>::SharedPtr(), mesh);
}

void Physics::Geom::setPriority(int prio) {
   this->priority = prio;
}

int Physics::Geom::getPriority() const {
   return priority;
}
