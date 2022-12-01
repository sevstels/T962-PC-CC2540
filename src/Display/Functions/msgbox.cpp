//==============================================================================
//File name:    "msgbox.cpp"
//Purpose:      Source File, CPage2 property page
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "display.h"

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
void CDSPL::MsgBox(const char *pTxt, RECT rect, COLORREF color, int shw_time)
{ 

  //write text message
  //CString txt = "max";
  //pDC->SetTextColor(RGB(0,0,0));
  //pDC->SetBkColor(RGB(255,255,111));//ETO_OPAQUE
  //pDC->ExtTextOutA(position.x+max_x+4,position.y-max_y-40,ETO_CLIPPED,NULL,txt,NULL);

}
