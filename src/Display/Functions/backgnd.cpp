//==============================================================================
//File name:    "backgnd.cpp"
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
//
//------------------------------------------------------------------------------
void CDSPL::Draw_BackGround(void)
{
  if(opened_bgnd!=1) return;
    
  //Fill background, залить белым
  BGND.FillSolidRect(0,0, display_width, display_height, RGB(255,255,255));
  		
  //Paint Window border, рисуем рамку окна //EDGE_BUMP
  CRect edge;
  edge.left = 0;
  edge.top = 0;
  edge.bottom = display_height;
  edge.right = display_width;
  BGND.DrawEdge(&edge, EDGE_ETCHED, BF_RECT);
}
