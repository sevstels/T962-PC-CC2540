//==============================================================================
//File name:    "dlg_reflow.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "dlg_reflow.h"

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
IMPLEMENT_DYNAMIC(CReflowSetupDlg, CDialog)

//------------------------------------------------------------------------------
//Constructor
//------------------------------------------------------------------------------
CReflowSetupDlg::CReflowSetupDlg()
{ 
  pDev = NULL;
  status_old = 0x00;
  enabled = 0;
  chn = 0;
  hLED_red = NULL;
  hLED_grey = NULL;
  hLED_green = NULL;
  hLED_yellow =	NULL;
  pCWnd = NULL;
}

//------------------------------------------------------------------------------ 
//Destructor
//------------------------------------------------------------------------------
CReflowSetupDlg::~CReflowSetupDlg()
{
  DeleteObject(hLED_red);
  DeleteObject(hLED_grey);
  DeleteObject(hLED_green);   
  DeleteObject(hLED_yellow);
  hLED_red = NULL;
  hLED_grey = NULL;
  hLED_green = NULL;
  hLED_yellow =	NULL;
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CReflowSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBTSetupDlg)
	/*
	//----
	DDX_Control(pDX, IDC_COMBO_TYPES, m_combo_sns_types);
	DDX_Control(pDX, IDC_COMBO_FILTER, m_combo_filter);
	DDX_Control(pDX, IDC_COMBO_MEASURE_RES, m_combo_measure_res);
	DDX_Control(pDX, IDC_COMBO_AMBIENT_RES, m_combo_ambient_res);
	DDX_Control(pDX, IDC_COMBO_MODE, m_combo_mode);
	DDX_Control(pDX, IDC_COMBO_BURST, m_combo_burst); */
	//----
	DDX_Control(pDX, IDC_EDIT_PAUSE_TIME, m_edit_pause);
	DDX_Control(pDX, IDC_EDIT_PREHIT_TIME, m_edit_prehit_time);
	DDX_Control(pDX, IDC_EDIT_PREHIT_TEMPR, m_edit_prehit_temper);
/*	DDX_Control(pDX, IDC_STATIC_PIC_ALERT1, m_status[0]);
	DDX_Control(pDX, IDC_STATIC_PIC_ALERT2, m_status[1]);
	DDX_Control(pDX, IDC_STATIC_PIC_ALERT3, m_status[2]);
	DDX_Control(pDX, IDC_STATIC_PIC_ALERT4, m_status[3]);
	DDX_Control(pDX, IDC_STATIC_PIC_IR,  m_status[4]);
	DDX_Control(pDX, IDC_STATIC_PIC_THU, m_status[5]);
	DDX_Control(pDX, IDC_STATIC_PIC_BMC, m_status[6]);
	//----
	DDX_Control(pDX, IDC_BUTTON_SET_DEFAULT, m_but_default);
	*/

	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_ALG_MODE, m_combo_align_mode);
}														  

//------------------------------------------------------------------------------ 
//mesages
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CReflowSetupDlg, CDialog)
  //{{AFX_MSG_MAP(CReflowSetupDlg)
  ON_WM_PAINT() 
  ON_WM_DESTROY()
  ON_WM_SYSCOMMAND()
  //ON_WM_CONTEXTMENU()
  ON_WM_QUERYDRAGICON()
  ON_WM_WINDOWPOSCHANGED()
/*  //----
  ON_BN_CLICKED(IDC_BUTTON_SET_DEFAULT, &CReflowSetupDlg::OnButtonSetDefault)
  //----

  //----
 
  //----
  ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CReflowSetupDlg::OnComboMode)
  ON_CBN_SELCHANGE(IDC_COMBO_TYPES, &CReflowSetupDlg::OnComboTypes)
  ON_CBN_SELCHANGE(IDC_COMBO_FILTER, &CReflowSetupDlg::OnComboFilter)
  ON_CBN_SELCHANGE(IDC_COMBO_BURST,  &CReflowSetupDlg::OnComboBurst)
  ON_CBN_SELCHANGE(IDC_COMBO_MEASURE_RES, &CReflowSetupDlg::OnComboMeasureResolution)
  ON_CBN_SELCHANGE(IDC_COMBO_AMBIENT_RES, &CReflowSetupDlg::OnComboAmbientResolution)
 */

