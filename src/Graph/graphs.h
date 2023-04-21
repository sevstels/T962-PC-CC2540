//==============================================================================
//File name:   "graphs.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _GRAPHS_H_
#define _GRAPHS_H_

#include <string>
#include "arrays.h"
#include "app-setup.h"
#include "device_struct.h"

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
  
  ApplSettings *pAPP;
  Device_Setup *pDEV;

  //----
  void SetHWnd(HANDLE hHWnd);
  void SetSize(int x, int y);
  void SetRangeX(int from, int to);
  void SetRangeY(int from, int to);
  void SetScaleX(int from, int to, int step);
  void SetScaleY(int from, int to, int step);
  void ResizeImgX(float factor_x);
  void ResizeImgY(float factor_y);
  void FlToInt(std::vector<PointINT> *pBufOut, 
	           std::vector<PointFLT> *pBufIn);
  //---
  int  ResizeDataY(std::vector<PointINT> *pBufOut, std::vector<PointINT> *pBufIn);
  int  ResizeDataX(std::vector<PointINT> *pBufOut, std::vector<PointINT> *pBufIn);
  int  ParserDots(std::vector<PointFLT> *pData);
  void SetFont(CDC *pCDC, int Size, BOOL Bold, BOOL Italic);
  void Calc_LinePoints(short *pBuf, int x1, int y1, int x2, int y2);
  int  round(double value);

  //Save to *_x.png file
  int CreateGraph(const char *pFileName);
  int ShowGraph(const char *pFileName);
  
  void Draw_Edge(CDC *pCDC, int x0, int y0, int dx, int dy, COLORREF color);

  void Draw_Line(CDC *pDC, char width, int x0, int y0, 
			     int x, int y, COLORREF color);

  void Draw_ScaleX(CDC *pCDC, int x0, int y0);

  void Draw_ScaleY(CDC *pCDC, int x0, int y0, 
	               int width, int height);

  void Draw_Curve(CDC *pDC, short *pBuf, char line_width, 
		          int x0, int y0, int x_from, int x_to, COLORREF color);

  void Draw_String(int x, int y, CString txt, COLORREF color);

  //------------------------------------
  //DSP
  //------------------------------------
  void Draw_Info(void);

  void Draw_MsgBox(CDC *pCDC, int x0, int y0, int dx, int dy, CString txt, int yes_no);
  void Draw_Point(CDC *pCDC, int buf_position);
  void Draw_Circle(CDC *pCDC, int x, int y, int d, COLORREF color);
  void Draw_PointBox(CDC *pCDC, int buf_position);
  CRect Draw_StringAt(CDC *pDC, int x, int y, 
					  CString txt, int height, COLORREF color);
  //------------------------------------
  //vars
  CDC *pCDC;
  HANDLE hWnd;
  HANDLE hDC;
  std::string last_file;
  
  //----
  int max;
  int min;
  int range_x0, range_xn;
  int range_y0, range_yn;
  int curve_height;
  int curve_length;
  int file_number;
  double resize_factor_x;
  double resize_factor_y;
  float img_resize_x;
  
  int picture_heigth;
  int picture_width;
  int scale_length_x;
  int scale_length_y;

  //value range
  int scale_x_begin;
  int scale_x_end;
  int scale_x_cell;
  int scale_y_begin;
  int scale_y_end;
  int scale_y_cell;
  int scale_line_width;
  COLORREF scale_line_color;

  //----
  int graph_max_value_x;
  int graph_max_value_y;

  std::vector<PointINT> graph_line[10];
  COLORREF graph_bgnd_color;

  //----
  char save_bmp;
  char save_jpg;
  char save_gif;
  char save_png;
  char save_svg;
};

#endif