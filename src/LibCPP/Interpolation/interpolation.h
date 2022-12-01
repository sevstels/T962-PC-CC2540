//==============================================================================
//File name:   "interpolation.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _DOT_CURVE_INTERPOLATION_H_
#define _DOT_CURVE_INTERPOLATION_H_
#include <vector>

//------------------------------------------------------------------------------
//Point2D class
//------------------------------------------------------------------------------
class Point2D
{
  public:
  double x, y;

  Point2D() { x = y = 0.0; };
  Point2D(double _x, double _y) { x = _x; y = _y; };
  //----
  Point2D operator +(const Point2D &point) 
          const{return Point2D(x + point.x, y + point.y);};
  //----
  Point2D operator -(const Point2D &point) 
          const{return Point2D(x - point.x, y - point.y);};
  //----
  Point2D operator *(double v) const { return Point2D(x * v, y * v);};
  void operator +=(const Point2D &point) {x += point.x; y += point.y;};
  void operator -=(const Point2D &point) {x -= point.x; y -= point.y;};

  void normalize();
};

//------------------------------------------------------------------------------
//CSegment class
//------------------------------------------------------------------------------
class CSegment
{
  public:
  Point2D points[4];
  void Calc(double t, Point2D &p);
};
/*
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int round(double value)
{
  return (int)floor(value+(double)0.5);
} */

//------------------------------------------------------------------------------
//Main class
//------------------------------------------------------------------------------
class CINTP
{
  //Construction
  public:
  CINTP();
 ~CINTP();

  int LoadData(short *pBuf, int length);
  int GetData(short *pBuf, int length);
  void TestBezier(int resolution);
  void Bezier(double resolution);
  void BezierResizeA(short *pBuf, CArray<CPoint, CPoint> *pArray, 
	                  int width, int height);
  void BezierInterpolation(CArray<CPoint, CPoint> *pArray);
  void BezierResizer(CArray<CPoint, CPoint> *pArray, 
	                 int x0, int x_max, int y0, int y_max);
  protected:
  Point2D p;
  std::vector<Point2D>  values; 
  std::vector<CSegment> bezier;
  std::vector<Point2D>  output;
   
  //----
  int CalcSpline(void); 
};

#endif
