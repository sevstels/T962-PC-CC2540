//==============================================================================
//File name:    "controls3.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page3.h"
#include "cmd.h"

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
//Function:
//------------------------------------------------------------------------------
void CPage3::Controls_Ini(void)
{ 
  //----
  m_edit_fan.SetWindowTextA("off");
  m_edit_power_limit.SetWindowTextA("off");
  m_edit_heater_power.SetWindowTextA("off");
  m_edit_temperature_limit.SetWindowTextA("off");
  m_check_manual.SetCheck(FALSE);
  //m_static_warnings.SetWindowTextA("");
  
  //---- Sliders
  m_slider_heater.SetRange(0, 255, TRUE);  
  m_slider_heater.SetPageSize(4);
  m_slider_heater.SetLineSize(1); //step +/-
  m_slider_heater.SetPos(0);
  //----
  m_slider_fan.SetRange(0, 255, TRUE);  
  m_slider_fan.SetPageSize(4);
  m_slider_fan.SetLineSize(1); //step +/-
  m_slider_fan.SetPos(0);

  //----
  //from to -14=0 to... 120=120+14
  m_progress_temperature.SetRange(0, 400); //from to	
  m_progress_temperature.SetPos(0);
  m_progress_temperature.SetStep(1); //Set the 1 step increment
  m_progress_temperature.SetBarColor(RGB(10, 127, 10)); //color
  m_progress_temperature.SetBkColor(RGB(0xff, 0xff, 0xff)); //color
  m_progress_temperature.ShowWindow(TRUE); //Show window 
  
  //----
  m_progress_power.SetRange(0, 2000); //from to	
  m_progress_power.SetPos(0);
  m_progress_power.SetStep(1); //Set the 1 step increment
  m_progress_power.SetBarColor(RGB(100, 100,100)); //color
  m_progress_power.SetBkColor(RGB(0xff, 0xff, 0xff)); //color
  m_progress_power.ShowWindow(TRUE); //Show window
  
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::Controls_Enable(int on_off)
{ 
  //Slider controls
  m_slider_fan.EnableWindow(on_off);
  m_slider_heater.EnableWindow(on_off);
  m_check_manual.EnableWindow(on_off);

  //Edit controls
  m_edit_fan.EnableWindow(on_off);
  m_edit_heater_power.EnableWindow(on_off);
  m_edit_power_limit.EnableWindow(on_off);
  m_edit_temperature_limit.EnableWindow(on_off);

  //Progress indicator
  m_progress_temperature.EnableWindow(on_off);
  m_progress_power.EnableWindow(on_off);
  
  //Static
  //m_static_info.EnableWindow(on_off);

  //Radio Butt
  //m_check_livemode.EnableWindow(on_off);
  //m_check_manual.EnableWindow(on_off);

  //reset indicators
  if(on_off==0)
  {
   	m_progress_temperature.SetPos(0);
	m_progress_power.SetPos(0);
  
	//If panel opened
    if(pPanel_temperature!=NULL) 
	   pPanel_temperature->SetPos(0);   
    //If panel opened
    if(pPanel_current!=NULL)
	   pPanel_current->SetPos(0);  
  }
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::Controls_Update(char val_number)
{
  CString txt; 
 			
  //---
  txt.Format("%d", pDevice->fan_speed);
  m_edit_fan.SetWindowTextA(txt);
  
  //---
  txt.Format("%d", pDevice->heater_total_power);
  m_edit_heater_power.SetWindowTextA(txt);

  //---
  txt.Format("%d", pDevice->heater_tempr_limit);
  m_edit_temperature_limit.SetWindowTextA(txt);

  //---
  txt.Format("%d", pDevice->heater_power_limit);
  m_edit_power_limit.SetWindowTextA(txt);
  
  //---
  m_progress_temperature.SetRange(0, pDevice->heater_tempr_limit+10); //from to
  m_progress_power.SetRange(0, pDevice->heater_power_limit+10); //from to	
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int CPage3::CheckTemperatureSensor(int adc_code)
{ /*
 //shorted 
 if(adc_code<20)
  {
	if(hTimer!=0) return -1;

	CString txt;
	txt = "Error detected!\nTemperature sensor wire shorted!";
	m_static_warnings.SetWindowTextA(txt);
	//m_static_warnings.SetBkColor(RGB(10,0,0));
    m_static_warnings.SetBlinkTextColors(RGB(255,0,0), RGB(160,0,0));
	m_static_warnings.StartTextBlink(TRUE, CColorStaticST::ST_FLS_FAST); 
	
	//warng_timer
	hTimer = SetTimer(ID_TIMER_WARNINGS, 10000, 0); 

  	return -1;
  }

 //disconnected
 else if(adc_code>1000)
  {
	if(hTimer!=0) return -1;
    CString txt;
	txt = "Error detected!\nTemperature sensor wire came off!";
	m_static_warnings.SetWindowTextA(txt);
	m_static_warnings.SetBlinkTextColors(RGB(255,0,0), RGB(160,0,0));
	m_static_warnings.StartTextBlink(TRUE, CColorStaticST::ST_FLS_FAST);

	//warng_timer
	hTimer = SetTimer(ID_TIMER_WARNINGS, 10000, 0); 
	return -1;
  }
 */
 return 1;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::ShowWarningMsg(CString *pTxt)
{  /*
  m_static_warnings.SetWindowTextA(*pTxt);
  //m_static_warnings.SetBkColor(RGB(10,0,0));
  m_static_warnings.SetBlinkTextColors(RGB(255,0,0), RGB(160,0,0));
  m_static_warnings.StartTextBlink(TRUE, CColorStaticST::ST_FLS_FAST); 
	
  //warng_timer
  hTimer = SetTimer(ID_TIMER_WARNINGS, 20000, 0); */
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::ShowOkMsg(CString *pTxt)
{ /*
  if(hTimer!=0)
  {
  	KillTimer(hTimer);
	hTimer = 0;
	m_static_warnings.SetWindowTextA("");
	Sleep(100);
  }
	
  m_static_warnings.SetWindowTextA(*pTxt);
  //m_static_warnings.SetBkColor(RGB(10,0,0));
  m_static_warnings.SetBlinkTextColors(RGB(0,0,0), RGB(0,255,0));
  m_static_warnings.StartTextBlink(TRUE, CColorStaticST::ST_FLS_FAST); 
	
  //warng_timer
  hTimer = SetTimer(ID_TIMER_WARNINGS, 10000, 0); */
}
