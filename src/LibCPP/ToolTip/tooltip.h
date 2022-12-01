//==============================================================================
//File name:   "tooltip.h"
//Purpose:      Header File
//Version:      2.00
//Copyright:    (c) 2016, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _PROJECT_TOOLTIP_H_
#define _PROJECT_TOOLTIP_H_

//----------------------------------
// struct
//----------------------------------
typedef struct prj_tooltip 
{  
  //hadle for control
  CWnd *pWnd;

  //tooltip txt
  const char *pTxt;  
  
} tts;

//------------------------------------------------------------------------------
// CToolTip class
//------------------------------------------------------------------------------
class CToolTip
{
  //Construction
  public:
  CToolTip();
 ~CToolTip();
 
  void SetToolTip(CWnd *pParentWnd, struct prj_tooltip *pTT, int length);   
  void PreTranslateMessage(MSG* pMsg);

  protected:
  CToolTipCtrl *m_pToolTip;
  struct prj_tooltip *pTTStruct;
  int count;
};

#endif
