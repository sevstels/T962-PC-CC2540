//==============================================================================
//File name:   "graphs.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _GRAPHS_H_
#define _GRAPHS_H_

#include <string>
#include "interpolation.h"
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
  void SetRangeX(int from, int to);
  void SetRangeY(int from, int to);
  void SetScaleX(int from, int to, int step);
  void SetScaleY(int from, int to, int step);
  void SetSize(int x, int y);

  //---
  void SetMulX(double factor_x);
  void SetMulY(double factor_y);
  void SetFont(CDC *pCDC, int Size, BOOL Bold, BOOL Italic);
  void Normalization(void);
  int  ResizeDataX(std::vector<PointXY> *pBufOut, std::vector<Point2D> *pBufIn);
  int  ResizeDataY(std::vector<PointXY> *pBufOut, std::vector<Point2D> *pBufIn);
  int  ParserDots(std::vector<Point2D> *pData);
  float Calc_Signal(void);
  float Calc_Signal2(void);
  void  Calc_LinePoints(short *pBuf, int x1, int y1, int x2, int y2);
  int   round(double value);
  int Recognition(CDC *pCDC, int signal_length);

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


  //------------------------------------
  //DSP
  //------------------------------------
  CRect WindGraph_Rect;
  CPoint p_control, p_test;
  int  DSP_FindMaxs(short *pBuf, int x_from, int x_to);
  int  DSP_ShowAllMaxs(CDC *pCDC, short *pBuf, int x_from, int x_to);
  
  //----
  void DisplayMax(CDC *pCDC, CPoint point, CString txt);
  void DisplayMaxLable(CDC *pCDC, CPoint point, CString txt);

  void DisplayList(CDC *pCDC, int x, int y, int buf_position);
  void Draw_MaxControl(CDC *pCDC, int x, int y, CString txt);
  void Draw_SearchFrame(CDC *pCDC, int x, int from, int to);
  void Draw_Report(CDC *pCDC, int x, int y, CString txt);
  void Draw_MsgBox(CDC *pCDC, int x0, int y0, int dx, int dy, CString txt, int yes_no);
  void Draw_Point(CDC *pCDC, int buf_position);
  int  Draw_0707(CDC *pCDC, int x_max);
  void Draw_Recognized_C(CDC *pCDC, int x, int y);
  void Draw_Recognized_T(CDC *pCDC, int x, int y);
  void Draw_DetectionLine(CDC *pCDC);
  void Draw_Circle(CDC *pCDC, int x, int y, int d, COLORREF color);
  void Draw_PointBox(CDC *pCDC, int buf_position);
  CRect Draw_StringAt(CDC *pDC, int x, int y, 
					  CString txt, int height, COLORREF color);
  //------------------------------------
  //vars
  HANDLE hWnd;
  HANDLE hDC;
  std::string last_file;
  
  //----
  int max_control_x;
  int max_control_y; 
  int min_control_y[2];
  int min_control_x[2];
  int control_offset;

  //----
  int max_test_y;
  int max_test_x;
  int med_test_x;
  int min_test_y[2];
  int min_test_x[2];
  int test_offset;

  int signal_detected;
  int find_control_line;
  int find_test_line;
  int control_line_length;
  int test_line_length;

  //----
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
  
  int picture_heigth;
  int picture_width;

  int find_Min(short *pBuff, int from, int to, int &x, int &y);
  int find_Max(short *pBuff, int from, int to, int &x, int &y);
  int find_LocalMin(short *pBuff, int from, int to, int &x, int &y);
  int find_Cross(short *pBuff, int from, int to, int value, int &x, int &y);
  //----
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

  char graph_line_width[6];
  std::vector<PointXY> graph_line[6];
  COLORREF graph_line_color[6];
  COLORREF graph_bgnd_color;

  //----
  char save_bmp;
  char save_jpg;
  char save_gif;
  char save_png;
  char save_svg;
};

#endif