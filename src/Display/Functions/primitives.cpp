//==============================================================================
//File name:    "primitives.cpp"
//Purpose:      Source File, CPage2 property page
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "display.h"
#include <math.h>
#include "Arrows.h"

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
//Draw line
//------------------------------------------------------------------------------
void CDSPL::Draw_Line(CDC *pDC, char width, int x0, int y0, 
                      int x, int y, COLORREF color)
{
  //Set parameters for Pancil
  CPen Pen; 
  
  //New pen
  Pen.CreatePen(PS_SOLID, width, color);   

  //Save first pen pointer, select new pancil
  CPen *pOldPen = pDC->SelectObject(&Pen); 
 
  //Jump to start point 
  pDC->MoveTo(x0, y0); //не попадает в окно если тут 0

  //Draw curve line
  pDC->LineTo(x, y);//не попадает в окно если тут 0
 
  //----- delete all Pen
  //Set old Pen to DC 
  pDC->SelectObject(pOldPen);
  Pen.DeleteObject();
}	

//------------------------------------------------------------------------------
//Draw frame
//------------------------------------------------------------------------------
void CDSPL::Draw_Frame(CDC *pDC, char width, int x0, int y0, 
                       int x, int y, COLORREF color)
{
  //Set parameters for Pancil
  CPen Pen; 
  
  //New pen
  Pen.CreatePen(PS_DOT, width, color);   

  //Save first pen pointer, select new pancil
  CPen *pOldPen = pDC->SelectObject(&Pen); 
 
  //Jump to start point 
  pDC->MoveTo(x0, y0);

  //Draw curve line
  pDC->LineTo(x, y0);
  pDC->LineTo(x, y);
  pDC->LineTo(x0, y);
  pDC->LineTo(x0, y0);

  //----- delete all Pen
  //Set old Pen to DC 
  pDC->SelectObject(pOldPen);
  Pen.DeleteObject();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::Draw_StringAt(CDC *pDC, int x, int y, CString txt, int height, 
                          int angle, COLORREF txt_color, COLORREF bgd_color)
{ 
 //set font
 CFont font, old_font;
 LOGFONT font_setup = {height,0,0,0, FW_BOLD, 0,0,0, DEFAULT_CHARSET,
                       OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                       PROOF_QUALITY, VARIABLE_PITCH | FF_DONTCARE,"Arial"}; 
 
 font_setup.lfEscapement = angle*10;
 font_setup.lfOrientation = angle*10;
 font.CreateFontIndirect(&font_setup);
 
 //назначаем текущим ранее подготовленный шрифт
 CFont *pOldFont = pDC->SelectObject(&font);

 //-----------------------------------------
 //Print digit horizontally
 //-----------------------------------------
 pDC->SetTextColor(txt_color);
 
 if(bgd_color==TRANSPARENT) pDC->SetBkMode(TRANSPARENT);
 else{pDC->SetBkMode(OPAQUE); pDC->SetBkColor(bgd_color);}

 pDC->TextOutA(x, y, txt);

 //---- delete graphic objects
 font.DeleteObject();

 pDC->SelectObject(pOldFont);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::Draw_Box(CDC *pCDC, int x0, int y0, int dx, int dy, COLORREF color)
{  
  //Fill BGnd
  pCDC->FillSolidRect(x0, y0, dx, dy+2, color);
  
  //Window edge line верхн€€ гориз
  Draw_Line(pCDC, 1, x0, y0, x0+dx, y0, RGB(0,0,0)); 
  //Window edge line нижн€€ гориз
  Draw_Line(pCDC, 1, x0, y0+dy+2, x0+dx, y0+dy+2, RGB(0,0,0));
  //Window edge line вертикальна€ лева€
  Draw_Line(pCDC, 1, x0, y0, x0, y0+dy+2, RGB(0,0,0)); 
  //Window edge line вертикальна€ права€
  Draw_Line(pCDC, 1, x0+dx, y0, x0+dx, y0+dy+2, RGB(0,0,0));
}

//-----------------------------------------------------------------------------
//Paint Searh Maximum Point
//-----------------------------------------------------------------------------
void CDSPL::Draw_MaxPoint(CDC *pDC, float *pBuffer, int size)
{
 CPoint position;
 position.x = 0; 
 position.y = 255; //127 - Midle level line
 
 int x_left = 20*8;
 int x_right = 20*8 + size;

 CPen pen_gray;
 pen_gray.CreatePen(PS_SOLID, 1, RGB(120,120,120));

 //---- Write Obserwation area line
 CPen *pOldPen = pDC->SelectObject(&pen_gray);		//select pancil
 pDC->MoveTo(position.x+x_left-1, position.y);		//Start point
 pDC->LineTo(position.x+x_left-1, position.y-200);	//draw
 pDC->MoveTo(position.x+x_right+1, position.y);		//Start point
 pDC->LineTo(position.x+x_right+1, position.y-200);	//draw
 
 //left/right arrows
 ARROWSTRUCT ARROW;
 ARROW.nWidth = 5;
 ARROW.fTheta = 0.4f;
 ARROW.bFill = true;
 ARROW.color = RGB(160,160,160);
 
 pDC->MoveTo(position.x+x_left-20, position.y-190);
 ArrowTo(pDC->m_hDC, x_left, position.y-190,&ARROW);
 pDC->MoveTo(position.x+x_right+20, position.y-190);
 ArrowTo(pDC->m_hDC, x_right, position.y-190,&ARROW);

 //---- Search Max Point
/// CBrent BR;
int x = 0;
 
/// int max_y = 255-(int)BR.BrentOptimize(pBuffer, x_left, x_right, 0.5, x);
 int max_y = 0;
 int max_x = (int)x;
 
 //check overhead 
 //if(max_y > 80)
  {
    //msg1 = "Y max  = ";
    //double tmp = -1*yy/255.0;	
    //msg2.Format("%.3f",tmp);
    //memCUR.TextOutA(120,-45, msg1+msg2);	

    //msg1 = "X point = ";
    //tmp = xx * 0.1875;
    //msg2.Format("%.2f",tmp);
    //memCUR.TextOut(120,-30, msg1+msg2);

    //write text message
    CString txt = "max";
    pDC->SetTextColor(RGB(0,0,0));
    pDC->SetBkColor(RGB(255,255,111));//ETO_OPAQUE
    pDC->ExtTextOutA(position.x+max_x+4,position.y-max_y-40,ETO_CLIPPED,NULL,txt,NULL);

    //write arrow
    ARROW.color = RGB(255,0,0);
    pDC->MoveTo(position.x+max_x+8, position.y-(max_y+16));
    ArrowTo(pDC->m_hDC, position.x+max_x, position.y-max_y,&ARROW);
  }

 //----- delete all Pen
 //¬ыбираем старый Pen в DC (освобождаем свой Pen из DC)
 pDC->SelectObject(pOldPen);
 pen_gray.DeleteObject();

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CDSPL::Draw_CycleArrow(CDC *pCDC)
{
 //left-right arrows setup
 ARROWSTRUCT arrow;
 arrow.nWidth = 5;
 arrow.fTheta = 0.5f;
 arrow.bFill = true;
 arrow.color = RGB(190,190,190);
 
 int step = 0;
 int count = index_cycles.GetCount();
 count >>= 1;

 for(int i=0; i<count; i++)
 {

  int index0 = index_cycles.GetAt(step++);
  int index1 = index_cycles.GetAt(step++);

  //-----
  int x1 = (int)profile[index0].x;
  int x = x1;
  int y = 46;

  //Left arrow 
  Convert_ToDisplay_XY(y, x, y);
  pCDC->MoveTo(x+1+20-scroll_position, y);
  ArrowTo(pCDC->m_hDC, x-scroll_position, y, &arrow);
 
  int x2 = (int)profile[index1].x;
  x = x2;
  y = 46;

  //Right arrow 
  Convert_ToDisplay_XY(y, x, y);
  pCDC->MoveTo(x-1-20-scroll_position, y);
  ArrowTo(pCDC->m_hDC, x-scroll_position, y, &arrow);

  //Write text message
  int x3 = x2-x1;
  x3 >>= 1;
  x3 += x1;
  y = 46;

  Convert_ToDisplay_XY(y, x3, y);
  CString txt;
  txt.Format("cycle %d",i+1);
  Draw_StringAt(&IMG, x3-16-scroll_position, y+7, 
                 txt, 13,0, RGB(80,80,80), TRANSPARENT);   
 }
}

//------------------------------------------------------------------------------
//For Screen draw Line
//------------------------------------------------------------------------------
void CDSPL::Calc_LinePoints(int x1, int fl_y1, int x2, int fl_y2)
{
 //округлить к ближайшему целому по Y
 int y1 = round(fl_y1);
 int y2 = round(fl_y2);

 const int deltaX = abs(x2 - x1);
 const int deltaY = abs(y2 - y1);
 const int signX = x1 < x2 ? 1 : -1;
 const int signY = y1 < y2 ? 1 : -1;
 
 //
 int error = deltaX - ((int)deltaY);
 //
 ProfileBuf[x2] = (float)y2;
 while(x1 != x2 || y1 != y2) 
 {
   ProfileBuf[x1] = (float)y1;
   const int error2 = error * 2;
   //
   if(error2 > -deltaY) 
   {
     error -= deltaY;
     x1 += signX;
   }
   if(error2 < deltaX) 
   {
     error += deltaX;
     y1 += signY;
   }
 }
}

//------------------------------------------------------------------------------
// For Temperature buffer draw Line
//------------------------------------------------------------------------------
void CDSPL::Calc_LinePointsT(int x1, float fl_y1, int x2, float fl_y2)
{
 //
 const double deltaX = abs(x2 - x1);
 const int signX = x1 < x2 ? 1 : -1;

 double stepY = 0; 
 if(fl_y1<fl_y2) {stepY = fl_y2-fl_y1; stepY /= deltaX;} 
 if(fl_y1>fl_y2) {stepY = fl_y1-fl_y2; stepY /= deltaX; stepY *= -1;}
  
 //last data point value
 ProfileBuf[x2] = fl_y2;

 /* какой то глюк тут !!!
 while(x1 != x2) 
 {
   //Save point
   ProfileBuf[x1] = fl_y1;
   fl_y1 += (float) stepY;
   x1 += signX;
 } */
}