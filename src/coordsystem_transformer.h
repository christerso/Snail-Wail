//===- coordsystem_transformer.h --------------------------------*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file declares the CoordSystemTransformer class.
//
//===----------------------------------------------------------------------===//

#ifndef COORDSYSTEM_TRANSFORMER_H
#define COORDSYSTEM_TRANSFORMER_H

#include "ref.h"

/// This class is a converter that describes a parent-child relationship
/// between two coordinate systems.
template<typename CoordSysT>
class TransformedCoordSystem : public CoordSysT {
public:
  
  /// Constructor - Binds the transformer to the child and sets
  /// the transformation to make.
  TransformedCoordSystem(const Ref<CoordSysT> & delegate,
                         const typename CoordSysT::data_type & frame)
    : delegate(delegate)
    , frame(frame)
  {
  }
  
  /// Transforms the incoming coordinate system with the one in the transformer
  /// and updates the delegate with it.
  void setTransform(const typename CoordSysT::data_type & cs) {
    typedef typename CoordSysT::data_type CsFrameT;
    
    if (typename Ref<CoordSysT>::SharedPtr lockedDelegate = delegate.lock()) {
      CsFrameT newFrame = CsFrameT(cs).transform(frame.inverse());
      lockedDelegate->setTransform(newFrame);
    }
  }
  
  /// Retrieves the coordinate system of the delegate and applies the inverted
  /// coordinate system of the transformer.
  typename CoordSysT::data_type getTransform() const {
    typedef typename CoordSysT::data_type CsFrameT;
    
    if (typename Ref<CoordSysT>::SharedPtr lockedDelegate = delegate.lock()) {
      CsFrameT delegateFrame = lockedDelegate->getTransform();
      return frame.transform(delegateFrame);
    }
    
    return frame;
  }
  
  
private:
  Ref<CoordSysT> delegate;
  typename CoordSysT::data_type frame;
};

class CoordSystem2;
typedef TransformedCoordSystem<CoordSystem2> TransformedCoordSystem2;

#endif // !COORDSYS_TRANSFORMER_H
