//==============================================================================
//File name:   "anls-graph.h"
//Purpose:      Header File
//Version:      2.00
//Copyright:    (c) 2021, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _ANALYS_GRAPH_2_H_
#define _ANALYS_GRAPH_2_H_

#include <string>
#include "interpolation.h"

#define  GRAPH_LOG       -10
#define  GRPH_WIDTH_AUTO -11
#define  GRPH_HEGHT_AUTO -12
#define  GRAPH_CHANNELS   6

#pragma pack(push, 1)
typedef struct struct_analys 
{
  unsigned short x;
  unsigned short ch[4][GRAPH_CHANNELS];
}sanalys;
#pragma pack(pop)

//------------------------------------------------------------------------------
//CGraph class
//------------------------------------------------------------------------------
class CAnGraph
{
  public:
  //---- Construction
  CAnGraph();
 ~CAnGraph();
  
  //----
  void SetHWnd(HANDLE hHWnd);
  void SetRangeX(int from, int to, int columns);
  void SetRangeY(int from, int to, int rows);
  //---
  void SetMulX(double factor_x);
  void SetMulY(double factor_y);

  void Normalization(void);
  int  Parser(CWordArray *pX, CWordArray *pY, int mode);
  int  Parser(CArray <struct struct_analys, struct struct_analys> *pArr);
  void Calc_LinePoints(int x1, int y1, int x2, int y2);

  //Save to *_x.png file
  int  CreateGraph(int width, int height, const char *pFileName);
  int  ShowGraph(const char *pFileName);
  
  void Polynom_Line(short *pBufOut, CArray<CPoint, CPoint> *pArray, 
	                int width, int height, int flag_normalize);

  void Normalize_Y(CArray<CPoint, CPoint> *pArray, 
	               int width, int height, 
				   int img_width, int img_height);

  void Resize_Curve(short *pBufOut, CArray<CPoint, CPoint> *pArray, 
	                int width, int height, int y_normalize);
  
  void Bezier_Resize(short *pBufOut, CArray<CPoint, CPoint> *pArray, 
	                 int width, int height);

  void Draw_Line(CDC *pDC, char width, int x0, int y0, 
			     int x, int y, COLORREF color);

  void Draw_ScaleX(CDC *pCDC, int x0, int y0, 
	               int width, int height, COLORREF color);

  void Draw_ScaleY(CDC *pCDC, int x0, int y0, 
	               int width, int height, COLORREF color);

  void Draw_Curve(CDC *pDC, short *pBuf, char line_width, 
		          int x0, int y0, int x_from, int x_to, COLORREF color);
  
  //----
  CArray<struct struct_analys, struct struct_analys> Analysis;
  
  //Data points array
  CArray<CPoint, CPoint> ArrCh[GRAPH_CHANNELS];

  protected:
  short Buf[80000];
  short *pBuf;
  int sample_count;

  //vars
  HANDLE hWnd;
  HANDLE hDC;
  std::string last_file;

  int max;
  int min;
  int range_x0, range_x_max;
  int range_y0, range_y_max;
  int columns_x, rows_y;
  int scale_height_px_y;
  int scale_width_px_y;
  int curve_height;
  int curve_length;
  int file_number;
  int normalize;
  int buf_in_count;
  double resize_factor_x;
  double resize_factor_y;

  int find_Min(signed short *pBuff, int from, int to, int &x, int &y);
  int find_Max(signed short *pBuff, int from, int to, int &x, int &y);
};

#endif