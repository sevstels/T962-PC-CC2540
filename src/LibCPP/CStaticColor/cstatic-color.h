//==============================================================================
//File name:   "cstatic-color.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _CSTATIC_COLOR_H_
#define _CSTATIC_COLOR_H_

#include "stdafx.h"

//------------------------------------------------------------------------------
//Class
//------------------------------------------------------------------------------
class CStaticColor : public CStatic
{
  public:
  CStaticColor::CStaticColor();

  //make the background transparent (or if 
  //ATransparent == true, restore the previous background color)
  void SetTransparent(BOOL ATransparent = TRUE);
  
  //set background color and make the background opaque
  void SetBackgroundColor(COLORREF);
  void SetTextColor(COLORREF);

  protected: 
  DECLARE_MESSAGE_MAP()
  HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

  private:
  BOOL MTransparent;
  //default is white
  COLORREF MBackgroundColor; 
  //default is black
  COLORREF MTextColor;                                        
};
 
#endif