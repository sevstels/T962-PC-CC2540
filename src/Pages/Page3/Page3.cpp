//==============================================================================
//File name:    "Page3.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page3.h"
#include "cmd.h"
#include "datatx.h"
#include "parse-value.h"
#include "dlg-monitor.h"

//Using multimedia timer
#include "Mmsystem.h" //MM_Timer

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
//CPage3 property page
//------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CPage3, CPropertyPage)

//------------------------------------------------------------------------------
//CPage3 constructor
//------------------------------------------------------------------------------
CPage3::CPage3() : CPropertyPage(CPage3::IDD)
{
  hTimer = 0;
  MM_TimerID = 0;
  slider_old_pwr = -1;
  slider_old_fan = -1;
  pPanel_temperature = NULL;
  pPanel_heater = NULL;
  pPanel_fan = NULL;
  pPanel_Txt_temperature = NULL;
  pPanel_Txt_heater = NULL;
  pPanel_Txt_fan = NULL;
  hPanelDlg = NULL;
}

//------------------------------------------------------------------------------
//CPage3 destructor
//------------------------------------------------------------------------------
CPage3::~CPage3()
{
  DeletePanel();
}

//------------------------------------------------------------------------------
//CPage3 property page
//------------------------------------------------------------------------------
void CPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_TMPR, m_progress_temperature);
	DDX_Control(pDX, IDC_PROGRESS_CURR, m_progress_heater);
	DDX_Control(pDX, IDC_EDIT_FAN,  m_edit_fan);
	DDX_Control(pDX, IDC_EDIT_HEATER_PWR, m_edit_heater_power);
	DDX_Control(pDX, IDC_EDIT_MAX_POWER, m_edit_power_limit);
	DDX_Control(pDX, IDC_EDIT_MAX_TMPR, m_edit_temperature_limit);

	//----
	DDX_Control(pDX, IDC_CHECK_MANUAL, m_check_manual);
	//----
	DDX_Control(pDX, IDC_SLIDER_FAN, m_slider_fan);
	DDX_Control(pDX, IDC_SLIDER_HEATER, m_slider_heater);
	DDX_Control(pDX, IDC_STATIC_WARNINGS, m_static_warnings);
	DDX_Control(pDX, IDC_STATIC_POWER_VAL, m_txt_power);
	DDX_Control(pDX, IDC_STATIC_TEMPERATURE_VAL, m_txt_temperature);
}

//------------------------------------------------------------------------------
//CPage3 message map
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPage3, CPropertyPage)	
  //----
  ON_BN_CLICKED(IDC_CHECK_MANUAL, &CPage3::OnCheckManual)
  ON_WM_HSCROLL()
  //----
  ON_WM_TIMER()
  ON_BN_CLICKED(IDC_BUTTON_PANEL, &CPage3::OnButtonPanel)

