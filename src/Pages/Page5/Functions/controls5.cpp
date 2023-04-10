//==============================================================================
//File name:    "controls5.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page5.h"

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
void CPage5::Controls_Ini(void)
{ 
  for(int i=0; i<4; i++)
  {
    //----
    m_combo_ts[i].ResetContent();
    m_combo_ts[i].InsertString(0, "heat-sensitive"); //
    m_combo_ts[i].InsertString(1, "small 0603"); //
    m_combo_ts[i].InsertString(2, "small chip"); //
    m_combo_ts[i].InsertString(3, "medium chip"); //
    m_combo_ts[i].InsertString(4, "large or metall"); //
	m_combo_ts[i].InsertString(5, "PCB center"); //
    m_combo_ts[i].SetCurSel(0);
  }

   m_combo_htr_calc.ResetContent();
   m_combo_htr_calc.InsertString(0, "use min T-value");
   m_combo_htr_calc.InsertString(1, "calc averaging");
   m_combo_htr_calc.InsertString(2, "use max T-value");
   m_combo_htr_calc.SetCurSel(0);

   m_combo_ts_calc.ResetContent();
   m_combo_ts_calc.InsertString(0, "calc averaging");
   m_combo_ts_calc.InsertString(1, "use max T-value");
   //m_combo_ts_calc.InsertString(2, "keep heat-sensitive");
   m_combo_ts_calc.SetCurSel(0);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage5::Controls_Enable(int on_off)
{
  //----  
  m_combo_ts[0].EnableWindow(on_off);
  m_combo_ts[1].EnableWindow(on_off);
  m_combo_ts[2].EnableWindow(on_off);
  m_combo_ts[3].EnableWindow(on_off);
  
  m_combo_ts_calc.EnableWindow(on_off);
  m_combo_htr_calc.EnableWindow(on_off);
  
  m_check_pcbs[0].EnableWindow(on_off);
  m_check_pcbs[1].EnableWindow(on_off);
  m_check_pcbs[2].EnableWindow(on_off);
  m_check_pcbs[3].EnableWindow(on_off);

  m_check_htrs[0].EnableWindow(on_off);
  m_check_htrs[1].EnableWindow(on_off);
} 

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage5::Controls_Update(void)
{
  int on;
  char check = pDevice->use_sensor_heater;

  //----
  if(check&0x01) on = 1;
  else on = 0;
  m_check_htrs[0].SetCheck(on);	  
  //----
  if(check&0x02) on = 1;
  else on = 0;
  m_check_htrs[1].SetCheck(on);
  
  check = pDevice->use_sensor_pcb;
  //----
  if(check&0x01) on = 1;
  else on = 0;
  m_check_pcbs[0].SetCheck(on);
  //----
  if(check&0x02) on = 1;
  else on = 0;
  m_check_pcbs[1].SetCheck(on);
  //----
  if(check&0x04) on = 1;
  else on = 0;
  m_check_pcbs[2].SetCheck(on);
  //----
  if(check&0x08) on = 1;
  else on = 0;
  m_check_pcbs[3].SetCheck(on);

  m_combo_htr_calc.SetCurSel(pDevice->calc_method_heater);
  m_combo_ts_calc.SetCurSel(pDevice->calc_method_sensors);
}	
