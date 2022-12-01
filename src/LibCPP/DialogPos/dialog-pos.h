//==============================================================================
//File name:   "dialog_pos.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _DLG_POSITION_H_
#define _DLG_POSITION_H_

//------------------------------------------------------------------------------
//CDPos class
//------------------------------------------------------------------------------
class CDPOS
{
 public:
  //Construction
  CDPOS();
 ~CDPOS();
 
  int last_x;
  int last_y;

  char tray;
  char start;
    
  //Class Functions
  void SetWindowPositon(HWND hWND);
  void WindowPosChanged(HWND hWND, WINDOWPOS *lpwndpos);
  int  GetScreenRectSize(HWND hWnd, CRect *pRect);
  int  GetDlgRectSize(HWND hWnd, CRect *pRect);
};

#endif
