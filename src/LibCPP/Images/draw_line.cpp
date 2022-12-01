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
//Draw line
//------------------------------------------------------------------------------
void CImages::DrawLine(CDC *pDC, char width, int x0, int y0, 
					             int x, int y, COLORREF color)
{
 //---- Set parameters for Pancil
 CPen Pen; 
 //New pen
 Pen.CreatePen(PS_SOLID, width, color);   

 //Save first pen pointer, select new pancil
 CPen *pOldPen = pDC->SelectObject(&Pen); 
 
 //Jump to start point 
 pDC->MoveTo(x0, y0);

 //Draw curve line
 pDC->LineTo(x, y);
 
 //----- delete all Pen
 //Set old Pen to DC 
 pDC->SelectObject(pOldPen);
 Pen.DeleteObject();
}	 
