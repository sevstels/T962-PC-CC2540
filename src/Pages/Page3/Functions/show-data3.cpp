//==============================================================================
//File name:    "show-data3.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page3.h"

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
void CPage3::Show_Temperature(void)
{
  float temperature = pParams->Heater_Temperature;

  //--------------------------------------------------
  //integrator for progress indicator
  //--------------------------------------------------
  short position = (short) temperature;
  INTG_TEMP.AddInputValue(position);
  INTG_TEMP.Integration();
  position = INTG_TEMP.result;

  //set progress indicator
  m_progress_temperature.SetPos(position);
        
  CString txt;
  txt.Format("T: %3.2f°C", temperature);
  m_txt_temperature.SetWindowTextA(txt);
  
  //If panel dlg opened
  if(pPanel_temperature!=NULL)
  {
    pPanel_temperature->SetPos(position); 
  }	 

  //If panel dlg opened	
  if(pPanel_Txt_temperature!=NULL)
  {
	pPanel_Txt_temperature->SetWindowTextA(txt);
  }
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::Show_HeaterPower(void)
{
  CString txt;
  float heater_pwm_power = ((float)pParams->pwm_heater)/256;
	  
  unsigned short heater_power;
  heater_power = (unsigned short)(heater_pwm_power*pDevice->heater_total_power);

  //--------------------------------------------------
  //integrator for progress indicator
  //--------------------------------------------------
  short position = (short)heater_power;
  INTG_HTRPWR.AddInputValue(position);
  INTG_HTRPWR.Integration();
  position = INTG_HTRPWR.result;

  //set progress indicator
  m_progress_heater.SetPos(position);
  txt.Format("P: %d Watt", heater_power);
  m_txt_power.SetWindowTextA(txt);

  //If panel opened
  if(pPanel_heater!=NULL)
  {
	 pPanel_heater->SetPos((int)position);
  }

  //If panel opened
  if(pPanel_Txt_heater!=NULL)
  {
	 txt.Format("Pwr: %d W", heater_power);
	 pPanel_Txt_heater->SetWindowTextA(txt);
  }

  //---------------------------------------------------  
  /*
  COLORREF color = RGB(100,100,100);    
  
  //---- for line color
  int current = (unsigned short)TEC_current; 
  if(current>pDevice->TEC_current_limit)
	 current = pDevice->TEC_current_limit; 
  
  if(current>4000)
  { 
	int delta = current-4000;
	delta = (int)(delta/7);
	int minus = delta>>1;
	color = RGB(100+delta,100-minus,100-minus); 
  }

  m_progress_heater.SetBarColor(color);
  m_progress_heater.SetPos((int)current);
  	*/
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::Show_FanSpeed(void)
{
  unsigned char FAN_speed = pParams->pwm_fan;

  //--------------------------------------------------
  //integrator for progress indicator
  //--------------------------------------------------
  short position = FAN_speed;
  INTG_FANPWM.AddInputValue(position);
  INTG_FANPWM.Integration();
  position = INTG_FANPWM.result;

  //If panel dlg opened
  if(pPanel_fan!=NULL)
  {
	//если значение не изменилось - не рисовать 
	pPanel_fan->SetPos(position); 
	
	if(pPanel_Txt_fan!=NULL)
	{
      CString txt;
      txt.Format("Fan: %d", FAN_speed);  
      pPanel_Txt_fan->SetWindowTextA(txt);
	} 
  }
}
