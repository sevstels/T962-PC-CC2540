//==============================================================================
//File name:    "dialog_pos.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "dialog-pos.h"
#include "AppWinReg.h"
//#include "WinRegDll\DllWinReg.h"

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
//Class	Constructor
//------------------------------------------------------------------------------
CDPOS::CDPOS()
{
  last_x = 0;
  last_y = 0;
  tray = 0;
  start = 0;  
}

//------------------------------------------------------------------------------
//Class	Destructor
//------------------------------------------------------------------------------ 
CDPOS::~CDPOS(){}

//------------------------------------------------------------------------------
//Class	ini this->m_hWnd
//------------------------------------------------------------------------------
int CDPOS::GetScreenRectSize(HWND hWnd, CRect *pRect)
{
  CRect rect;
  HMONITOR hMonitor = MonitorFromWindow(hWnd,MONITOR_DEFAULTTONEAREST);
  MONITORINFO monitorinfo={0};
  monitorinfo.cbSize = sizeof(monitorinfo);
  if(hMonitor!=NULL && GetMonitorInfo(hMonitor,&monitorinfo))
  {
    rect = monitorinfo.rcWork;
  }
  else
  {
    rect.SetRect(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
  }

  *pRect = rect;
  
  return 1;
}

//------------------------------------------------------------------------------
//Class	ini this->m_hWnd
//------------------------------------------------------------------------------
int CDPOS::GetDlgRectSize(HWND hWnd, CRect *pRect)
{
  CRect rect;
  HMONITOR hMonitor = MonitorFromWindow(hWnd,MONITOR_DEFAULTTONEAREST);
  MONITORINFO monitorinfo={0};
  monitorinfo.cbSize = sizeof(monitorinfo);
  if(hMonitor!=NULL && GetMonitorInfo(hMonitor,&monitorinfo))
  {
    rect = monitorinfo.rcWork;
  }
  else
  {
    rect.SetRect(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
  }

  *pRect = rect;
  
  return 1;
}

//------------------------------------------------------------------------------
//Use: this->m_hWnd or GetSafeHwnd()
//------------------------------------------------------------------------------
void CDPOS::SetWindowPositon(HWND hWND)
{ 
  int result;
  CAppWinReg WinReg;
  CRect Display, Dialog;
  
  ShowWindow(hWND, SW_SHOWNORMAL);
  GetClientRect(hWND, &Dialog);
  GetScreenRectSize(hWND, &Display);

  //Get dialog window name
  char txt_name[64];
  memset(txt_name, 0, sizeof(txt_name));
  result = GetWindowTextA(hWND, txt_name, 64);
  
  //char txt_class[10];
  //memset(txt_class, 0, sizeof(txt_class));
  //result = GetClassNameA(hWND, txt_class, 10);
  CString dlg_name;
  if(result>0)
  {
  	dlg_name = txt_name;
	dlg_name += "_"; 
  }
  
  //Set dialog position
  int x, y, res;
  res = WinReg.GetIntVar("Dlg_" + dlg_name + "last_x", x); 
  if(res<=0 || x<0){x = (Display.right>>1) - (Dialog.right>>1);}
  res = WinReg.GetIntVar("Dlg_" + dlg_name + "last_y", y);
  if(res<=0 || y<0){y = (Display.bottom>>1) - (Dialog.bottom>>1);}
  last_x = x;
  last_y = y;
   
  //CWnd *cdw = this->GetWindow(IDD_DIALOG);
  SetWindowPos(hWND, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
  start = 1;              
}

//------------------------------------------------------------------------------
//Save dialog window position in display screen area
//------------------------------------------------------------------------------
void CDPOS::WindowPosChanged(HWND hWND, WINDOWPOS *lpwndpos)
{ 
  //не запоминать если только что стартовали
  if(start==0) return;
  
  HWND hWnd = NULL;
  CRect Dlg;
  memset(&Dlg,0, sizeof(CRect));
  
  if(lpwndpos!=NULL)
  {
	hWnd = lpwndpos->hwnd;
	//не запоминать позицию если окно спрятано в трей
    if(lpwndpos->x<0 || lpwndpos->y<0) {tray=1; return;}
    else {tray = 0;}
  }
  else
  {
  	if(hWND==NULL) return;
	hWnd = hWND;
    GetClientRect(hWnd, &Dlg);
  }

  //Get dialog window name
  char txt_name[64];
  memset(txt_name, 0, sizeof(txt_name));
  int result = ::GetWindowTextA(hWnd, txt_name, 64);
  
  CString dlg_name;
  if(result>0)
  {
  	dlg_name = txt_name;
	dlg_name += "_"; 
  }

  CAppWinReg WinReg;
  if(lpwndpos->x != last_x) 
    {WinReg.SetIntVar("Dlg_" + dlg_name+ "last_x", lpwndpos->x); last_x = lpwndpos->x;} 
  if(lpwndpos->y != last_y) 
    {WinReg.SetIntVar("Dlg_" + dlg_name + "last_y", lpwndpos->y); last_y = lpwndpos->y;}
}
