//==============================================================================
//File name:    "pidpc_help.cpp"
//Purpose:      Source File, CPage2 property page
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "T962.h"
#include "MainDlg.h"
#include "user_help.h"
#include <Htmlhelp.h>
#pragma comment(lib,"Htmlhelp.lib")

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

//==================================================================
/* Help Support:
//==================================================================
hlp\ttt.hhp
    This file is a help project file. It contains the data needed to
    compile the help files into a .chm file.

//----------------------------
hlp\ttt.hhc
    This file lists the contents of the help project.

//----------------------------
hlp\ttt.hhk
    This file contains an index of the help topics.

//----------------------------
makehtmlhelp.bat
    This file is used by the build system to compile the help files.

//----------------------------
hlp\Images\*.gif
    These are bitmap files required by the standard help file topics for
    Microsoft Foundation Class Library standard commands.  */
/*
static const DWORD aMenuHelpIDs[] =
      {
       ID_FILE_NEW,   IDH_PAGE_MAIN,
         ID_FILE_OPEN,  IDH_PAGE_ADC,
         ID_FILE_SAVE,  IDH_PAGE_DAC,
         ID_EDIT_CUT,   IDH_EDIT_CUT,
         ID_EDIT_COPY,  IDH_EDIT_COPY,
         ID_EDIT_PASTE, IDH_EDIT_PASTE,
         ID_FILE_PRINT, IDH_FILE_PRINT,
         ID_APP_ABOUT,  IDH_APP_ABOUT, 
         ///ID_FILE_PRINT_PREVIEW, IDH_FILE_PRINT_PREVIEW,
         0, 0
      };   */
 
//Subtract 1 from the total number of ID pairs to account
//for the NULL pair at the end of the array.
//DWORD numHelpIDs = (((sizeof(aMenuHelpIDs)/sizeof(DWORD))/2)-1);
//CMainDlg dlg;
//m_pMainWnd = &dlg;

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CMainDlg::WinHelp(DWORD dwData, UINT nCmd)
{ 
  LPCSTR page;
  HWND hWnd;
  CWnd *W;
  W = GetDesktopWindow();          
  hWnd = W->GetSafeHwnd();
  
  int iTab = m_Tab.GetCurSel()+1;
  switch(iTab)
  {
     case 0: page = "PCRM.chm::/pages/index.htm";break;
     case 1: page = "PCRM.chm::/pages/tab1.htm"; break;
     case 2: page = "PCRM.chm::/pages/tab2.htm"; break;
     case 3: page = "PCRM.chm::/pages/tab3.htm"; break;
     case 4: page = "PCRM.chm::/pages/tab4.htm"; break;
     case 5: page = "PCRM.chm::/pages/tab5.htm"; break;
     default: 
     return;
  }
  
  ::HtmlHelpA(hWnd, page, HH_DISPLAY_TOPIC, NULL);
  
  /*
  switch (nCmd)
   {
    case HELP_CONTEXT:
    //If it is a help context command, search for the
    //control ID in the array.
    for(i= 0; i < numHelpIDs*2; i+=2)
     {
       if(aMenuHelpIDs[i] == LOWORD (dwData))
        {
          i++;  // pass the help context id to HTMLHelp
          ///::HtmlHelpA(NULL,(LPCSTR)"PIDPC.chm", HH_HELP_CONTEXT, aMenuHelpIDs[i]);
          return;
        }
     }

    //If the control ID cannot be found,display the default topic.  (DWORD_PTR)"PIDPC.chm::/pages\adc.htm"
    if(i==numHelpIDs*2)
     { 
	   //HH_DISPLAY_TOPIC  HH_DISPLAY_INDEX  "PIDPC.chm::/pages\adc.htm") ;
       ///::HtmlHelpA(hWnd2,(LPCSTR)"PIDPC.chm", HH_DISPLAY_TOPIC, NULL);
       ///::HtmlHelpA(NULL,(LPCSTR)"PIDPC.chm", HH_HELP_CONTEXT, aMenuHelpIDs[3]);
       ///::HtmlHelpA(hWnd2,(LPCSTR)"PIDPC.chm::/pages/adc.htm", HH_DISPLAY_TOPIC, NULL);
     }                  
    break;    
   }  */
}
    
