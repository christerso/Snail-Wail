//===- coord_system2.h - Coordinate system class definitions ----*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file declares the CoordSystemData2 class and CoordSystem2
// interface. These classes are the basis for managing two-dimensional
// coordinate systems in the engine.
//
//===----------------------------------------------------------------------===//

#ifndef COORDSYSTEM2_H
#define COORDSYSTEM2_H

#include "math/vec2.h"
#include "math/mat2.h"
#include "ref.h"

#include <vector>

/// Describes a transformation in a 2D coordinate system. What kind of space a
/// transformation will take a frame of reference to depends on the use and
/// context of the transformation; this class is agnostic of
/// [World|Model|Eye]View et al.
///
/// @brief A transformation in 2D space 
class Transform2 {
public:
  typedef vec2 position_type;
  typedef mat2 orientation_type;

  Transform2(const position_type &pos, const orientation_type &orient);

  /// Transform a 2D vector by this transformation.
  vec2 transform(const vec2 &other) const;

  /// Transform another transformation, the return value will be relative to
  /// this.
  Transform2 transform(const Transform2 &other) const;

  /// Get the inverse of this transformation. Transforming a transformation by
  /// its inverse should yield Identity().
  Transform2 inverse() const;


  position_type position;
  orientation_type orientation;
};

/// Transformations can be connected together forming a directed acyclic graph,
/// changes to a node will propagate to its children/delegates. Make sure not to
/// introduce any cycles; they are not detected here!
///
/// @brief A node in a DAG of 2D transformations
class TransformNode2 {
public:

  /// Add a child/delegate to this node that should receive transformation
  /// updates. Updates will be relative to the last transformation, and the
  /// delegate will be removed from the list if its reference fails to lock.
  ///
  /// @brief Add a child/delegate to this node
  void addDelegate(const Ref<TransformNode2> &delegate);

  /// Set the transformation and propagate the change to the delegates.
  void setTransform(const Transform2 &transform);

  /// Get the local transformation
  Transform2 getTransform() const;

  
private:
  typedef std::vector<Ref<TransformNode2> > NodeVector;

  Transform2 transformation;
  NodeVector delegates;
};

// FIXME: this needs testing, preferably sooner than later

/// CoordSystemData2 describes a coordinate system in two dimensions containing
/// a position/translation and orientation/rotation.
class CoordSystemData2 {
public:
  typedef vec2 position_type;
  typedef mat2 orientation_type;
  
  CoordSystemData2(const position_type &pos, const orientation_type &orient);
  
  vec2 transform(const vec2 &other) const;
  CoordSystemData2 transform(const CoordSystemData2 &other) const;
  CoordSystemData2 inverse() const;
  
  static CoordSystemData2 Identity();
  
  position_type position;
  orientation_type orientation;
};

/// Interface for modifying and retrieving a coordinate system.
class CoordSystem2 {
public:
  typedef CoordSystemData2 data_type;
  
  virtual ~CoordSystem2() {}
  
  /// Updates the coordinate system with the new one. A successive call to
  /// getTransform should return the same value as was given to setTransform
  /// unless it's a read-only coordinate system.
  virtual void setTransform(const data_type &cs) =0;

  virtual data_type getTransform() const =0;
};

/// A leaf/final implementation of the CoordSystem2 interface, returns the given
/// coordinate system data.
class CoordSystemLeaf2 : public CoordSystem2 {
public:
  CoordSystemLeaf2(const data_type::position_type &pos,
                   const data_type::orientation_type &orient);   
  CoordSystemLeaf2(const data_type &coordSystem = data_type::Identity());
  
  void setTransform(const data_type &cs);
  data_type getTransform() const;
  
private:
  data_type coordSystem;
};

#endif /* end of include guard: COORDSYSTEM2_H */
