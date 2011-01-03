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

