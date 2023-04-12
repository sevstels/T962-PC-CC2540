//==============================================================================
//File name:    "edits.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page2.h"
#include "cmd.h"

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
//Function:
//------------------------------------------------------------------------------    
int CPage2::EditBoxs(MSG *pMsg)
{
  /* //Check message from press ENTER 
 if((WM_KEYDOWN != pMsg->message)||(VK_RETURN != pMsg->wParam)) return 0; 

 int dlgid; BOOL result;
 unsigned char cmd;
 unsigned short value;
 CString txt;
 
 //---------------------------------------------------------
 if(pMsg->hwnd==m_edit_period.m_hWnd)
   { 
     cmd = CMD_SET_PTEST_PERIOD; 
     dlgid = m_edit_period.GetDlgCtrlID();
     value = (unsigned short)GetDlgItemInt(dlgid, &result, 0);
     
     if(value<1||value>600)
      {
        ::AfxMessageBox("Value must be: 1<->600"); 
        txt.Format("%d", Board.TestPulsePeriod);  
        m_edit_period.SetWindowTextA(txt);
        return 1;
      }
     Board.TestPulsePeriod = value;
     //Send 2 Byte    

     Beep(1000, 5);  
     return 1;
   }

  //---------------------------------------------------------  
  if(pMsg->hwnd==m_edit_duration.m_hWnd)
   { 
     cmd = CMD_SET_PTEST_DURATION; 
     dlgid = m_edit_duration.GetDlgCtrlID();
     value = (unsigned short)GetDlgItemInt(dlgid, &result, 0);
     txt.Format("%d", Board.TestPulseDuration);
     
     if(Board.TestPulsePeriod<=(value+10))
      {
        ::AfxMessageBox("Pulse Duration must be less than Pulse Period + 10 uS."); 
        m_edit_duration.SetWindowTextA(txt);
        return 1;
      } 
     else if(value<1||value>599)
      {
        ::AfxMessageBox("Value must be: 1<->599");
        m_edit_duration.SetWindowTextA(txt);
        return 1;
      } 
 
     Board.TestPulseDuration = value;
     
     //Send 2 Byte

     Beep(1000, 5);
     return 1;
   }
     */
   return 0;
}
