//==============================================================================
//File name:    ".cpp"
//Purpose:      Source File, CPage2 property page
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "display.h"
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

//------------------------------------------------------------------------------
//Draw
//------------------------------------------------------------------------------
void CDSPL::Draw_SensorSQR(int x, COLORREF color)
{ 
  int cx, cy;
  int line_width;
  CRect rect;
  CPoint center;

  short	y = ProfileBuf[x];

  rect.SetRect(0, 0, 8, 16);
  center = rect.CenterPoint();

  x = x - center.x;
  y = y - center.y;

  IMG.FillSolidRect(x,y,8,16, color);
  //IMG.Rectangle(x,y,8,16);
  //IMG.DrawFocusRect(x,y,8,16);
}

//------------------------------------------------------------------------------
//Draw
//------------------------------------------------------------------------------
void CDSPL::Draw_DragPoints(int index)
{ 
  int x, y;
  float fl_y;
  int line_width;
  double value;
  CRect rect;
  CPoint center;

  x = profile[index].x;
  fl_y = profile[index].y;
  /*
  rect.SetRect(0, 0, 6, 6);
  center = rect.CenterPoint();
  x = x - center.x;
  y = y - center.y;
  IMG.FillSolidRect(x,y,6,6, RGB(0,0,0));
  */
  int r1, r2;
  r1 = r2 = 4;
  
  //Рисуем кружок с красным бордюром (сменим перо)
  //и закрашенной областью (сменим кисть).
  CPen *pOldPen, NewPen;
  CBrush *pOldBrush, NewBrush;
  
  if(object_selected &&
	 object_index == index-1 &&
	 object_type == SELECT_DRAGPOINT) 
  {  
	NewPen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	NewBrush.CreateSolidBrush(RGB(255,0, 0));
	r1 = r2 = 6;
  }
  else 
  {
	NewPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	NewBrush.CreateSolidBrush(RGB(100,100,100));	
  }

  pOldPen = IMG.SelectObject(&NewPen);
  pOldBrush = IMG.SelectObject(&NewBrush);
  
  //Преобразовать в координаты на дисплее
  ///Overflow_Correction(x, fl_y, 2048);
  Convert_ToDisplay_XY(y, x, fl_y);

  //Limits  
  //TRACE1("off: %d ", offset); 
  //TRACE2("x: %d  y: %d\n", x, y);  

  if(((x-scroll_position)>=scale_offset_x) &&
	 ((x-scroll_position)<=(scale_width_x+scale_offset_x)))
  {
    //draw
    IMG.Ellipse(x-r1/2-scroll_position, y-r2/2, x+r1/2-scroll_position, y+r2/2);
  }

  //Восстанавливаем старые перо и кисть.
  IMG.SelectObject(pOldPen);
  IMG.SelectObject(pOldBrush);
}
