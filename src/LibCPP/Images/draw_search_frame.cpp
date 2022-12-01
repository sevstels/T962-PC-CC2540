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
void CImages::Draw_SearchFrame(CDC *pCDC, int x, int from, int to)
{
 
 if(x<-199) return;	
	
 CPoint position;
 position.x = x; //graph x-center poin 
 position.y = 127; //graph y-center poin 

 //---- Set parameters for Pancil
 CPen pen_green;
 pen_green.CreatePen(PS_SOLID, 2, RGB(0,200,0));//Green
  
 //Save first pen pointer
 CPen *pOldPen = pCDC->SelectObject(&pen_green);	//select pancil
  
 //--------------------------------------------------------
 //Draw left vertical level line
 //--------------------------------------------------------
 pCDC->MoveTo(from+4, position.y+96);//Start point	 
 pCDC->LineTo(from, position.y+96);	//draw line l
 pCDC->LineTo(from, position.y-96);	//draw line l
 pCDC->LineTo(from+4, position.y-96);//draw line l

 //--------------------------------------------------------
 //Draw right vertical level line
 //--------------------------------------------------------
 pCDC->MoveTo(to-4, position.y+96);//Start point
 pCDC->LineTo(to, position.y+96);	//draw line r
 pCDC->LineTo(to, position.y-96);	//draw line r
 pCDC->LineTo(to-4, position.y-96);//draw line r
 
 //----- delete all Pen
 //¬ыбираем старый Pen в DC (освобождаем свой Pen из DC)
 pCDC->SelectObject(pOldPen);
 pen_green.DeleteObject();
}
