//==============================================================================
//File name:    "show-data3.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2021, Akimov Vladimir  E-mail: decoder@rambler.ru	
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
  static int progress_temperature_old = 0;
  static short HeaterTemperature_old = 0x0FFF;

  //--------------------------------------------------
  //integrator for progress indicator
  //--------------------------------------------------
  short position = (short)pParams->Heater_Temperature;
  INTG_TEMP.AddInputValue(position);
  INTG_TEMP.Integration();
  position = INTG_TEMP.result;

  //set progress indicator
  //если значение не изменилось - не перерисовывать
  if(progress_temperature_old!=position)
  {
	m_progress_temperature.SetPos(position);
  }
  progress_temperature_old = position;
  //If panel opened
  if(pPanel_temperature!=NULL) 
	 pPanel_temperature->SetPos(position); 
  //---------------------------------------------------

  //если значение не изменилось - не считать
  if(HeaterTemperature_old==position) return;
  HeaterTemperature_old = position;

  CString txt;
  txt.Format("%3.2f °C", pParams->Heater_Temperature);
  m_txt_temperature.SetWindowTextA(txt);
  
  //if panel dlg opened
  if(pPanel_Txt_temperature!=NULL)
	 pPanel_Txt_temperature->SetWindowTextA(txt);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::Show_Power(void)
{
  static unsigned short Heater_power_old = 4000;
  static int progress_power_old = 0;
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
  //если значение не изменилось - не перерисовывать
  if(progress_power_old!=position)
  {
	m_progress_power.SetPos(position);
  }
  progress_power_old = position;
  //If panel opened
  if(pPanel_current!=NULL)
	 pPanel_current->SetPos((int)position);
  //---------------------------------------------------

  //если значение не изменилось - не считать
  if(Heater_power_old==heater_power) return;
  Heater_power_old = heater_power;

  CString txt;
  txt.Format("%d Watt", heater_power);

  m_txt_power.SetWindowTextA(txt);
  
  //If panel opened
  if(pPanel_Txt_current!=NULL)
	 pPanel_Txt_current->SetWindowTextA(txt);
  
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

  m_progress_power.SetBarColor(color);
  m_progress_power.SetPos((int)current);
  	*/
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage3::Show_Voltage(void)
{
  static float TEC_voltage_old = -1000;
  static int progress_voltage_old = 0;
  float TEC_voltage = pParams->TEC_voltage;

  //--------------------------------------------------
  //integrator for progress indicator
  //--------------------------------------------------
  short position = (short)(TEC_voltage*10);
  INTG_VOLT.AddInputValue(position);
  INTG_VOLT.Integration();
  position = INTG_VOLT.result;

  //set progress indicator
  //если значение не изменилось - не перерисовывать
/*  if(progress_voltage_old!=position)
    {m_progress_voltage.SetPos(position);}
  progress_voltage_old = position; */
  //---------------------------------------------------

  //если значение не изменилось - не считать
  if(TEC_voltage_old==TEC_voltage) return;
  TEC_voltage_old = TEC_voltage;

  CString txt;
/*  txt.Format("%2.2f V", TEC_voltage);
  m_txt_voltage.SetWindowTextA(txt); */

  //calc power
  float power;
  power = TEC_voltage;
  power *= pParams->TEC_current;
  power /= 1000; 
  //
  txt.Format("%2.2f Watt", power);
  m_txt_power.SetWindowTextA(txt);
}
