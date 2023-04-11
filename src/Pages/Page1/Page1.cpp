//==============================================================================
//File name:    "Page1.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page1.h"
#include "cmd.h"
#include "Images.h"
#include "datatx.h"
#include "parse-value.h"

//---- Using multimedia timer
#include "Mmsystem.h"				//MM_Timer
#pragma comment(lib,"Winmm.lib")	//MM_Timer

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
//CPage1 property page
//------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CPage1, CPropertyPage)

//------------------------------------------------------------------------------
//CPage1 constructor
//------------------------------------------------------------------------------
CPage1::CPage1() : CPropertyPage(CPage1::IDD)
{
  pDevice = NULL;
}

//------------------------------------------------------------------------------
//CPage1 destructor
//------------------------------------------------------------------------------
CPage1::~CPage1()
{

}
/*
Калибровка сенсоров нагревателей

Мы будем использовать максимальную шкалу измерения 400 градусов.
Калибровку можно делать по двум точкам 0 и 100 градусов. 
Установите значения калибровок Offset и DGain равными 0;

Термопары погружаются в кипящую воду. Подстроечным резистором 
регулировки усиления на плате контроллера, установите выход 
сенсоров равным 100 градусов.

Теперь замкните термопару коротким проводом или пинцетом.
Значение на индикаторе должно быть равно величине от дачика температуры CJ.
Если оно не совпадает - введите корректирующую константу в окно Offset.

Разомкните термопару и повторите процесс калибровки несколько раз.
Параметр DGain нужен для точной подстройки усиления без разборки печки.	*/
//------------------------------------------------------------------------------
//CPage1 property page
//------------------------------------------------------------------------------
void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//----
	DDX_Control(pDX, IDC_EDIT_OFFSET1, m_offset[0]);
	DDX_Control(pDX, IDC_EDIT_DGAIN1, m_dgain[0]);
	DDX_Control(pDX, IDC_EDIT_OFFSET2, m_offset[1]);
	DDX_Control(pDX, IDC_EDIT_DGAIN2, m_dgain[1]);
	DDX_Control(pDX, IDC_STATIC_TXTMSG, m_txt);
	DDX_Control(pDX, IDC_STATIC_TOUT1, m_temperature[0]);
	DDX_Control(pDX, IDC_STATIC_TOUT2, m_temperature[1]);
}

//------------------------------------------------------------------------------
//CPage1 message map
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPage1, CPropertyPage)                  
  //----

END_MESSAGE_MAP()


