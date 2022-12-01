//==============================================================================
//File name:    "cstatic-color.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "cstatic-color.h"

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

BEGIN_MESSAGE_MAP(CStaticColor, CStatic)
  ON_WM_CTLCOLOR_REFLECT( )
END_MESSAGE_MAP( )

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
CStaticColor::CStaticColor()
{
  MTransparent = TRUE;
  //default is white (in case someone sets opaque without setting a color)
  MBackgroundColor = RGB(255, 255, 255); 
  //default is black. it would be more clean 
  //to not use the color before set with SetTextColor(..), but whatever...
  MTextColor = RGB(0, 0, 0);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CStaticColor::SetTransparent(BOOL ATransparent)
{
  MTransparent = ATransparent;
  Invalidate();
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CStaticColor::SetBackgroundColor(COLORREF ColorRGB)
{
  MBackgroundColor = ColorRGB;
  MTransparent = FALSE;
  Invalidate();
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CStaticColor::SetTextColor(COLORREF ColorRGB)
{
  MTextColor = ColorRGB;
  Invalidate();
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
HBRUSH CStaticColor::CtlColor(CDC* pDC, UINT nCtlColor)
{
  pDC->SetTextColor(MTextColor);

  //we do not want to draw background when drawing text 
  //background color comes from drawing the control background
  pDC->SetBkMode(TRANSPARENT);  
                                  
  if(MTransparent)
	//return nullptr to indicate that the parent object
	//should supply the brush. it has the appropriate background color
    return nullptr;  
                     
  else
    //color for the empty area of the control
	return (HBRUSH) CreateSolidBrush(MBackgroundColor);  
}
