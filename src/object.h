/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef OBJECT_H_GV2YM86M
#define OBJECT_H_GV2YM86M

#include "ref.h"
#include "coord_system2.h"
#include <utility>
#include <vector>

class World;
class ObjectList;
class Object;

class ObjectEventListener {
public:
  virtual ~ObjectEventListener() {}

  virtual void onObjectDeath(const Object & object) =0;
};

  
class Object {
public:
  typedef Ref<CoordSystem2> *Tag;
  typedef std::pair<std::string, Tag> TagPair;
  
  Object();
  virtual ~Object() {}

  Tag getTag(const std::string &id) const;
  

  void addedOwner(ObjectList * owner);
  void kill();
  void setEventListener(const Ref<ObjectEventListener> & newListener);
  void addTags(std::pair<std::string, Tag> *begin,
               std::pair<std::string, Tag> *end);
  
  virtual void activate();
  virtual void deactivate();

private:
  typedef std::vector<TagPair> TagVector;
  TagVector tags;
  Ref<ObjectEventListener> eventListener;
  ObjectList * owner;
};


/// A coordinate system that describes an object's tag
class ObjectTagWrap : public CoordSystem2 {
public:
  ObjectTagWrap(const Ref<Object> &object, const std::string &id)
    : object(object), id(id) {}

  void setTransform(const CoordSystemData2 &data) {
    (*object->getTag(id))->setTransform(data);
  }
  
  CoordSystemData2 getTransform() const {
    return (*object->getTag(id))->getTransform();
  }

private:
  const Ref<Object> object;
  const std::string id;
};


#endif /* end of include guard: OBJECT_H_GV2YM86M */
