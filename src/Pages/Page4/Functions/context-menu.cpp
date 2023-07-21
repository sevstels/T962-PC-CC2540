//==============================================================================
//File name:   ".cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page4.h"

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
//контекстное меню
//------------------------------------------------------------------------------
void CPage4::OnContextMenu(CWnd* pWnd, CPoint point)
{
 if(point.x == -1 && point.y == -1)
  {
      //keystroke invocation
      CRect rect;
      GetClientRect(rect);
      ClientToScreen(rect);

      point = rect.TopLeft();
      point.Offset(15, 15);
    }
    
  CMenu menu;
  
  if(DSPL.object_selected==0 &&
	 DSPL.index_selected.GetSize()==0)
  {
    VERIFY(menu.LoadMenu(IDR_MENU4));
  }  
  
  else if(DSPL.index_selected.GetSize()>0)
  {
    VERIFY(menu.LoadMenu(IDR_MENU3));
  }

  //только для линий и точек
  else if(DSPL.object_selected==1 &&
	(DSPL.object_type==1 || 
	 DSPL.object_type==2 || 
	 DSPL.object_type==3))
  { 
	VERIFY(menu.LoadMenu(IDR_MENU1)); 
  }
  else return;
 
  //DSPL.ShowMenu(&menu);
  CMenu *pPopup = menu.GetSubMenu(0);
  ASSERT(pPopup != NULL);
  CWnd* pWndOwner = pWnd;

  //while(pWndOwner->GetStyle() & WS_CHILD)
  //pWndOwner = pWndOwner->GetParent();

  pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
                         point.x, point.y, pWndOwner);	 
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage4::OnAddVertex(){DSPL.AddVertex();}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage4::OnDeleteVertex(){DSPL.DelVertex();}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnAddSegment(){}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage4::OnDeleteSegment(){DSPL.DelSegment();}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnPage5_SetpProfileDefault()
{ 
  //Standard 
 /* DSPL.Load_Profile(PCR_STANDARD); 
  DSPL.Scroll_Setup();
  DSPL.ev_Redraw.SetEvent();
  m_but_ProfileWrite.EnableWindow(TRUE); */
}
