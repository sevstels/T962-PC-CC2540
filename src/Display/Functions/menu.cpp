//==============================================================================
//File name:    ".cpp"
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
void CDSPL::ShowMenu(CMenu *pMenu)
{ 	/*
  //только для линий и точек
  if(DSPL.object_selected==1 &&
	(DSPL.object_type==1 || 
	 DSPL.object_type==2 || 
	 DSPL.object_type==3))
  {
	VERIFY(menu.LoadMenu(IDR_MENU1));  
	//menu.AppendMenu(MF_STRING, 3443434,(LPCTSTR)"yweriwei");
	//menu.AppendMenuA(MF_STRING,ID_PAGE5_DELETEVERTEX,"dfdfdfd");
	 
  }
  else return;
  
  //menu.AppendMenu(MF_POPUP, (UINT_PTR)menu.m_hMenu, "Menu Name");

  CMenu *pPopup = menu.GetSubMenu(0);
  */
}
