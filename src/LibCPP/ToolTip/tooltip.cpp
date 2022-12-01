//==============================================================================
//File name:    "tooltip.cpp"
//Purpose:      Source File
//Version:      2.00
//Copyright:    (c) 2016, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "afxwin.h"
#include "tooltip.h"
#include <stdlib.h>

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
//Constructor
//------------------------------------------------------------------------------
CToolTip::CToolTip()
{					
//  pTTStruct = NULL;
  m_pToolTip = NULL;
  count = 0;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
CToolTip::~CToolTip()
{
  if(m_pToolTip!=NULL) delete m_pToolTip;
  m_pToolTip = NULL;
}

//-----------------------------------------------------------------------------
//handler        
//-----------------------------------------------------------------------------
void CToolTip::PreTranslateMessage(MSG* pMsg)
{
  if(pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST)
   { 
     if(NULL != m_pToolTip) m_pToolTip->RelayEvent(pMsg);
   }
}

//------------------------------------------------------------------------------
//        
//------------------------------------------------------------------------------
void CToolTip::SetToolTip(CWnd *pParentWnd, struct prj_tooltip *pTT, int length)
{
  if(pParentWnd==NULL || length==0 || pTT==NULL) return;
  
  m_pToolTip = NULL;
  pTTStruct = pTT;
  count = length;
  
  //==========================================================
  //Set the tooltip (help hints)
  //==========================================================
  m_pToolTip = new CToolTipCtrl;
  if(!m_pToolTip->Create(pParentWnd))
   {
     TRACE("Unable To create ToolTip\n");
     return;
   }

  CString err, num;
  err = "Unable to add the tooltip: ";
  
  CWnd *pWnd;
  LPCTSTR msg;
  struct prj_tooltip *pTTS;
  
  for(int i=0; i<count; i++)
  {
    pTTS = &pTTStruct[i];
    msg = (LPCTSTR)&pTTS->pTxt[0];
    pWnd = pTTS->pWnd;

    BOOL result = m_pToolTip->AddTool(pWnd, msg, NULL,0);
	if(result==FALSE){num.Format("%d\n", 2); err += num; TRACE(err);}  
  }  
  m_pToolTip->Activate(TRUE);
}

