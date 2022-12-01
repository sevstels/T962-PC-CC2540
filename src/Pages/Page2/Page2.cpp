//==============================================================================
//File name:    "Page2.cpp"
//Purpose:      Source File, CPage2 property page
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page2.h"
#include "MainDlg.h"
#include "float.h"
#include "cmd.h"
#include "dlg_pid-test.h"
#include "datatx.h"
#include "parse-value.h"

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
 
//==============================================================================
//Thread: Monitoring CAN bus, receive CAN data
//==============================================================================
DWORD CAN_BUS_MONITORING3(LPVOID lParam);

IMPLEMENT_DYNCREATE(CPage2, CPropertyPage)

//------------------------------------------------------------------------------
//Constructor
//------------------------------------------------------------------------------
CPage2::CPage2():CPropertyPage(CPage2::IDD)
{
  PID_Period = 0;
  pConsole = NULL; 
  pSample = NULL;
  monitoring = 0;
  PID_log = 0;
  PID_test_log = 0;
  PID_tune_log = 0;
  memset(&tLast, 0, sizeof(LARGE_INTEGER));
  memset(&tPeriod, 0, sizeof(LARGE_INTEGER));
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
CPage2::~CPage2()
{
  if(pSample!=NULL){delete [] pSample; pSample = NULL;} 
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//---- Text controls
	DDX_Control(pDX, IDC_STATIC_CHN, m_txt_chn);

	//---- Edit box control, Float support
	DDX_Control(pDX, IDC_EDIT_KP, m_KP);
	DDX_Control(pDX, IDC_EDIT_KI, m_KI);
	DDX_Control(pDX, IDC_EDIT_KD, m_KD);
	DDX_Control(pDX, IDC_EDIT_FAN_GAIN, m_FAN_Gain);
	DDX_Control(pDX, IDC_EDIT_REF, m_REF);

	DDX_Control(pDX, IDC_EDIT_PID_PERIOD, m_PID_Period);

	//----
	DDX_Control(pDX, IDC_CHECK_LOG_FIX, m_check_LogFix);
	DDX_Control(pDX, IDC_RADIO_PID_MODE_MANUAL, m_radio_tuning);
	DDX_Control(pDX, IDC_RADIO_PID_MODE_AUTO, m_radio_normal);

	//---- Buttons
	DDX_Control(pDX, IDC_BUTTON_SETCHNDEFAULT, m_but_set_default);
	DDX_Control(pDX, IDC_BUTTON_TEST_MEANDER, m_but_PID_test);
	DDX_Control(pDX, IDC_BUTTON_TEST_STEP, m_but_test_step);
	DDX_Control(pDX, IDC_BUTTON_TEST_LINERITY, m_but_test_linerity);
	DDX_Control(pDX, IDC_BUTTON_PID_MANUAL_RUN, m_but_PID_Manual);

	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_AUTO_TUNE, m_but_PID_AutoTune);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPage2, CPropertyPage)
  //{{AFX_MSG_MAP(CPage2)
  ON_WM_HSCROLL()
  //ON_WM_TIMER()									
  //ON_WM_KEYDOWN()
    
  //---- Push Remote Control buttons
///  ON_BN_CLICKED(IDC_BUTTON_SETCHNDEFAULT, OnBnSetDefault)
///  ON_BN_CLICKED(IDC_BUTTON_AUTO_TUNING, OnButtonPidTest)
///  ON_BN_CLICKED(IDC_BUTTON_GET_LINERITY, &CPage2::OnButtonTestLinerity)
  ON_BN_CLICKED(IDC_CHECK_LOG_FIX, &CPage2::OnCheckFixLog)

  //----
  ON_BN_CLICKED(IDC_RADIO_PID_MODE_MANUAL, &CPage2::OnRadioPidModeManual)
  ON_BN_CLICKED(IDC_RADIO_PID_MODE_AUTO, &CPage2::OnRadioPidModeAuto)

  //----
///  ON_BN_CLICKED(IDC_BUTTON_STEP, &CPage2::OnButtonTestStep)
  ON_BN_CLICKED(IDC_BUTTON_PID_MANUAL_RUN, &CPage2::OnButtonPidManualRun)

  ON_BN_CLICKED(IDC_CHECK_LOG_FIX, &CPage2::OnCheckReflowLogFix)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2 message handlers
