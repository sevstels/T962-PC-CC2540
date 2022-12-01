//==============================================================================
//File name:    "controls6.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page6.h"

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
void CPage6::Controls_Ini(void)
{
  CString txt;	/*
  int value;	   
  pREG->GetIntVar("page8_number", value);
  txt.Format("%d", value);
  m_sample_number.SetWindowTextA(txt);
  
  //----
  pREG->GetIntVar("page8_delay", value);
  txt.Format("%d", value);
  m_sample_delay.SetWindowTextA(txt);
  
  //----	   
  pREG->GetIntVar("page8_period", value);
  txt.Format("%d", value);
  m_sample_period.SetWindowTextA(txt);

  //----	   
  pREG->GetIntVar("page8_on_time", value);  
  txt.Format("%d", value);
  m_led_on_time.SetWindowTextA(txt);
  
  //----
  pREG->GetTxtVar("page8_path", txt);
  m_edit_path.SetWindowTextA(txt);

  //----
  pREG->GetIntVar("page8_led_mux", value);
  LED_Matrix = (unsigned short)value;

  for(int i=0; i<4; i++)
  {
	//распаковываем установки
    char check = (char)(LED_Matrix>>(4*i));
	  
	//----
    m_check_MeasureSample[i][0].SetCheck(check&1);
    m_check_MeasureSample[i][1].SetCheck((check>>1)&1);
    m_check_MeasureSample[i][2].SetCheck((check>>2)&1);
    m_check_MeasureSample[i][3].SetCheck((check>>3)&1);
  }

  m_radio_led_ams.SetCheck(FALSE);
  m_radio_led_can.SetCheck(FALSE);
  m_edit_ledcan_id.SetWindowTextA("");	 */

  //----
  m_progress.SetRange(0, 256); //from to	
  m_progress.SetPos(0);
  m_progress.SetStep(1); //Set the 1 step increment
  m_progress.SetBarColor(RGB(100, 100, 100)); //color
  m_progress.SetBkColor(RGB(0xff, 0xff, 0xff)); //color
  m_progress.ShowWindow(TRUE); //Show window

}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage6::Controls_Enable(int on_off)
{  /*
  //----
  m_but_measure.EnableWindow(on_off);
  m_sample_number.EnableWindow(on_off);
  m_sample_delay.EnableWindow(on_off);
  m_sample_period.EnableWindow(on_off);
  	
  m_radio_led_ams.EnableWindow(on_off);
  m_radio_led_can.EnableWindow(on_off);
  m_edit_ledcan_id.EnableWindow(on_off);

  //Check Box`s	ON
  for(int i=0; i<4; i++)
  {
    //----
    m_check_MeasureSample[i][0].EnableWindow(on_off);
    m_check_MeasureSample[i][1].EnableWindow(on_off);
    m_check_MeasureSample[i][2].EnableWindow(on_off);
    m_check_MeasureSample[i][3].EnableWindow(on_off);
  }*/
} 

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage6::Controls_Update(int number)
{  /*
  //Check Box`s	ON
  for(int i=0; i<4; i++)
  {
	//распаковываем установки
    char check = (char)(LED_Matrix>>(4*i));
	  
	//----
    m_check_MeasureSample[i][0].SetCheck(check&1);
    m_check_MeasureSample[i][1].SetCheck((check>>1)&1);
    m_check_MeasureSample[i][2].SetCheck((check>>2)&1);
    m_check_MeasureSample[i][3].SetCheck((check>>3)&1);
  }

  if(pDevice->Mode.LED_AMS_or_CAN)
  {
	m_radio_led_ams.SetCheck(FALSE);
    m_radio_led_can.SetCheck(TRUE);
	m_edit_ledcan_id.EnableWindow(TRUE);
  }
  else
  {
	m_radio_led_ams.SetCheck(TRUE);
    m_radio_led_can.SetCheck(FALSE);
	m_edit_ledcan_id.EnableWindow(FALSE);
  }

  CString txt;

  //---
  txt.Format("0x%X", pDevice->LED_ControllerID);
  m_edit_ledcan_id.SetWindowTextA(txt);

  //If dialog opened
  if(pSnDlg!=NULL) pSnDlg->Controls_Update(); */
}	
