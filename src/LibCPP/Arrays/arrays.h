//==============================================================================
//File name:   "arrays.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _ARRAYS_H_
#define _ARRAYS_H_

#include <vector>

//------------------------------------------------------------------------------
//Point class
//------------------------------------------------------------------------------
class PointINT
{
  public:
  int x, y;

  PointINT() { x = y = 0; };
  PointINT(int _x, int _y) { x = _x; y = _y; };
  //----
  PointINT operator +(const PointINT &point) 
          const{return PointINT(x + point.x, y + point.y);};
  //----
  PointINT operator -(const PointINT &point) 
          const{return PointINT(x - point.x, y - point.y);};
  //----
  PointINT operator *(int v) const { return PointINT(x * v, y * v);};
  void operator +=(const PointINT &point) {x += point.x; y += point.y;};
  void operator -=(const PointINT &point) {x -= point.x; y -= point.y;};
};

//------------------------------------------------------------------------------
//Point class
//------------------------------------------------------------------------------
class PointFLT
{
  public:
  float x, y;

  PointFLT() { x = y = 0.0; };
  PointFLT(float _x, float _y) { x = _x; y = _y; };
  //----
  PointFLT operator +(const PointFLT &point) 
          const{return PointFLT(x + point.x, y + point.y);};
  //----
  PointFLT operator -(const PointFLT &point) 
          const{return PointFLT(x - point.x, y - point.y);};
  //----
  PointFLT operator *(float v) const { return PointFLT(x * v, y * v);};
  void operator +=(const PointFLT &point) {x += point.x; y += point.y;};
  void operator -=(const PointFLT &point) {x -= point.x; y -= point.y;};
};

//------------------------------------------------------------------------------
//Point class
//------------------------------------------------------------------------------
class PointDBL
{
  public:
  double x, y;

  PointDBL() { x = y = 0.0; };
  PointDBL(double _x, double _y) { x = _x; y = _y; };
  //----
  PointDBL operator +(const PointDBL &point) 
          const{return PointDBL(x + point.x, y + point.y);};
  //----
  PointDBL operator -(const PointDBL &point) 
          const{return PointDBL(x - point.x, y - point.y);};
  //----
  PointDBL operator *(double v) const { return PointDBL(x * v, y * v);};
  void operator +=(const PointDBL &point) {x += point.x; y += point.y;};
  void operator -=(const PointDBL &point) {x -= point.x; y -= point.y;};
};

#endif