//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
BOOL CPage1::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  // TODO:  Add extra initialization here
  Controls_Ini();
  Controls_Enable(FALSE);

  //tooltips
  struct prj_tooltip tooltips[] = 
  {
	{&m_offset[0], "Offset factor"},
	{&m_offset[1], "Offset factor"},
	{&m_dgain[0], "Gain factor"},
	{&m_dgain[1], "Gain factor"},
	{&m_temperature[0], "Left Top Sensor out"},
	{&m_temperature[1], "Right Top Sensor out"},
  };

  ToolTip.SetToolTip(FromHandle(this->m_hWnd), tooltips, _countof(tooltips));
  //return TRUE unless you set the focus to a control
  return TRUE;  
  //EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage1::Controls_Ini(void)
{
  m_offset[0].SetWindowText("off");
  m_offset[1].SetWindowText("off");
  m_dgain[0].SetWindowText("off");
  m_dgain[1].SetWindowText("off");
  
  //---- 
  m_temperature[0].SetWindowText("off");
  m_temperature[1].SetWindowText("off");

  //----
  CString txt;
  txt = "TC Type-K voltage at 400°C = 16.397mV";
  m_txt.SetWindowText(txt);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage1::Controls_Enable(BOOL on_off)
{
  //Edit controls
  m_offset[0].EnableWindow(on_off);
  m_offset[1].EnableWindow(on_off);
  m_dgain[0].EnableWindow(on_off);
  m_dgain[1].EnableWindow(on_off);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage1::Controls_Update(void)
{
  CString txt;
  
  //----
  txt.Format("%2.1f", pDevice->adc_gain[0]);
  m_dgain[0].SetWindowTextA(txt);
  
  txt.Format("%2.1f", pDevice->adc_gain[1]);
  m_dgain[1].SetWindowTextA(txt); 

  txt.Format("%d", pDevice->adc_offset[0]);
  m_offset[0].SetWindowTextA(txt);

  txt.Format("%d", pDevice->adc_offset[1]);
  m_offset[1].SetWindowTextA(txt);
}						

//------------------------------------------------------------------------------
//WM Hahdler
//------------------------------------------------------------------------------
BOOL CPage1::PreTranslateMessage(MSG* pMsg)
{
  //Check message from press ENTER 
  if((WM_KEYDOWN == pMsg->message)&&(VK_RETURN == pMsg->wParam)) 
  {	
    unsigned char command=0;
    char value;
	float fl_value;
	int result;

	//----		
    if(pMsg->hwnd==m_dgain[0].m_hWnd) 
	{
		command = CMD_SET_DGAIN0;
		result = ParseValueFl(&m_dgain[0], fl_value, 0.1, 2);
		if(!result) return TRUE; 
		//----
		pDevice->adc_gain[0] = fl_value;
		pBT->Tx(command, (char*)&fl_value, 4);
		//запрет дальнейшей обработки
		return TRUE; 
	} 

	//----		
    if(pMsg->hwnd==m_dgain[1].m_hWnd) 
	{
		command = CMD_SET_DGAIN1;
		result = ParseValueFl(&m_dgain[0], fl_value, 0.1, 2);
		if(!result) return TRUE; 
		//----
		pDevice->adc_gain[1] = fl_value;
		pBT->Tx(command, (char*)&fl_value, 4);
		//запрет дальнейшей обработки
		return TRUE; 
	}

	//----		
    if(pMsg->hwnd==m_offset[0].m_hWnd) 
	{
		command = CMD_SET_OFFSET0;
		result = ParseValueCh(&m_offset[0], value, -25, 25);
		if(!result) return TRUE; 
		//----
		pDevice->adc_offset[0] = value;
		pBT->Tx(command, (char*)&value, 1);
		//запрет дальнейшей обработки
		return TRUE; 
	}

	//----		
    if(pMsg->hwnd==m_offset[1].m_hWnd) 
	{
		command = CMD_SET_OFFSET1;
		result = ParseValueCh(&m_offset[1], value, -25, 25);
		if(!result) return TRUE; 
		//----
		pDevice->adc_offset[1] = value;
		pBT->Tx(command, (char*)&value, 1);
		//запрет дальнейшей обработки
		return TRUE; 
	}

  }

  ToolTip.PreTranslateMessage(pMsg);
  return CPropertyPage::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
//Multimedia Timer Handler (CALLBACK)
//-----------------------------------------------------------------------------
static void CALLBACK MM_Timer(UINT uID,UINT uMsg,DWORD dwUser,DWORD dw1,DWORD dw2)
{
  CPage1 *pDlg = reinterpret_cast<CPage1*>(dwUser);
  char cmd = TC_HEATER_ARRAY;
  pDlg->pBT->Tx(CMD_GET_TEMPERATURE, &cmd, 1);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage1::Monitoring(int on)
{ 

  //----
  if(on==1)
  {
    int period_ms = 500;
    
    //Launch Multimedia timer
    MM_TimerID = timeSetEvent(period_ms, 0, (LPTIMECALLBACK)&MM_Timer, 
                              reinterpret_cast<DWORD_PTR>(this),
                              TIME_PERIODIC | TIME_CALLBACK_FUNCTION);
    
   // m_but_scan.SetWindowTextA("Stop Read");
  }
  else 
  {
    //Stop Multimedia timer
    timeKillEvent(MM_TimerID);
    MM_TimerID = 0;
    // m_but_scan.SetWindowTextA("Loop Read");
    
    //Disable remote device 
    //Sleep(20);
  }
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage1::HeaterTemperature(char *pBuf)
{
  CString txt1, txt2; 
  float heater_temperature[2];
  memcpy(heater_temperature, pBuf, 8);

  //----
  txt1.Format(" %3.1f °C", heater_temperature[0]);
  txt2.Format(" %3.1f °C", heater_temperature[1]);

  //Print value
  m_temperature[0].SetWindowText(txt1);
  m_temperature[1].SetWindowText(txt2);
}
