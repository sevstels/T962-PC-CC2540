//==============================================================================
//File name:    "bezier.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "afxwin.h"
#include "interpolation.h"
#include <vector>
#include <iostream>
//#include <cmath>
#include <math.h>

//---- Mem Leakage Debug
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//----

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

#define EPSILON 1.0e-5
#define RESOLUTION 32

//------------------------------------------------------------------------------
//Constructor
//------------------------------------------------------------------------------
CINTP::CINTP()
{					
  values.clear(); 
  bezier.clear();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
CINTP::~CINTP()
{
  values.clear(); 
  bezier.clear();
}

//------------------------------------------------------------------------------
//CSegment class
//------------------------------------------------------------------------------
void CSegment::Calc(double t, Point2D &p)
{
  double t2 = t * t;
  double t3 = t2 * t;
  double nt = 1.0 - t;
  double nt2 = nt * nt;
  double nt3 = nt2 * nt;
  
  //----
  p.x = nt3 * points[0].x + 3.0 * t * nt2 * points[1].x + 
        3.0 * t2 * nt * points[2].x + t3 * points[3].x;
  //----
  p.y = nt3 * points[0].y + 3.0 * t * nt2 * points[1].y + 
        3.0 * t2 * nt * points[2].y + t3 * points[3].y;
}

//------------------------------------------------------------------------------
//Point2D class
//------------------------------------------------------------------------------
void Point2D::normalize()
{
  double l = sqrt(x * x + y * y);
  x /= l;
  y /= l;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------  
int CINTP::LoadData(short *pBuf, int length)
{
  int i;
  values.clear();
  values.empty();

  for(i=0; i<length; i++)
  {
	values.push_back(Point2D(i, *pBuf++));
  }

  return i;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
short round(double value)
{
  return (short)floor(value+(double)0.5);
} 

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------  
int CINTP::GetData(short *pBuf, int length)
{
  int i;
  int data_size = output.size();
  int index;

  if(data_size>length)index = length; 
  else index = data_size;
  
  for(i=0; i<index; i++)
  {
	short value = round(output[i].y);
	*pBuf++ = value;

    TRACE2("X=%d  Y=%d\n", i, value);
  }

  output.clear();
  output.empty();
  return i;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
void CINTP::TestBezier(int resolution)
{
  values.push_back(Point2D(0, 0));
  values.push_back(Point2D(20, 0));
  values.push_back(Point2D(45, -47));
  values.push_back(Point2D(53, 335));
  values.push_back(Point2D(57, 26));
  values.push_back(Point2D(62, 387));
  values.push_back(Point2D(74, 104));
  values.push_back(Point2D(89, 0));
  values.push_back(Point2D(95, 100));
  values.push_back(Point2D(100, 0));
 
  CalcSpline();
  CSegment sg;
  
  int count = values.size(); 
  for(int i=0; i<count-1; i++)
  {
	 sg.points[0] = bezier[i].points[0];
	 sg.points[1] = bezier[i].points[1];
	 sg.points[2] = bezier[i].points[2];
	 sg.points[3] = bezier[i].points[3];

	 for(int j=0; j<resolution; j++)
	 {
	   sg.Calc((double)j/(double)resolution, p);
 	   //----
	   TRACE1("i:%d  ", j);
	   TRACE2("X=%f  Y=%f\n", p.x, p.y);
	   //cout << p.x << " " << p.y << endl;
	   //printf("%lf %lf\n", p.x, p.y);
	 }
  }
}

//------------------------------------------------------------------------------
//приводит значени€ к ближайшим целым значени€м
//берЄм значени€ и возвращаем результат в тот же массив 
//------------------------------------------------------------------------------ 
void CINTP::BezierResizer(CArray<CPoint, CPoint> *pArray, 
	                      int x0, int x_max, int y0, int y_max)
{
  CPoint cp;
  Point2D pt;
  values.empty();

  //---- Load data 
  for(int i=0; i<pArray->GetCount(); i++)
  {
	pt.x = pArray->GetAt(i).x;
	pt.y = pArray->GetAt(i).y;
	//----
	values.push_back(pt);
  }
 
  //Clean old buffer data for fill resized values
  pArray->RemoveAll();
  
  CalcSpline();
  CSegment sg;
  
  int count = values.size(); 
  for(int i=0; i<count-1; i++)
  {
	 sg.points[0] = bezier[i].points[0];
	 sg.points[1] = bezier[i].points[1];
	 sg.points[2] = bezier[i].points[2];
	 sg.points[3] = bezier[i].points[3];

	 for(int j=0; j<x_max; j++)
	 {
	   sg.Calc((double)j/(double)x_max, p);
 	   //----
	   //TRACE1("Step: %d  ", j);
	   //TRACE2("X=%f  Y=%f  ", p.x, p.y);

	   //находим ближайшую дискретную точку
	   unsigned short y_short = (unsigned short)floor(p.y+(double)0.5);
	   //----
	   //TRACE2("Y[%d]=%d\n", j, y_short);
	   
	   //временный буфер дл€ перегрузки в массив
	   cp.x = j; 
	   cp.y = y_short;
	   pArray->Add(cp);
	 }
  }
}

//------------------------------------------------------------------------------
//приводит значени€ к ближайшим целым значени€м
//берЄм значени€ и возвращаем результат в массив точек 
//------------------------------------------------------------------------------ 
void CINTP::BezierResizeA(short *pBuf, CArray<CPoint, CPoint> *pArray, 
	                       int width, int height)
{
  CPoint cp;
  Point2D pt;
  values.empty();
  double var;
  
  //---- Load data 
  for(int i=0; i<pArray->GetCount(); i++)
  {
	pt.x = pArray->GetAt(i).x;
	pt.y = pArray->GetAt(i).y;
	//----
	values.push_back(pt);
  }
   
  CalcSpline();
  CSegment sg;
  
  int count = values.size(); 
  for(int i=0; i<count-1; i++)
  {
	 sg.points[0] = bezier[i].points[0];
	 sg.points[1] = bezier[i].points[1];
	 sg.points[2] = bezier[i].points[2];
	 sg.points[3] = bezier[i].points[3];

	 int index = 0;
	 for(int j=0; j<width; j++)
	 {
	   var = (double)j;
	   var /=(double)width;
	   sg.Calc(var, p);

 	   //----
	   //TRACE1("Step: %d  ", j);
	   //TRACE2("X=%f  Y=%f  ", p.x, p.y);
	   //float y_db = (float)p.y;

	   //p.y = 200;
	   //делаем ресайз по Y
		  //TRACE2("Y[%d]=%f\n", index, y_db);
	   //находим ближайшую дискретную точку Y
	   short y_short = (short)p.y;//(unsigned short)floor(y_db+(float)0.5);
	     //TRACE2("Y[%d]=%d\n", index, y_short);
	   pBuf[index] = y_short;
	   index++;
	   //----
	   //TRACE2("Y[%d]=%d\n", j, y_short);
	 }
  }
}

//------------------------------------------------------------------------------
//делает интерпол€цию и сглаживание кривой в массиве того же размера 
//------------------------------------------------------------------------------ 
void CINTP::BezierInterpolation(CArray<CPoint, CPoint> *pArray)
{
  CPoint cp;
  Point2D pt;
  values.empty();
  const int array_count = pArray->GetCount();

  //---- Load data 
  for(int i=0; i<array_count; i++)
  {
	pt.x = pArray->GetAt(i).x;
	pt.y = pArray->GetAt(i).y;
	//----
	values.push_back(pt);
  }
 
  //Clean old buffer data for fill resized values
  pArray->RemoveAll();
  
  CalcSpline();
  CSegment sg;
  
  int count = values.size(); 
  for(int i=0; i<count-1; i++)
  {
	 sg.points[0] = bezier[i].points[0];
	 sg.points[1] = bezier[i].points[1];
	 sg.points[2] = bezier[i].points[2];
	 sg.points[3] = bezier[i].points[3];

	 for(int j=0; j<array_count; j++)
	 {
	   sg.Calc((double)j/(double)array_count, p);
 	   //----
	   TRACE1("Step: %d  ", j);
	   TRACE2("X=%f  Y=%f  ", p.x, p.y);

	   //находим ближайшую дискретную точку
	   unsigned short y_short = (unsigned short)floor(p.y+(double)0.5);
	   //----
	   TRACE2("Y[%d]=%d\n", j, y_short);
	   
	   //временный буфер дл€ перегрузки в массив
	   cp.x = j; 
	   cp.y = y_short;
	   pArray->Add(cp);
	 }
  }
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------ 
void CINTP::Bezier(double resolution)
{
  CPoint cp;
  Point2D pt;
  
  //Check data 
  int count = values.size(); 
  if(count<2) return;
  
  output.empty();

  CalcSpline();
  CSegment sg;
  
  for(int i=0; i<count-1; i++)
  {
	 sg.points[0] = bezier[i].points[0];
	 sg.points[1] = bezier[i].points[1];
	 sg.points[2] = bezier[i].points[2];
	 sg.points[3] = bezier[i].points[3];

	 for(int j=0; j<resolution; j++)
	 {
	   sg.Calc((double)j/(double)resolution, p);
 	   
	   output.push_back(p);

	   //----
	   TRACE2("X=%f  Y=%f\n", p.x, p.y);
	 }
  }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CINTP::CalcSpline(void)
{  
  //----
  int n = values.size() - 1;
  if(n<2) return -1;

  bezier.resize(n);

  Point2D tgL;
  Point2D tgR;
  Point2D cur;
  Point2D next;
  next = values[1] - values[0];
  next.normalize();

  double l1, l2, tmp, x;
 
  for(int i=0; i<n; ++i)
  {
    bezier[i].points[0] = bezier[i].points[1] = values[i];
    bezier[i].points[2] = bezier[i].points[3] = values[i+1];

    tgL = tgR;
    cur = next;

    if(i+1 < n)
    {
      next = values[i+2] - values[i+1];
      next.normalize();

      tgR = cur + next;
      tgR.normalize();	
    }
    else
    {
      tgR.x= 0.0;
      tgR.y= 0.0;
    }

    if(abs(values[i+1].y - values[i].y) < EPSILON)
    {
      l1 = l2 = 0.0;
    }
    else
    {
      tmp = values[i+1].x - values[i].x;
      l1 = abs(tgL.x) > EPSILON ? tmp/(2.0 * tgL.x):1.0;
      l2 = abs(tgR.x) > EPSILON ? tmp/(2.0 * tgR.x):1.0;
    }

    if(abs(tgL.x) > EPSILON && abs(tgR.x) > EPSILON)
     {
      tmp = tgL.y / tgL.x - tgR.y / tgR.x;
      if(abs(tmp) > EPSILON)
       {
         x = (values[i+1].y - tgR.y / tgR.x * values[i+1].x - 
              values[i].y + tgL.y / tgL.x * values[i].x) / tmp;
         
         if(x > values[i].x && x < values[i+1].x)
          {
            if(tgL.y > 0.0)
             {
              if(l1 > l2)
                 l1 = 0.0;
              else
                 l2 = 0.0;
             }
            else
             {
              if(l1 < l2)
                 l1 = 0.0;
              else
                 l2 = 0.0;
             }
          }
        }
      }

   bezier[i].points[1] += tgL * l1;
   bezier[i].points[2] -= tgR * l2;
 }
 
 return 1;
}
