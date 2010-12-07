/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GEOM_H_R5WJNYU0
#define GEOM_H_R5WJNYU0

#include "reference_frame2.h"
#include "math/rect.h"
#include "ref.h"

#include <bitset>

class Object;

namespace Graphics {class RenderList; class Renderer; }

namespace Physics {
	class Body;
	class Geom;
	
	class GeomEventHandler {
	public:
	   virtual ~GeomEventHandler() {}
	   virtual void collided(const Ref<Geom>::SharedPtr & with) {}
	};

   class Geom : public ReferenceFrame2 {      
   public:
	  friend class Subsystem; // TODO: get rid of this
	  
      Geom(const rect & size);

      void setBody(const Ref<Physics::Body> & body);
      void setRefFrame(const Ref<ReferenceFrame2> & refFrame);
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
	  
	  // ReferenceFrame2 --------------------------------------------
      void setPosition(const vec2 & newPos);
      vec2 getPosition() const;
      void setOrientation(const mat2 & orient);
      mat2 getOrientation() const;
      // ------------------------------------------------------------
   
   private:
	  Ref<Physics::Body> linkedBody;
	  Ref<ReferenceFrame2> refFrame;
	  Ref<GeomEventHandler> eventHandler;
	  
	  rect size;
      int priority;
      
	  std::bitset<32> collisionMask;
	  unsigned int collisionId;
	   
	  vec2 position;
   };
}

#endif /* end of include guard: GEOM_H_R5WJNYU0 */
