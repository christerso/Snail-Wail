/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "object.h"
#include "object_list.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <iterator>

namespace {
/// Helper struct for sorting pairs by their first value
template<class Type>
struct less1st : public std::binary_function<Type, Type, bool> {
  bool operator() (const Type &lhs, const Type &rhs) const {
    return lhs.first < rhs.first;
  }

  bool operator() (const Type &lhs, const typename Type::first_type &rhs) const {
    return lhs.first < rhs;
  }

  bool operator() (const typename Type::first_type &lhs, const Type &rhs) const {
    return lhs < rhs.first;
  }
};

}

Object::Object() {
   owner = NULL;
}

void Object::addedOwner(ObjectList * owner) {
   if (this->owner)
      throw std::runtime_error("addedOwner: object is already owned!");
      
   this->owner = owner;
}

void Object::kill() {
   if (owner) {
      owner->enqueueRemoval(this);
	  owner = NULL; // TODO: check if enqueueRemoval succeeds, and then remove owner?
	  
	  if (Ref<ObjectEventListener>::SharedPtr lockedListener = eventListener.lock())
		 lockedListener->onObjectDeath(*this); // TODO: in object dtor instead? but maybe not, what about caches
   }
   else {
	  std::cout << "object::kill: no owner" << std::endl;
   }
}

void Object::setEventListener(const Ref<ObjectEventListener> & newListener) {
   this->eventListener = newListener;
}

// Called by the cache or something
void Object::activate() {

}

// Also called by the cache or owner
void Object::deactivate() {

}

void Object::addTags(std::pair<std::string, Tag> *begin,
                     std::pair<std::string, Tag> *end) {
  tags.insert(tags.end(), begin, end);
  std::sort(tags.begin(), tags.end(), less1st<TagPair>());
}


Object::Tag Object::getTag(const std::string &id) const {
  std::pair<TagVector::const_iterator, TagVector::const_iterator> tagRange =
    std::equal_range(tags.begin(), tags.end(), id, less1st<TagPair>());

  Tag ret;

  if (std::distance(tagRange.first, tagRange.second) > 1) {
    // if the string matched more than one tag
    std::cout << "warning: ambiguous tag '" << id << "'";
    ret = tagRange.first->second;
  }
  else if (tagRange.first != tagRange.second) {
    // if it could find a result
    ret = tagRange.first->second;
  }
  else {
    throw std::runtime_error("tag not found: '" + id + "'");
  }
  
  return ret;
}

