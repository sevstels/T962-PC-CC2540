#include "stdafx.h"
#include "Images.h"
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
//Paint	curve
//------------------------------------------------------------------------------
void CImages::DrawCurve(CDC *pDC, char *pBuf, char line_width, 
		                          int x0, int y0, 
				                  int x_from, int x_to, 
						          int h_scale, int v_scale, 
						          COLORREF color)
{
  //---- Set parameters for Pancil
  CPen pen;
  //New pen
  pen.CreatePen(PS_SOLID, line_width, color); 

  //Save first pen pointer
  //select pancil
  CPen *pOldPen = pDC->SelectObject(&pen); 
 
  //Start point
  int x,y;

  //--------------------------------------------------------
  //Draw curve line
  //--------------------------------------------------------
  do 
   {								
	 //Calculation current point position
	 x = x0 + x_from * h_scale; 
	 y = pBuf[x_from]* v_scale + y0;
	
	 //if(value_overhead && value>=360) 
	 pDC->MoveTo(x, y);

	 //next point from data buffer
	 x_from++;

	 //Calculation next point position
	 x = x0 + x_from * h_scale; 
	 y = pBuf[x_from]* v_scale + y0;

	 if(pBuf[x_from]>0)
	  {
	    pDC->LineTo(x, y);
	  }
   } 
  while (x_from*h_scale < x_to);
 
 //----- delete all Pen
 //Set old Pen to DC 
 pDC->SelectObject(pOldPen);
 pen.DeleteObject();
}

//------------------------------------------------------------------------------
//Paint	curve for char Array
//------------------------------------------------------------------------------
void CImages::Draw_Curve(char *pBuf, char width, 
                         int x_from, int x_to, COLORREF color)
{
 
 CURVE.FillSolidRect(0,0, WindGraph_Rect.Width(), WindGraph_Rect.Height(),RGB(255,255,255));

 //---- Set parameters for Pancil
 CPen pen;
 pen.CreatePen(PS_SOLID, width, color);   //New pen

 //Save first pen pointer
 CURVE.SelectObject(&pen); //select pancil
 
 //Start point 
 CURVE.MoveTo(xy0.x+x_from, xy0.y-pBuf[x_from]);
 
 //--------------------------------------------------------
 //Draw curve line
 //--------------------------------------------------------
 do {								
	  x_from++;
	  CURVE.LineTo(xy0.x+x_from, xy0.y-pBuf[x_from]);
	} 
 while (x_from < x_to);
 
 //----- delete all Pen
 //Set old Pen to DC 
 pen.DeleteObject();
}

//------------------------------------------------------------------------------
//Paint	curve for char Array
//------------------------------------------------------------------------------
void CImages::DrawCurveS(CDC *pDC, short *pBuf, char line_width, 
		                   int x0, int y0, 
				           int x_from, int x_to, COLORREF color)
{
  //---- Set parameters for Pancil
  CPen pen;
  //New pen
  pen.CreatePen(PS_SOLID, line_width, color); 

  //Save first pen pointer
  //select pancil
  CPen *pOldPen = pDC->SelectObject(&pen); 
 
  //Start point
  int x, y;

  //--------------------------------------------------------
  //Draw curve line
  //--------------------------------------------------------
  do 
   {								
	 //Calculation current point position
	 x = x0 + x_from; 
	 y = pBuf[x_from] + y0;
	
	 //if(value_overhead && value>=360) 
	 pDC->MoveTo(x, y);

	 //next point from data buffer
	 x_from++;

	 //Calculation next point position
	 x = x0 + x_from; 
	 y = pBuf[x_from] + y0;

	 pDC->LineTo(x, y);
   } 
  while (x_from < x_to);
 
 //----- delete all Pen
 //Set old Pen to DC 
 pDC->SelectObject(pOldPen);
 pen.DeleteObject();
}

//------------------------------------------------------------------------------
//Paint	curve
//------------------------------------------------------------------------------
void CImages::Clear_Curve(void)
{
  CURVE.FillSolidRect(0,0, WindGraph_Rect.Width(), 
	    WindGraph_Rect.Height(),RGB(255,255,255));
}
