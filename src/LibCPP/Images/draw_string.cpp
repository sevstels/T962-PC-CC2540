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

//HDC -это идентификатор структуры данных,описывающих устройство вывода графики
//CDC -это контекст графического устройства

//------------------------------------------------------------------------------
//Draw Scale_X
//------------------------------------------------------------------------------
CRect CImages::Draw_StringAt(CDC *pDC, int x, int y, 
							CString txt, int height, COLORREF color)
{ 
 //set font
 CFont font, old_font;
 font.CreateFont(height,0,0,0,FW_BOLD,0,0,0,RUSSIAN_CHARSET,
				OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY,VARIABLE_PITCH | FF_DONTCARE,"Arial");
 
 CFont *pOldFont = pDC->SelectObject(&font);
 CRect Edge;

 //-----------------------------------------
 //Print digit horizontally
 //-----------------------------------------
 pDC->SetTextColor(color);

 //move to point (x,y)
 //pDC->MoveTo(x, y);
	
 //put digit horizontally
 //txt.Format("%d", x);

 pDC->TextOutA(x, y, txt);	

 Edge.top = y;
 Edge.left = x;
 Edge.bottom = y+height;
 Edge.right = x+(height>>1);

 //---- delete graphic objects
 font.DeleteObject();

 pDC->SelectObject(pOldFont);
 
 return Edge;
}
