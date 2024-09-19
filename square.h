#include <memory>
class Square;
using SquarePtr = std::shared_ptr<Square>; 

#ifndef SQUARE_H
#define SQUARE_H

#include "shape.h"

class Square : public Shape {
  unsigned int m_vao;
protected:
  Square();
public:
  static SquarePtr Make ();
  virtual ~Square ();
  virtual void Draw (StatePtr st);
};
#endif