END_MESSAGE_MAP()

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
BOOL CPage3::OnInitDialog()
{
  CPropertyPage::OnInitDialog();
  //Add extra initialization here
  Controls_Ini();
  Controls_Enable(FALSE);

  //ini progress line integrator
  INTG_TEMP.ResetZero(); 
  INTG_HTRPWR.ResetZero();
  INTG_HTRPWM.ResetZero(); 
  INTG_FANPWM.ResetZero(); 
  
  //set integration time 
  INTG_TEMP.IntDivChange(1);
  INTG_HTRPWR.IntDivChange(1);
  INTG_HTRPWM.IntDivChange(1);
  INTG_FANPWM.IntDivChange(1);

  //return TRUE unless you set the focus to a control
  return TRUE;  
  //EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------------------
//Functions:
//------------------------------------------------------------------------------
void CPage3::TabOpen(void)
{
  Controls_Update();
}

//-----------------------------------------------------------------------------
//Multimedia Timer Handler (CALLBACK)
//-----------------------------------------------------------------------------
static void CALLBACK MM_Timer(UINT uID,UINT uMsg,DWORD dwUser,DWORD dw1,DWORD dw2)
{
  CPage3 *pDlg = reinterpret_cast<CPage3*>(dwUser);
  //T-result + Heater pwm value + Fan pwm value
  char cmd = TC_TMPR_HEATER_FAN;
  pDlg->pBT->Tx(CMD_GET_TEMPERATURE, &cmd, 1);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage3::Monitoring(int on)
{
  //----
  if(on==1)
  {
    int period_ms = 250;
    
    //Launch Multimedia timer
    MM_TimerID = timeSetEvent(period_ms, 0, (LPTIMECALLBACK)&MM_Timer, 
                              reinterpret_cast<DWORD_PTR>(this),
                              TIME_PERIODIC | TIME_CALLBACK_FUNCTION);
  }
  else 
  {
    //Stop Multimedia timer
    timeKillEvent(MM_TimerID);
    MM_TimerID = 0;
  }
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::MonitoringParser(char *pBuf)
{
  //CString txt1, txt2; 
  float heater_temperature;
  unsigned char pwm_heater;
  unsigned char pwm_fan;
  
  //parse data
  memcpy(&heater_temperature, pBuf, 4);
  pBuf +=4;
  pwm_heater = *pBuf++;
  pwm_fan = *pBuf;
  
  //save data
  pParams->Heater_Temperature = heater_temperature;
  pParams->pwm_heater = pwm_heater;
  pParams->pwm_fan = pwm_fan;

  //show data
  Show_Temperature();
  Show_HeaterPower();
  Show_FanSpeed();
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::Manual_Enable(int on_off)
{ 
  char cmd[3];

  //Slider controls
  m_slider_fan.EnableWindow(on_off);
  m_slider_heater.EnableWindow(on_off);
  Sleep(20);

  if(on_off!=0)
  {
    //----  
    cmd[0] = 1;
    cmd[1] =(unsigned char) m_slider_heater.GetPos(); 
    cmd[2] =(unsigned char) m_slider_fan.GetPos();
  }
  else
  {
    //----  
    cmd[0] = 0;
	cmd[1] = 0;
	cmd[2] = 0;
  }
    
  //tx
  pBT->Tx(CMD_SET_MANUAL_MODE, cmd, 3);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::OnCheckManual()
{
  int mode = m_check_manual.GetCheck();

  if(mode>0)
  {	 
	Manual_Enable(TRUE); 
  }
  else 
  {
	Manual_Enable(FALSE);
  }
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int CPage3::Calc_HeaterPowerLimit(void)
{
  float temp = pDevice->heater_power_limit;
  temp /= pDevice->heater_total_power;  
  temp *= 256;
  // temp += 1;
  if(temp>255) temp = 255;
  
  //set limit 
  return (unsigned char) temp;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
 //Conversion to CSliderCtrl* pointer
 CSliderCtrl *pSlider = (CSliderCtrl*)pScrollBar;
 
 //Get current hWnd
 HWND hWnd = pSlider->m_hWnd;
 unsigned short position, limit;

 //========================================
 //Slider: change value
 //========================================
 if(hWnd == m_slider_heater.m_hWnd)
 {
	position = (unsigned short)m_slider_heater.GetPos();
	limit = Calc_HeaterPowerLimit();
	if(position<=limit)
	{
	  pBT->Tx(CMD_SET_HEATER, (char*)&position, 2);
	  //pParams->pwm_heater = position;
	  //Show_HeaterPower();
	}
	else m_slider_heater.SetPos(limit);
 }

 //========================================
 //Slider: change value
 //========================================
 if(hWnd == m_slider_fan.m_hWnd)
 {
    position = (unsigned short)m_slider_fan.GetPos();
	pBT->Tx(CMD_SET_FAN, (char*)&position, 2);
 }

  CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
BOOL CPage3::PreTranslateMessage(MSG* pMsg)
{
  unsigned char command; 
  unsigned char ch_value;
  short value;
  int result;
	
  //Check message from press ENTER 
  if((WM_KEYDOWN == pMsg->message)&&(VK_RETURN == pMsg->wParam)) 
  {	
    //Set total power
	if(pMsg->hwnd==m_edit_heater_power.m_hWnd)
	{	
	   command = CMD_SET_HTR_TOTAL_PWR;
	   result = ParseValueSh(&m_edit_heater_power, value, 10, 3000);
	   if(!result) return TRUE; 
		
	   //----
	   pDevice->heater_total_power = value;
	   pBT->Tx(command, (char*)&value, 2);
	   Controls_Update();
		
	   //запрет дальнейшей обработки
       return TRUE;	
     }	  
	 
     //Set Max temperature limit
	if(pMsg->hwnd==m_edit_temperature_limit.m_hWnd)
	{	
	   command = CMD_SET_HTR_TMPRT_LIM;
	   result = ParseValueSh(&m_edit_temperature_limit, value, 10, 400);
	   if(!result) return TRUE; 
		
	   //----
	   pDevice->heater_tempr_limit = value;
	   pBT->Tx(command, (char*)&value, 2);
	   Controls_Update();

	   //запрет дальнейшей обработки
       return TRUE;	
	}	

     //Set Max power
	if(pMsg->hwnd==m_edit_power_limit.m_hWnd)
	{	
	   command = CMD_SET_HTR_POWER_LIM;
	   result = ParseValueSh(&m_edit_power_limit, value, 10, 3000);
	   if(!result) return TRUE; 
		
	   //----
	   pDevice->heater_power_limit = value;
	   pBT->Tx(command, (char*)&value, 2);
	   Controls_Update();

	   //запрет дальнейшей обработки	
       return TRUE;	
	}	    
	 
	//Set FAN SPEED
	if(pMsg->hwnd==m_edit_fan.m_hWnd)
	{	
	   command = CMD_SET_FAN_SPEED;
	   result = ParseValueUCh(&m_edit_fan, ch_value, 0, 255);
	   if(!result) return TRUE; 
		
	   //----
	   pDevice->fan_speed = ch_value;
	   pBT->Tx(command, (char*)&ch_value, 1);
		
	   //запрет дальнейшей обработки
       return TRUE;	
	}	
   }

  return CPropertyPage::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::OnTimer(UINT_PTR nIDEvent)
{ /*
  if(nIDEvent==ID_TIMER_WARNINGS)
  {
  	KillTimer(hTimer);
	hTimer = 0;
	m_static_warnings.SetWindowTextA("");
  }	*/
  CPropertyPage::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::OnButtonPanel()
{
  if(hPanelDlg != NULL) return;
  CMonDlg *pPanelDlg = new CMonDlg();
  pPanelDlg->Create(IDD_MONITOR_TEMPR, this->GetWindow(IDD_Page4));
  pPanelDlg->ShowWindow(SW_SHOW);
  pPanelDlg->IniPointers(this);	
  hPanelDlg	= (HANDLE) pPanelDlg; 
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::DeletePanel(void)
{
  if(hPanelDlg == NULL) return;

  CMonDlg *pPanelDlg = (CMonDlg *) hPanelDlg;
  delete []	pPanelDlg;
  pPanelDlg = NULL;
}