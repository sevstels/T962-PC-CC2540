//==============================================================================
//File name:   "dlg_pid-test.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "cmd.h"
#include "dlg_pid-test.h"

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
IMPLEMENT_DYNAMIC(CPidTestDlg, CDialog)

//------------------------------------------------------------------------------
//---- Constructor
//------------------------------------------------------------------------------
CPidTestDlg::CPidTestDlg(CWnd* pParent, HANDLE hPage)
                   : CDialog(CPidTestDlg::IDD, pParent)
{ 
  pPage2 = (CPage2*)hPage;
}

//------------------------------------------------------------------------------ 
//---- Destructor
//------------------------------------------------------------------------------
CPidTestDlg::~CPidTestDlg()
{

}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CPidTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPidTestDlg)

	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_TEST_FUNCTION, m_combo_function);
	DDX_Control(pDX, IDC_EDIT_MAX_VALUE, m_edit_max);
	DDX_Control(pDX, IDC_EDIT_MIN_VALUE, m_edit_min);
	DDX_Control(pDX, IDC_EDIT_CYCLES2, m_edit_period);
	DDX_Control(pDX, IDC_EDIT_CYCLES, m_edit_cycles);
	DDX_Control(pDX, IDC_BUTTON_TEST_ONOFF, m_butt_testrun);
}														  

//------------------------------------------------------------------------------ 
//mesages
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPidTestDlg, CDialog)
  //{{AFX_MSG_MAP(CPidTestDlg)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_WM_DESTROY()

  ON_BN_CLICKED(IDC_BUTTON_TEST_ONOFF, &CPidTestDlg::OnButtonTestOnoff)
  ON_CBN_SELCHANGE(IDC_COMBO_TEST_FUNCTION, &CPidTestDlg::OnComboTestFunction)
END_MESSAGE_MAP()

//------------------------------------------------------------------------------ 
//Dialog initialization
//------------------------------------------------------------------------------
BOOL CPidTestDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  CString txt;

  //Ini all
  m_combo_function.ResetContent();
  m_combo_function.InsertString(0, "Square");
  m_combo_function.InsertString(1, "Triangular");
  m_combo_function.InsertString(2, "Sinusoidal");
  m_combo_function.InsertString(3, "Linear rise");
/*  m_combo_function.SetCurSel(pPage2->pDevice->PID_TEST_function); 
  
  //---- 
  txt.Format("%d", pPage2->pDevice->PID_TEST_max_temperature);
  m_edit_max.SetWindowTextA(txt);
  m_edit_max.UpdateWindow();
  
  //---- 
  txt.Format("%d", pPage2->pDevice->PID_TEST_min_temperature);
  m_edit_min.SetWindowTextA(txt);
  m_edit_min.UpdateWindow();

  //---- 
  txt.Format("%d", pPage2->pDevice->PID_TEST_period_sec);
  m_edit_period.SetWindowTextA(txt);
  m_edit_period.UpdateWindow();
  
  //---- 
  txt.Format("%d", pPage2->pDevice->PID_TEST_cycles);
  m_edit_cycles.SetWindowTextA(txt);
  m_edit_cycles.UpdateWindow();

  //----
  enable = pPage2->pDevice->Mode.PID_TEST_enable;
  if(enable==1) txt = "Test Stop";
  else txt = "Test Run";
  m_butt_testrun.SetWindowTextA(txt);
 */
  return TRUE;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CPidTestDlg::ParseValue(CEdit *pEdit, short min, short max)
{
  CString txt;
  int value;
  int result=0;

  int dlgid = pEdit->GetDlgCtrlID();
  result = GetDlgItemTextA(dlgid, txt);  
  
  //read or not, empty 
  if(result<1)
  {
    txt = "Can`t read it, enter a number...";  
	::AfxMessageBox(txt);
	return -1;
  }

  //read value in any format
  result = sscanf_s(txt, "%d",(int*)&value);
  
  if(result<1)
  {
    txt = "This is not digit!\r\nEnter a number...";  
	::AfxMessageBox(txt);
	return -1;
  }

  if(value<min || value>max)
   {
	  txt.Format("Value must be\r\nbetween: %d... %d", min, max);      
      ::AfxMessageBox(txt);
      return -1;
   }
 
  return (int) value;
}

//------------------------------------------------------------------------------ 
//Press OK
//------------------------------------------------------------------------------
BOOL CPidTestDlg::PreTranslateMessage(MSG* pMsg)
{
  //Check message from press ENTER 
  if((WM_KEYDOWN == pMsg->message)&&(VK_RETURN == pMsg->wParam)) 
  {
   unsigned char cmd = 0, value = 0;

   //---- PID TEST max temperature   
   if(pMsg->hwnd==m_edit_max.m_hWnd)
   { 
     int temperature = ParseValue(&m_edit_max, 20, 100);
	 if(temperature<0) return TRUE; 
	 value = (unsigned char)temperature; 
///	 pPage2->pDevice->PID_TEST_max_temperature = value;
	 cmd = PID_TEST_MAX_TEMPER;
   }   

   //---- PID TEST min temperature   
   if(pMsg->hwnd==m_edit_min.m_hWnd)
   { 
     int temperature = ParseValue(&m_edit_min, 20, 100);
	 if(temperature<0) return TRUE; 
	 value = (unsigned char)temperature; 
///	 pPage2->pDevice->PID_TEST_min_temperature = value;
	 cmd = PID_TEST_MIN_TEMPER;
   }   

   //---- PID TEST Period    
   if(pMsg->hwnd==m_edit_period.m_hWnd)
   { 
     int period = ParseValue(&m_edit_period, 2, 50);
	 if(period<0) return TRUE; 
	 value = (unsigned char)period; 
///	 pPage2->pDevice->PID_TEST_period_sec = value;
	 cmd = PID_TEST_PERIOD;
   }

   //---- PID TEST cycles   
   if(pMsg->hwnd==m_edit_cycles.m_hWnd)
   { 
     int cycles = ParseValue(&m_edit_cycles, 0, 255);
	 if(cycles<0) return TRUE; 
	 value = (unsigned char)cycles; 
///	 pPage2->pDevice->PID_TEST_cycles = value;
	 cmd = PID_TEST_CYCLES;
   }

   //Send to device
   if(cmd>0) 
   {
	 return TRUE;
   }
  }
          
  return CDialog::PreTranslateMessage(pMsg);
}  

//------------------------------------------------------------------------------ 
//Press Exit
//------------------------------------------------------------------------------
void CPidTestDlg::OnBnClickedOk()
{
  OnCancel();
  
  // TODO: Add your control notification handler code here
  //CDialog::OnOK();
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CPidTestDlg::OnOK()
{
  CDialog::OnOK();
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CPidTestDlg::OnCancel()
{
  CloseDlg();
}                  

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CPidTestDlg::CloseDlg(void)
{  
  CDialog::OnCancel();
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CPidTestDlg::OnButtonTestOnoff()
{  /*
  CString txt;
  if(enable==1){enable=0; txt = "Test Run"; 
  pPage2->pDevice->Mode.PID_TEST_enable = false;}
  else{enable=1; txt = "Test Stop";
  pPage2->pDevice->Mode.PID_TEST_enable = true;}
  m_butt_testrun.SetWindowTextA(txt);
 */
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CPidTestDlg::OnComboTestFunction()
{
  unsigned char function, cmd = PID_TEST_FUNCTION;
  function = (unsigned char) m_combo_function.GetCurSel();
}