END_MESSAGE_MAP()

//------------------------------------------------------------------------------ 
//Dialog initialization
//------------------------------------------------------------------------------
BOOL CReflowSetupDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  CString txt;
  
  //номер канала который обслуживает сейчас диалог
  //txt.Format("Temperature Sensor %d", chn+1);
  //this->SetWindowTextA(txt);
  POS.SetWindowPositon(this->m_hWnd);

  //Preload LED images
  HINSTANCE hInstance;
  //hInstance = AfxGetResourceHandle();
  //hLED_red = LoadBitmapA(hInstance, (LPCSTR)IDB_LED_RED);
  //hLED_grey = LoadBitmapA(hInstance, (LPCSTR)IDB_LED_GREY);
  //hLED_green = LoadBitmapA(hInstance, (LPCSTR)IDB_LED_GREEN);
  //hLED_yellow = LoadBitmapA(hInstance, (LPCSTR)IDB_LED_YELLOW);
  
  Controls_Ini();
  Controls_Enable(FALSE);

/*
  //----
  ///Controls_Enable((BOOL)pDev->TS[chn].connected);
  //add tooltips
  struct prj_tooltip tooltips[] = 
  { 
    {&m_combo_sns_types, "Thermocouple Types"},
    {&m_combo_filter, "Digital filter strength"}, 
    {&m_combo_measure_res, "Temperature Measurement Resolution"},
    {&m_combo_ambient_res, "Ambient Measurement Resolution"},
	{&m_combo_burst, "Number of samples"},	   
    {&m_combo_mode,  "Mode of operation"},  
    {&m_edit_calibr, "Add a voltage shift"},
    {&m_status[0], "Alert 1"},
	{&m_status[1], "Alert 2"},
	{&m_status[2], "Alert 3"},
	{&m_status[3], "Alert 4"},
	{&m_status[4], "Sensor Damage"},
	{&m_status[5], "Temperature conversion complete"},
    {&m_status[6], "Burst mode conversions complete"}
  };

  ToolTip.SetToolTip(FromHandle(this->m_hWnd), tooltips, _countof(tooltips)); */
  return TRUE;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CReflowSetupDlg::Controls_Ini(void)
{
  CString txt;

  //----
  //Ini all
  m_combo_align_mode.ResetContent();
  m_combo_align_mode.InsertString(0, "Differential alignment");
  m_combo_align_mode.InsertString(1, "Adjust to the same value");
  m_combo_align_mode.InsertString(2, " ");
  m_combo_align_mode.InsertString(3, " ");
  m_combo_align_mode.SetCurSel(0);
  /*
  //----
  m_combo_filter.ResetContent();
  m_combo_filter.InsertString(0, "Filter Off");  
  m_combo_filter.InsertString(1, "1 Min, fast"); 
  m_combo_filter.InsertString(2, "2 strength");
  m_combo_filter.InsertString(3, "3 strength");   
  m_combo_filter.InsertString(4, "4 Medium");   
  m_combo_filter.InsertString(5, "5 strength");
  m_combo_filter.InsertString(6, "6 strength");
  m_combo_filter.InsertString(7, "7 Max, slow");
  m_combo_filter.SetCurSel(pDev->TS[chn].filter); 
 
  //----
  m_combo_measure_res.ResetContent();
  m_combo_measure_res.InsertString(0, "18-bit 320ms");  
  m_combo_measure_res.InsertString(1, "16-bit 80ms"); 
  m_combo_measure_res.InsertString(2, "14-bit 20ms");
  m_combo_measure_res.InsertString(3, "12-bit 5ms"); 
  m_combo_measure_res.SetCurSel(pDev->TS[chn].measure_resolution);

  //----
  m_combo_ambient_res.ResetContent();
  m_combo_ambient_res.InsertString(0, "0.0625°C");
  m_combo_ambient_res.InsertString(1, "0.25°C");
  m_combo_ambient_res.SetCurSel(pDev->TS[chn].ambient_resolution);

  //----
  m_combo_burst.ResetContent();
  m_combo_burst.InsertString(0, "1 sample");
  m_combo_burst.InsertString(1, "2 samples");
  m_combo_burst.InsertString(2, "4 samples");
  m_combo_burst.InsertString(3, "8 samples");
  m_combo_burst.InsertString(4, "16 samples");
  m_combo_burst.InsertString(5, "32 samples");
  m_combo_burst.InsertString(6, "64 samples");
  m_combo_burst.InsertString(7, "128 samples");
  m_combo_burst.SetCurSel(pDev->TS[chn].burst_samples);
  m_combo_burst.EnableWindow(FALSE);

  //----
  m_combo_mode.ResetContent();
  m_combo_mode.InsertString(0, "Normal");
  m_combo_mode.InsertString(1, "Shutdown");
  //m_combo_mode.InsertString(2, "Burst");
  m_combo_mode.SetCurSel(pDev->TS[chn].mode);
						  */
  //---- 
  txt.Format("%d", 10);	//  pDev->TS[chn].calibration
  m_edit_pause.SetWindowTextA(txt);
  m_edit_pause.UpdateWindow();
  
  //----
  txt.Format("%d", 3);	
  m_edit_prehit_time.SetWindowTextA(txt);
  txt.Format("%d", 50);	
  m_edit_prehit_temper.SetWindowTextA(txt);

  /*
  if(enabled==0)
  {
	m_combo_sns_types.SetCurSel(-1);
    m_combo_filter.SetCurSel(-1);
	m_combo_measure_res.SetCurSel(-1);
	m_combo_ambient_res.SetCurSel(-1);
	m_combo_burst.SetCurSel(-1);
	m_combo_mode.SetCurSel(-1);
  	m_edit_calibr.SetWindowTextA("");
	Controls_Enable(FALSE);
  }	 */
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CReflowSetupDlg::Controls_Enable(BOOL on_off)
{	/*
  //Combo`s
  m_combo_filter.EnableWindow(on_off); 
  m_combo_sns_types.EnableWindow(on_off);
  m_combo_measure_res.EnableWindow(on_off); 
  m_combo_ambient_res.EnableWindow(on_off); 
  m_combo_mode.EnableWindow(on_off);   */
  m_combo_align_mode.EnableWindow(FALSE);
  
  //Edit controls
  m_edit_pause.EnableWindow(on_off);
  m_edit_prehit_time.EnableWindow(on_off);
  m_edit_prehit_temper.EnableWindow(on_off);

  //Buttons
  //m_but_default.EnableWindow(on_off);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CReflowSetupDlg::Controls_Update(void)
{  
  //Combo`s
 // m_combo_gain.SetCurSel(pDev->gain);
  
  /*
  m_combo_calibration.EnableWindow(on_off);
  //Radio Box`s
  for(int row=0; row<10; row++)
  {
    for(int i=0; i<7; i++)
    {
      m_radio_MUX[row][i].EnableWindow(on_off);
    }
  }
  //Edit controls
  m_edit_intg_time.EnableWindow(on_off);
  m_edit_led_current.EnableWindow(on_off);

  //Buttons
  m_but_save.EnableWindow(on_off);
  m_but_default.EnableWindow(on_off);*/
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CReflowSetupDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
  CDialog::OnWindowPosChanged(lpwndpos);
  POS.WindowPosChanged(this->m_hWnd, lpwndpos);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CReflowSetupDlg::ParseValue(CEdit *pEdit, short min, int max)
{
  CString txt;
  int value;
  int result=0;

  int dlgid = pEdit->GetDlgCtrlID();
  result = GetDlgItemTextA(dlgid, txt);  
  
  //read or not, empty 
  if(result<1)
  {
    txt = "Can`t read it, enter a number";  
    ::AfxMessageBox(txt);
    return -1;
  }

  //read value in any format
  result = sscanf_s(txt, "%d",(int*)&value);
  
  if(result<1)
  {
    txt = "This is not digit!\r\nEnter a number";  
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
//
//------------------------------------------------------------------------------
int CReflowSetupDlg::ParseValueFl(CEdit *pEdit, float min, float max, float &ret)
{
  CString txt;
  float value;
  int result=0;

  int dlgid = pEdit->GetDlgCtrlID();
  result = GetDlgItemTextA(dlgid, txt);  
  
  //read or not, empty 
  if(result<1)
  {
    txt = "Can`t read it, enter a number";  
    ::AfxMessageBox(txt);
    return 0;
  }

  //read value in any format
  result = sscanf_s(txt, "%f",(float*)&value);
  
  if(result<1)
  {
    txt = "This is not digit!\r\nEnter a number";  
    ::AfxMessageBox(txt);
    return 0;
  }

  if(value<min || value>max)
   {
      txt.Format("Value must be\r\nbetween: %2.1f... %2.1f", min, max);      
      ::AfxMessageBox(txt);
      return 0;
   }
  
  ret = value;

  return 1;
}

//------------------------------------------------------------------------------ 
//Press OK
//------------------------------------------------------------------------------
BOOL CReflowSetupDlg::PreTranslateMessage(MSG* pMsg)
{
  //Check message from press ENTER 
  if((WM_KEYDOWN == pMsg->message)&&(VK_RETURN == pMsg->wParam)) 
  {	
 /*  unsigned char cmd = 0; 
   float value = 0;
   int ret;

   //----    
   if(pMsg->hwnd==m_edit_calibr.m_hWnd)
   { 
     ret = ParseValueFl(&m_edit_calibr,-10, 10, value);
     if(ret!=1) return TRUE; 
     
	 pDev->TS[chn].calibration = value;
	 //Send cmd to main dlg
     if(pCWnd!=NULL)pCWnd->PostMessage(UWM_DIALOG_MESSAGES, DEV_SET_CALIBR, chn);
     return TRUE;
   }	*/
  }
  
//  ToolTip.PreTranslateMessage(pMsg);      
  return CDialog::PreTranslateMessage(pMsg);
}  

//------------------------------------------------------------------------------ 
//Press Exit
//------------------------------------------------------------------------------
void CReflowSetupDlg::OnBnClickedOk()
{
  OnCancel();
  //TODO: Add your control notification handler code here
  //CDialog::OnOK();
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CReflowSetupDlg::OnOK()
{
  //CDialog::OnOK();	
  TRACE("OnOK\n");
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CReflowSetupDlg::OnCancel()
{
  CloseDlg();
}                  

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CReflowSetupDlg::CloseDlg(void)
{
  CDialog::OnCancel();

  //Send DLG_CLOSE
  if(pCWnd!=NULL)pCWnd->PostMessageA(UWM_DIALOG_MESSAGES, DEV_DIALG_CLOSE, chn);
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CReflowSetupDlg::OnButtonSetDefault()
{
  //Send cmd to main dlg
  if(pCWnd!=NULL)pCWnd->PostMessage(UWM_DIALOG_MESSAGES, DEV_SET_STP_DEFAULT, chn);
}

//------------------------------------------------------------------------------ 
// https://www.watlow.com/resources-and-support/engineering-tools/reference-data/thermocouple-types
//------------------------------------------------------------------------------
void CReflowSetupDlg::OnComboTypes()
{
  int select = m_combo_sns_types.GetCurSel();
  pDev->TS[chn].sensor_type = (unsigned char)select;
  //Send cmd to main dlg
  if(pCWnd!=NULL)pCWnd->PostMessage(UWM_DIALOG_MESSAGES, DEV_SET_SENSOR_TYPE, chn);
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CReflowSetupDlg::OnComboFilter()
{
  int select = m_combo_filter.GetCurSel();
  pDev->TS[chn].filter = (unsigned char)select;
  //Send cmd to main dlg
  if(pCWnd!=NULL)pCWnd->PostMessage(UWM_DIALOG_MESSAGES, DEV_SET_FILTER, chn);
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CReflowSetupDlg::OnComboMeasureResolution()
{
  int select = m_combo_measure_res.GetCurSel();
  pDev->TS[chn].measure_resolution = (unsigned char)select;
  //Send cmd to main dlg
  if(pCWnd!=NULL)pCWnd->PostMessage(UWM_DIALOG_MESSAGES, DEV_SET_MSR_RES, chn);
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CReflowSetupDlg::OnComboAmbientResolution()
{
  int select = m_combo_ambient_res.GetCurSel();
  pDev->TS[chn].ambient_resolution= (unsigned char)select;
  //Send cmd to main dlg
  if(pCWnd!=NULL)pCWnd->PostMessage(UWM_DIALOG_MESSAGES, DEV_SET_AMB_RES, chn);
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CReflowSetupDlg::OnComboMode()
{
  int select = m_combo_mode.GetCurSel();
  pDev->TS[chn].mode = (unsigned char)select;

  //Send cmd to main dlg
  if(pCWnd!=NULL)pCWnd->PostMessage(UWM_DIALOG_MESSAGES, DEV_SET_MODE, chn);
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CReflowSetupDlg::OnComboBurst()
{
  int select = m_combo_burst.GetCurSel();
  pDev->TS[chn].burst_samples = (unsigned char)select;
  //Send cmd to main dlg
  if(pCWnd!=NULL)pCWnd->PostMessage(UWM_DIALOG_MESSAGES, DEV_SET_BURST, chn);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CReflowSetupDlg::LED_Control(unsigned char status)
{ 
  if(status_old == status) return;
  
  //bit0
  if((status&0x01)!=(status_old&0x01))
  {
	if((status&0x01)==1){m_status[0].SetBitmap(hLED_red);}
	else{m_status[0].SetBitmap(hLED_grey);}
  }
  
  //bit1
  if((status&0x02)!=(status_old&0x02))
  {
	if((status&0x02)==2){m_status[1].SetBitmap(hLED_red);}
	else{m_status[1].SetBitmap(hLED_grey);}
  }
	
  //bit2
  if((status&0x04)!=(status_old&0x04))
  {
	if((status&0x04)==4){m_status[2].SetBitmap(hLED_red);}
	else{m_status[2].SetBitmap(hLED_grey);}
  }

  //bit3
  if((status&0x08)!=(status_old&0x08))
  {
	if((status&0x08)==8){m_status[3].SetBitmap(hLED_red);}
	else{m_status[3].SetBitmap(hLED_grey);}
  }

  //bit4
  //Temperature Range Detection bit (read-only)
  //1 = error
  //0 = ok
  if((status&16)!=(status_old&16))
  {
	if((status&16)==16){m_status[4].SetBitmap(hLED_red);}
	else{m_status[4].SetBitmap(hLED_grey);}
  }

  //bit6
  //TH Update: Temperature Update Flag bit
  //1 = Temperature conversion complete
  if((status&64)!=(status_old&64))
  {
	if((status&64)==64){m_status[5].SetBitmap(hLED_green);}
	else{m_status[5].SetBitmap(hLED_grey);}
  }

  //bit7
  //Burst Complete: Burst Mode Conversions Status Flag bit
  //1 = TΔ register Burst mode conversions complete
  if((status&128)!=(status_old&128))
  {
	if((status&128)==128){m_status[6].SetBitmap(hLED_green);}
	else{m_status[6].SetBitmap(hLED_grey);}
  }

  status_old = status;
}