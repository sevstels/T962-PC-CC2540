//==============================================================================
//File name:   "dlg_mon.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "dlg-monitor.h"

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
IMPLEMENT_DYNAMIC(CMonDlg, CDialog)

//------------------------------------------------------------------------------
//---- Constructor
//------------------------------------------------------------------------------
CMonDlg::CMonDlg()
{ 
  pPage3 = NULL;
  pDC = NULL;
}

//------------------------------------------------------------------------------ 
//---- Destructor
//------------------------------------------------------------------------------
CMonDlg::~CMonDlg()
{
  CloseDlg();
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CMonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonDlg)

	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PROGRESS_TEMPERATURE, m_progress_temperature);
	DDX_Control(pDX, IDC_PROGRESS_HEATER, m_progress_heater);
	DDX_Control(pDX, IDC_PROGRESS_FAN, m_progress_fan);
	DDX_Control(pDX, IDC_STATIC_TEMPERATURE, m_static_temperature);
	DDX_Control(pDX, IDC_STATIC_HEATER, m_static_heater);
	DDX_Control(pDX, IDC_STATIC_FAN, m_static_fan);
}														  

//------------------------------------------------------------------------------ 
//mesages
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMonDlg, CDialog)
  //{{AFX_MSG_MAP(CMonDlg)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_WM_DESTROY()
  ON_WM_WINDOWPOSCHANGED()

  ON_WM_PAINT()
END_MESSAGE_MAP()

//------------------------------------------------------------------------------ 
//Dialog initialization
//------------------------------------------------------------------------------
BOOL CMonDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  //Ini all
  //----
  m_progress_temperature.SetRange(0, 300); //from to	
  m_progress_temperature.SetPos(0);
  m_progress_temperature.SetStep(1); //Set the 1 step increment
  m_progress_temperature.SetBarColor(RGB(10, 140, 10)); //color
  m_progress_temperature.SetBkColor(RGB(0xff, 0xff, 0xff)); //color
  m_progress_temperature.ShowWindow(TRUE); //Show window 

  //----
  m_progress_heater.SetRange(0, 1800); //from to	
  m_progress_heater.SetPos(0);
  m_progress_heater.SetStep(1); //Set the 1 step increment
  m_progress_heater.SetBarColor(RGB(140, 10, 10)); //color
  m_progress_heater.SetBkColor(RGB(0xff, 0xff, 0xff)); //color
  m_progress_heater.ShowWindow(TRUE); //Show window
  
  //----
  m_progress_fan.SetRange(0, 255); //from to	
  m_progress_fan.SetPos(0);
  m_progress_fan.SetStep(1); //Set the 1 step increment
  m_progress_fan.SetBarColor(RGB(10, 10, 180)); //color
  m_progress_fan.SetBkColor(RGB(0xff, 0xff, 0xff)); //color
  m_progress_fan.ShowWindow(TRUE); //Show window
  
  POS.SetWindowPositon(this->m_hWnd);

  return TRUE;
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------  
void CMonDlg::IniPointers(HANDLE hClass)
{
  pPage3 = (CPage3*) hClass;
  pPage3->pPanel_temperature = &m_progress_temperature;
  pPage3->pPanel_heater = &m_progress_heater;
  pPage3->pPanel_fan = &m_progress_fan;
  pPage3->pPanel_Txt_temperature = &m_static_temperature;
  pPage3->pPanel_Txt_heater = &m_static_heater;
  pPage3->pPanel_Txt_fan = &m_static_fan;

  int lower, upper;
  pPage3->m_progress_heater.GetRange(lower, upper);
  m_progress_heater.SetRange((short)lower, (short)upper); //from to
  pPage3->m_progress_temperature.GetRange(lower, upper);
  m_progress_temperature.SetRange((short)lower, (short)upper); //from to	
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CMonDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
  CDialog::OnWindowPosChanged(lpwndpos);
  POS.WindowPosChanged(this->m_hWnd, lpwndpos);
}

//------------------------------------------------------------------------------ 
//Press Exit
//------------------------------------------------------------------------------
void CMonDlg::OnBnClickedOk()
{
  OnCancel();
  
  //TODO: Add your control notification handler code here
  //CDialog::OnOK();
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CMonDlg::OnOK()
{
  CDialog::OnOK();
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CMonDlg::OnCancel()
{
  CloseDlg();
  CDialog::OnCancel();
}                  

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CMonDlg::CloseDlg(void)
{  
  pPage3->pPanel_temperature = NULL;
  pPage3->pPanel_heater = NULL;
  pPage3->pPanel_fan = NULL;
  pPage3->pPanel_Txt_temperature = NULL;
  pPage3->pPanel_Txt_heater = NULL;
  pPage3->pPanel_Txt_fan = NULL;
  pPage3->hPanelDlg = NULL;
}
