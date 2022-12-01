#include "stdafx.h"
#include "Images.h"
#include <math.h>
//#include "Brent.h"

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
//Paint
//------------------------------------------------------------------------------
void CImages::Draw_Edge(CDC *pCDC, int x0, int y0, int dx, int dy, COLORREF color)
{  
  //Fill BGnd
  pCDC->FillSolidRect(x0, y0, dx, dy+2, color);
  
  //Window edge line верхняя гориз
  DrawLine(pCDC, 1, x0, y0, x0+dx, y0, RGB(0,0,0)); 
  //Window edge line нижняя гориз
  DrawLine(pCDC, 1, x0, y0+dy+2, x0+dx, y0+dy+2, RGB(0,0,0));
  //Window edge line вертикальная левая
  DrawLine(pCDC, 1, x0, y0, x0, y0+dy+2, RGB(0,0,0)); 
  //Window edge line вертикальная правая
  DrawLine(pCDC, 1, x0+dx, y0, x0+dx, y0+dy+2, RGB(0,0,0));
}

//------------------------------------------------------------------------------
//Paint
//------------------------------------------------------------------------------
void CImages::Draw_Edge(CDC *pCDC, CRect rect, COLORREF color)
{  
  //Fill BGnd
  //pCDC->FillSolidRect(rect, color);
													
  //Window edge line верхняя гориз
  //DrawLine(pCDC, 1, rect.left, rect.top, rect.right, rect.top, RGB(0,0,0)); 
  //Window edge line нижняя гориз
/*  DrawLine(pCDC, 1, rect.left, y0+dy, x0+dx, y0+dy, RGB(0,0,0));
  //Window edge line вертикальная левая
  DrawLine(pCDC, 1, rect.left, y0, x0, y0+dy, RGB(0,0,0)); 
  //Window edge line вертикальная правая
  DrawLine(pCDC, 1, rect.left, y0, x0+dx, y0+dy, RGB(0,0,0));   */
}