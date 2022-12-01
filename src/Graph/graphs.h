//==============================================================================
//File name:   "graphs.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _GRAPHS_H_
#define _GRAPHS_H_

#include <string>
#include "interpolation.h"

#define GRPH_WIDTH_AUTO -11 

//------------------------------------------------------------------------------
//CGraph class
//------------------------------------------------------------------------------
class CGraphs
{
  public:
  //---- Construction
  CGraphs();
 ~CGraphs();
  
  //----
  void SetHWnd(HANDLE hHWnd);
  void SetRangeX(int from, int to);
  void SetRangeY(int from, int to);
  //---
  void SetMulX(double factor_x);
  void SetMulY(double factor_y);

  void Normalization(void);
  int  Parser(std::vector<Point2D> *pProfile);
  //int  ParserDots(short *pBuf, int length);
  int  ParserDots(std::vector<Point2D> *pData);
  void Calc_LinePoints(short *pBuf, int x1, int y1, int x2, int y2);

  //Save to *_x.png file
  int CreateGraph(int height, int width, const char *pFileName);
  int ShowGraph(const char *pFileName);
  
  void Draw_Line(CDC *pDC, char width, int x0, int y0, 
			     int x, int y, COLORREF color);

  void Draw_ScaleX(CDC *pCDC, int x0, int y0, 
	               int width, int height,
		           int line_number, COLORREF color);

  void Draw_ScaleY(CDC *pCDC, int x0, int y0, 
	               int width, int height, 
	               int line_number, COLORREF color);

  void Draw_Curve(CDC *pDC, short *pBuf, char line_width, 
		          int x0, int y0, int x_from, int x_to, COLORREF color);

  protected:
  short Buf[80000];
  short *pBuf;
  
  //Additional user curves
  short Buf2[80000];
  short *pBuf2;
  int curve_length2;

  //vars
  HANDLE hWnd;
  HANDLE hDC;
  std::string last_file;

  int max;
  int min;
  int range_x0, range_xn;
  int range_y0, range_yn;
  int curve_height;
  int curve_length;
  int file_number;
  int normalize;
  double resize_factor_x;
  double resize_factor_y;

  int find_Min(signed short *pBuff, int from, int to, int &x, int &y);
  int find_Max(signed short *pBuff, int from, int to, int &x, int &y);
};

#endif