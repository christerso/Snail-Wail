//===- sprite.h - Sprite class definition -----------------------*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file declares the Sprite class.
//
//===----------------------------------------------------------------------===//


#ifndef SPRITE_H
#define SPRITE_H

#include <vector>
#include "math/vec2.h"
#include "math/mat2.h"
#include "graphics/vertex.h"
#include "math/rect.h"
#include "ref.h"

class CoordSystem2;

namespace Graphics {
class Renderer;
class RenderList;

class SpriteEventHandler {
public:
  virtual ~SpriteEventHandler() {}
  
  virtual void enteredView() {}
  virtual void leftView() {}
};
   

class Sprite {
public:
  Sprite(const Ref<Graphics::Renderer>::SharedPtr & renderer, const rect & size);
  
  void enqueueRender(const Ref<Graphics::RenderList>::SharedPtr & renderList);
  void enteredView();
  void leftView();
  
  void setGrid(int columns, int rows);
  void setCell(int x, int y);
  
  rect getSize() const;
  rect getBoundingBox() const;
  void setEventHandler(const Ref<SpriteEventHandler>::WeakPtr & eventHandler);
  
  Ref<CoordSystem2> origin;
  
private:
  std::vector<Vertex2T2> constructVertices() const;
  
  Ref<SpriteEventHandler>::WeakPtr eventHandler;
  Ref<Graphics::Renderer>::SharedPtr renderer;
  rect size, originalSize;
  vec2 position;
  mat2 orientation;
  int columns, rows;
  int cellX, cellY;
};

}

#endif /* end of include guard: SPRITE_H */
