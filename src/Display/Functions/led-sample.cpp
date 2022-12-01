//==============================================================================
//File name:    "led-sample.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
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

/*
//------------------------------------------------------------------------------
//Draw
//------------------------------------------------------------------------------
void CDSPL::Draw_SampleLED(int x, char leds)
{ 
  if(leds==0) return;
	
  COLORREF color;

  if(leds==1) color = RGB(255,0,0);
  if(leds==2) color = RGB(0,255,0);
  if(leds==3) color = RGB(0,0,255);
  if(leds==4) color = RGB(255,255,0);
  color = RGB(255,0,0);

  Draw_SensorSQR(x, color);
} */

//------------------------------------------------------------------------------
//Draw
//------------------------------------------------------------------------------
void CDSPL::Draw_SampleLED(int index, short leds, float fl_y)
{ 
  //if(leds==0) return;
  
  CRect rect;
  CPoint center;
  
  int x, y;
  
  //Прочитать залитое ранее значение у-линии 
  x = (int) profile[index].x;
  //fl_y = profile[index-1].y;
  //fl_y = profile[index].y;

  /*
  rect.SetRect(0, 0, 6, 6);
  center = rect.CenterPoint();
  x = x - center.x;
  y = y - center.y;
  IMG.FillSolidRect(x,y,6,6, RGB(0,0,0));
  */

  COLORREF color; 
  if(leds)color = RGB(100,0,0);
  else color = RGB(100,100,100);
  color = RGB(100,0,0);

  int r1, r2;
  r1 = r2 = 8;
  
  //Рисуем кружок с красным бордюром (сменим перо)
  //и закрашенной областью (сменим кисть).
  CPen *pOldPen, NewPen;
  CBrush *pOldBrush, NewBrush;
  
  if(object_selected &&
	 object_index == index-1 &&
	 object_type == SELECT_DRAGPOINT) 
  {  
	NewPen.CreatePen(PS_SOLID, 1, color); // RGB(255, 0, 0)
	NewBrush.CreateSolidBrush(color);// RGB(255,0,0)
	r1 = r2 = 8;
  }
  else 
  {
	NewPen.CreatePen(PS_SOLID, 1, color);//RGB(0, 0, 0)
	NewBrush.CreateSolidBrush(color);//RGB(100,100,100)	
  }

  pOldPen = IMG.SelectObject(&NewPen);
  pOldBrush = IMG.SelectObject(&NewBrush);
  
  //Преобразовать в координаты на дисплее
  Overflow_Correction(x, fl_y, 2048);
  Convert_ToDisplay_XY(y, x, fl_y);

  //draw
  IMG.Ellipse(x-r1/2-scroll_position, y-r2/2, x+r1/2-scroll_position, y+r2/2);
  
  //Восстанавливаем старые перо и кисть.
  IMG.SelectObject(pOldPen);
  IMG.SelectObject(pOldBrush);
}


//------------------------------------------------------------------------------
//Draw
//------------------------------------------------------------------------------
void CDSPL::AddSensor(int x, int channel)
{ 
  Point2D p;
  int count = profile.size();

  //save keypoints 
  for(int i=0; i<count-1; i++)
  {
	int x1 = (int)profile[i].x;
	int x2 = (int)profile[i+1].x;

//	if(x1<=x && x<=x2) profile.at(x,p);  //.push_back(p);
	TRACE2("\nAdd Sensor: X:%d Chn:", x, channel);
  }

  //Draw_SensorSQR(x, color);
}
