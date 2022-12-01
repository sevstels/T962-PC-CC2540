//==============================================================================
//File name:   "Images.h"
//Purpose:      Header File
//Version:      3.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _DRAW_IMAGES_H_
#define _DRAW_IMAGES_H_

#define RGB_YELLOW	RGB(0xff,0xff,0)
#define RGB_BLUE	RGB(0,0,0xff)

#pragma once
class CImages
{
  public:
  CImages(void);
 ~CImages(void);
    
  CRect Window_Rect;
  CRect WindGraph_Rect;	

  CDC BGND;
  CDC CURVE;
  CDC WINDOW;

  CBitmap bmp_BackGround;
  CBitmap bmp_Curve;
  CBitmap bmp_Window;

  CPoint xy0;

  int  Create_CDC(CDC *pDC);   	
  void Draw_Window(CDC *pDC, HWND hWnd);

  void Draw_BackGround(void);
  void Draw_Scale_X(CDC *pCDC, int x0, int y0, int size_x, int size_y,
					  int line_number, COLORREF color);
  void Draw_Scale_Y(CDC *pCDC, int x0, int y0, int size_x, 
		            int size_y, int line_number, COLORREF color);
  void Clear_Curve(void);

  void Draw_Curve(char *pBuf, char width, 
				  int x_from, int x_to, COLORREF color);

  void Draw_SearchFrame(CDC *pDC, int x, int from, int to);
  CRect Draw_StringAt(CDC *pDC, int x, int y, CString txt,  
		               int height, COLORREF color);


  void Display_XYZ(CDC *pDC, int x, int y, int z);
  void Display_G(CDC *pDC, unsigned char G, unsigned char alarm_level);
  void Curve_ModuleG(CDC *pDC, unsigned char *pBuffer);
  void MaxPoint(CDC *pDC, unsigned char *pBuffer, int size);
  void Paint_Power_Vector(CDC *pDC, CPoint center, int angle);
  
  void DrawCurveS(CDC *pDC, short *pBuf, char line_width, 
		                          int x0, int y0, 
				                  int x_from, int x_to, 
						          COLORREF color);

  void DrawCurve(CDC *pDC, char *pBuf, char line_width, 
		                   int x0, int y0, 
				           int x_from, int x_to, 
						   int h_scale, int v_scale,
						   COLORREF color);

  void DrawLine(CDC *pDC, char width, int x0, int y0, 
		                    int x, int y,  COLORREF color);

  void Draw_Edge(CDC *pCDC, int x0, int y0, int dx, int dy, COLORREF color);
  void Draw_Edge(CDC *pCDC, CRect rect, COLORREF color);
	
  //sensitivity levels
  struct _SENSE
	{
	 unsigned char x;
	 unsigned char y;
	 unsigned char z;
     unsigned char g;
	} SENSE;
	
	//vibration noise levels
  struct _NOISE
	{
	 unsigned char x;
	 unsigned char y;
	 unsigned char z;
  } NOISE;

  protected:
  CPoint Ellipse_xy(CRect ellipse, int angle);
};

#endif