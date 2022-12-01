//==============================================================================
//File name:   "anls-dlg-settings.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "anls-dlg-settings.h"

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

//---- Macro
IMPLEMENT_DYNAMIC(CAnGraphDlg, CDialog)

//------------------------------------------------------------------------------
//---- Constructor
//------------------------------------------------------------------------------
CAnGraphDlg::CAnGraphDlg()
{ 
  pPage4 = NULL;
}

//------------------------------------------------------------------------------ 
//---- Destructor
//------------------------------------------------------------------------------
CAnGraphDlg::~CAnGraphDlg()
{ /*
  pPage4->pPanel_temperature = NULL;
  pPage4->pPanel_current = NULL;
  pPage4->pPanel_Txt_temperature = NULL;
  pPage4->pPanel_Txt_current = NULL; */
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CAnGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonDlg)

	//}}AFX_DATA_MAP 
/*	DDX_Control(pDX, IDC_PROGRESS_TEMPERATURE, m_progress_temperature);
	DDX_Control(pDX, IDC_PROGRESS_CURRENT, m_progress_power);
	DDX_Control(pDX, IDC_STATIC_T, m_static_temperature);
	DDX_Control(pDX, IDC_STATIC_C, m_static_current);  */
}														  

//------------------------------------------------------------------------------ 
//mesages
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAnGraphDlg, CDialog)
  //{{AFX_MSG_MAP(CMonDlg)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_WM_DESTROY()
  ON_WM_WINDOWPOSCHANGED()

END_MESSAGE_MAP()

//------------------------------------------------------------------------------ 
//Dialog initialization
//------------------------------------------------------------------------------
BOOL CAnGraphDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  /*
  //Ini all
  //----
  m_progress_temperature.SetRange(0, 1000); //from to	
  m_progress_temperature.SetPos(0);
  m_progress_temperature.SetStep(1); //Set the 1 step increment
  m_progress_temperature.SetBarColor(RGB(10, 127, 10)); //color
  m_progress_temperature.SetBkColor(RGB(0xff, 0xff, 0xff)); //color
  m_progress_temperature.ShowWindow(TRUE); //Show window 

  //----
  m_progress_power.SetRange(0, 5000); //from to	
  m_progress_power.SetPos(0);
  m_progress_power.SetStep(1); //Set the 1 step increment
  m_progress_power.SetBarColor(RGB(100, 100, 100)); //color
  m_progress_power.SetBkColor(RGB(0xff, 0xff, 0xff)); //color
  m_progress_power.ShowWindow(TRUE); //Show window

  POS.SetWindowPositon(this->m_hWnd);
 */
  return TRUE;
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------  
void CAnGraphDlg::IniPointers(HANDLE hClass)
{ /*
  pPage4 = (CPage4*) hClass;
  pPage4->pPanel_temperature = & m_progress_temperature;
  pPage4->pPanel_current = & m_progress_power;
  pPage4->pPanel_Txt_temperature = &m_static_temperature;
  pPage4->pPanel_Txt_current = &m_static_current; */
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CAnGraphDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{ /*
  CDialog::OnWindowPosChanged(lpwndpos);
  POS.WindowPosChanged(this->m_hWnd, lpwndpos);	*/
}

//------------------------------------------------------------------------------ 
//Press Exit
//------------------------------------------------------------------------------
void CAnGraphDlg::OnBnClickedOk()
{
  OnCancel();
  
  //TODO: Add your control notification handler code here
  //CDialog::OnOK();
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CAnGraphDlg::OnOK()
{
  CDialog::OnOK();
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CAnGraphDlg::OnCancel()
{
  CloseDlg();
}                  

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CAnGraphDlg::CloseDlg(void)
{  
  CDialog::OnCancel();
}