#include <emmintrin.h>
//------------------------------------------------------------------------------
//Edit Box change handler
//------------------------------------------------------------------------------
BOOL CPage2::PreTranslateMessage(MSG* pMsg)
{
  ToolTip.PreTranslateMessage(pMsg);

  int id, result;
  CString txt, dig;
  signed short sh_value;
  float fl_value;

  //Check message from press ENTER 
  if((WM_KEYDOWN == pMsg->message)&&(VK_RETURN == pMsg->wParam)) 
   {
     unsigned char command=0;
     signed short value;
	 float var=0;
	 double dvar;

     //Get Edit Box data
     UpdateData(TRUE);
    
    //---- P		
    if(pMsg->hwnd==m_KP.m_hWnd) 
	{
		command = CMD_PID_SET_P;
		result = ParseValueFl(&m_KP, fl_value, -20, 20);
		if(!result) return TRUE; 
		//----
		pDevice->PID.P = fl_value;
		pBT->Tx(command, (char*)&fl_value, 4);
		//запрет дальнейшей обработки
		return TRUE;
	}

	//---- I
    if(pMsg->hwnd==m_KI.m_hWnd) 
	{   
		command = CMD_PID_SET_I; 
		result = ParseValueFl(&m_KI, fl_value, -10, 10);
		if(!result) return TRUE; 
		//----
		pDevice->PID.I = fl_value;
		pBT->Tx(command, (char*)&fl_value, 4);
		//запрет дальнейшей обработки
		return TRUE;
	}

	//---- D
    if(pMsg->hwnd==m_KD.m_hWnd) 
	{
		command = CMD_PID_SET_D; 
		result = ParseValueFl(&m_KD, fl_value, 0, 40);
		if(!result) return TRUE; 
		//----
		pDevice->PID.D = fl_value;
		pBT->Tx(command, (char*)&fl_value, 4);
		//запрет дальнейшей обработки
		return TRUE;
	}

   	//---- REF
	if(pMsg->hwnd==m_REF.m_hWnd)
	 {
		command = CMD_PID_SET_REF; 
        result = ParseValueFl(&m_REF, fl_value, 20, 250);
		if(!result) return TRUE;

		//----
		pDevice->PID.REF = fl_value;
		pBT->Tx(command, (char*)&fl_value, 4);
		//запрет дальнейшей обработки
		return TRUE;
	 }
	
	//---- Fan    
    if(pMsg->hwnd==m_FAN_Gain.m_hWnd)
     { 
		command = CMD_SET_FAN_SPEED_GAIN; 
		result = ParseValueFl(&m_FAN_Gain, fl_value, 0.2, 10);
		if(!result) return TRUE; 
		//----
		pDevice->fan_gain = fl_value;
		pBT->Tx(command, (char*)&fl_value, 4);
		//запрет дальнейшей обработки
	   return TRUE; 
     }
  }

  return CPropertyPage::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------
//Click MODE
//------------------------------------------------------------------------------
void CPage2::OnModePidEnable()
{  /*
  pDevice->PID.chn_enable |= (1<<PID_channel);
  
  m_radio_pid_remote_dac.SetCheck(0);
  
  ///CAN_SendCommand(pCAN,CAN_device_ID,0,CMD_PID_SET_MODE,
	              PID_channel, CHN_MODE_PID);
   */
  Controls_Update();
}

//------------------------------------------------------------------------------
//Click MODE
//------------------------------------------------------------------------------
void CPage2::OnModePidDisable()
{
  ////pDevice->PID.chn_mode[PID_channel] = 1;
 
//  m_radio_pid_enable.SetCheck(0);
//  m_radio_pid_remote_dac.SetCheck(0);
  
  Controls_PIDEnable(false);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
BOOL CPage2::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  //Disable controls
  Controls_Enable(FALSE);
    
  //tooltips
  struct prj_tooltip tooltips[] = 
  {  
//    {&m_check_low_noise_mode, "Not in use..."},
//    {&m_check_onoff_I, "ON or OFF I-factor"},
//    {&m_check_onoff_D, "ON or OFF D-factor"},
    {&m_check_LogFix, "Schedule log delay compensation"},
//    {&m_radio_pid_remote_dac, "Stop PID, run Remote access to ADC or DAC"},
    {&m_PID_Period, "PID loop period"},
    {&m_KP, "Set P-factor"},
    {&m_KI, "Set I-factor"},
    {&m_KD, "Set D-factor"}, 
    {&m_REF, "Set temperature °C"},
    {&m_but_set_default, "Reset PID settings to default"},
    {&m_but_PID_test, "PID testing, manual/auto"},
	{&m_but_PID_Manual, "PID manual Run with T REF °C"}
  };

  ToolTip.SetToolTip(FromHandle(this->m_hWnd), tooltips, _countof(tooltips));
  
  //return TRUE unless you set the focus to a control
  return TRUE;  
  //EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------------------
//Click button Reset channel setup
//------------------------------------------------------------------------------
void CPage2::OnBnSetDefault()
{  
  //return pid current setup
///  GetDeviceSetup(CAN_device_ID);
  Controls_Update();
}

//------------------------------------------------------------------------------
//Click button DAC inversion
//------------------------------------------------------------------------------
void CPage2::ShowMode(char number)
{ /*
  m_radio_pid_enable.SetCheck(0);
  m_radio_pid_remote_dac.SetCheck(0);

  switch(number)
  {
    case 0: m_radio_pid_enable.SetCheck(1); break;
    case 2: m_radio_pid_remote_dac.SetCheck(1); break;    
  }														    */
}

#define OVSP_TEMPERATURE     0

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage2::OnButtonPidTest()
{  
  CPidTestDlg PID_TestDlg(GetWindow(IDD_Page2), this);
  
  //Call dialog
  if(PID_TestDlg.DoModal() == IDOK)
  {
     //here all for dialog
  } 
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage2::OnCheckFixLog()
{
  PID_log = m_check_LogFix.GetCheck();
  PID_test_log = PID_log;
}

//------------------------------------------------------------------------------
//Рассчет текущей мгновенной частоты
//------------------------------------------------------------------------------
void CPage2::Calc_Freq(void)
{  
  QueryPerformanceFrequency(&Frequency);
}

//------------------------------------------------------------------------------
//Time delay calculation in uS
//------------------------------------------------------------------------------
unsigned long CPage2::Calc_Delay(LARGE_INTEGER *pTimeStart, LARGE_INTEGER *pTimeEnd)
{  
  double freq = (double)Frequency.QuadPart;
  double time  = (double)(pTimeEnd->QuadPart - pTimeStart->QuadPart);
  time /= (double) Frequency.QuadPart;
  time *= 1000000;

  return (unsigned long)time;
}

//------------------------------------------------------------------------------
//Current Time calculation in uS
//------------------------------------------------------------------------------
unsigned long CPage2::Calc_Time(LARGE_INTEGER *pTime)
{  
  double freq = (double)Frequency.QuadPart;
  double time  = (double)pTime->QuadPart;
  time /= freq;
  time *= 1000;

  return (unsigned long)time;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage2::OnButtonTestLinerity()
{
///  char direction = (char) m_radio_lin_pos.GetCheck();
///  PID_TuneOpenFile();
 
  //Send cmd
  ///CAN_SendCommand(pCAN, CAN_device_ID, CAN_CMD_CHANNEL, 
	///              CMD_TEC_TEST_LINERITY, direction, 0);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage2::OnButtonTestStep()
{
  PID_TuneOpenFile();
  //Send cmd
  ///CAN_SendCommand(pCAN, CAN_device_ID, CAN_CMD_CHANNEL, 
	///              CMD_TEC_TEST_STEP, 0, 0);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage2::OnButtonPidManualRun()
{	/*
  if(pDevice->PID.mode==false)
  {
    //Send cmd
    ///CAN_SendCommand(pCAN, CAN_device_ID, CAN_CMD_CHANNEL, 
	  ///              CMD_PID_MANUAL_ENABLE, 1, 0);

	pDevice->PID.mode = true;
	m_but_PID_Manual.SetWindowText("Stop Manual");
  }
  else
  {
    //Send cmd
    ///CAN_SendCommand(pCAN, CAN_device_ID, CAN_CMD_CHANNEL, 
	///                CMD_PID_MANUAL_ENABLE, 0, 0);

	///pDevice->PID.mode = false;
	m_but_PID_Manual.SetWindowText("Manual Run");
  }	 */
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage2::OnCheckReflowLogFix()
{
	// TODO: Add your control notification handler code here
}
   
void CPage2::OnRadioPidModeManual(){}
  
void CPage2::OnRadioPidModeAuto(){}