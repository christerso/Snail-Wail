//===- coord_system2.cpp - Implements coordinate systems --------*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file implements the data class and leaf class for 2D coordinate systems.
// A "leaf" class is an implementation of the CoordSystem interface and contains
// concrete data that it returns, making it a leaf in a tree of transformations.
//
//===----------------------------------------------------------------------===//

#include "coord_system2.h"

CoordSystemData2 CoordSystemData2::Identity() {
  static CoordSystemData2 cs(CoordSystemData2::position_type::Identity(),
                             CoordSystemData2::orientation_type::Identity());
  return cs;
}


Transform2::Transform2(const position_type &pos, const orientation_type &orient)
  : position(pos)
  , orientation(orient)
{
}

vec2 Transform2::transform(const vec2 &other) const {
  return vec2(this->position + this->orientation * other);
}

Transform2 Transform2::transform(const Transform2 &other) const {
  return Transform2(this->position + this->orientation * other.position,
                    this->orientation * other.orientation);
}

Transform2 Transform2::inverse() const {
  return Transform2(-position, orientation.inverse());
}

void TransformNode2::addDelegate(const Ref<TransformNode2> &delegate) {
  delegates.push_back(delegate);
}


void TransformNode2::setTransform(const Transform2 &transform) {
  if (!delegates.empty()) {
    typedef NodeVector::value_type DelegateRef;
    const Transform2 invPrevious = transformation.inverse();
    
    NodeVector::iterator i = delegates.begin(), e = delegates.end();

    for ( ; i != e; ) {
      if (DelegateRef::SharedPtr lDelegate = i->lock()) {
        // Locked the delegate; it's a valid reference
        const Transform2 absTransform = lDelegate->getTransform();
        const Transform2 relTransform = absTransform.transform(invPrevious);

        lDelegate->setTransform(transform.transform(relTransform));
        ++i;
      }
      else {
        // Remove the delegate
        *i = *--e;
      }
    }

    delegates.erase(e, delegates.end());
  }

  transformation = transform;
}

Transform2 TransformNode2::getTransform() const {
  return transformation;
}











CoordSystemData2::CoordSystemData2(const position_type &pos,
                                   const orientation_type &orient)
  : position(pos)
  , orientation(orient)
{
  
}

vec2 CoordSystemData2::transform(const vec2 &other) const {
  return vec2(this->position + this->orientation * other);
}

CoordSystemData2 CoordSystemData2::transform(const CoordSystemData2 &other) const {
  return CoordSystemData2(this->position + this->orientation * other.position,
                          this->orientation * other.orientation);
}

CoordSystemData2 CoordSystemData2::inverse() const {
  return CoordSystemData2(-position, orientation);
}

CoordSystemLeaf2::CoordSystemLeaf2(const data_type::position_type &pos,
                                   const data_type::orientation_type &orient)
  : coordSystem(CoordSystemData2(pos, orient))
{
}

CoordSystemLeaf2::CoordSystemLeaf2(const data_type &coordSystem)
  : coordSystem(coordSystem)
{
}

void CoordSystemLeaf2::setTransform(const data_type &cs) {
  coordSystem = cs;
}

CoordSystemLeaf2::data_type CoordSystemLeaf2::getTransform() const {
  return coordSystem;
}

