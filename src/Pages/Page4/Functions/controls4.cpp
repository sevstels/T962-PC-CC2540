//==============================================================================
//File name:    "controls5.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page4.h"

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
void CPage4::Controls_Ini(void)
{
  DSPL.pScroll = &m_Scroll;
	
  //1  Amtech 	NC-31 	        160°C
  //2  ChipQuik SMDLTLFP 	    165°C
  //3  Kester 	NP505-LT 	    190°C 
  //4  Amtech 	4300 63S/37P 	215°C
  //5  Kester 	HM531 	        230°C
  //6  ChipQuik SMD291AX 	    235°C
  //7  Amtech 	SynTECH-LF 	    245°C
  //8  ChipQuik SMD291SNL 	    249°C
  //9  Kester 	R276 	        250°C
  //10 Kester 	NP505-HR 	    250°C
  //11 Kester 	NP545, NP560 	255°C

  m_combo_SelectProfile.ResetContent();
  m_combo_SelectProfile.AddString("Amt NC-31 160°C");
  m_combo_SelectProfile.AddString("ChQ SMDLTLFP 165°C");
  m_combo_SelectProfile.AddString("Kst NP505-LT 190°C");
  m_combo_SelectProfile.AddString("Amt 4300 63S/37P 215°C");
  m_combo_SelectProfile.AddString("Kst HM531 230°C");
  m_combo_SelectProfile.AddString("ChQ SMD291AX 235°C");
  m_combo_SelectProfile.AddString("Amt SynTECH-LF 245°C");
  m_combo_SelectProfile.AddString("ChQ SMD291SNL  249°C");
  m_combo_SelectProfile.AddString("Kst R276 250°C");
  m_combo_SelectProfile.AddString("Kst NP505-HR 250°C");
  m_combo_SelectProfile.AddString("Kst NP545 255°C");
  m_combo_SelectProfile.AddString("Custom Profile");
  m_combo_SelectProfile.AddString("Preheat 50°C 7min");
  m_combo_SelectProfile.AddString("Preheat 75°C 7min");
  m_combo_SelectProfile.AddString("Ramp Speed Test");
  m_combo_SelectProfile.AddString("PID Test"); 
  m_combo_SelectProfile.SetCurSel(-1);

  int check = 0;
  pREG->GetIntVar("page5_log", check);
  m_check_write_log_file.SetCheck(check);
  write_log = check;

  m_check_show_log_console.SetCheck(FALSE);
  m_check_show_log_console.EnableWindow(FALSE);
  EVNT_log = 0;

  m_but_ReflowPause.EnableWindow(FALSE);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::Controls_Enable(int on_off)
{
  //Controls	  
  m_Progress_Write.EnableWindow(on_off);   
  m_Scroll.EnableWindow(on_off);

  //Edit controls
  //Check Box`s
  m_check_show_log_console.EnableWindow(on_off);
  //Buttons
  ///m_but_ShowGraph.EnableWindow(on_off);
  ///m_but_FileShow.EnableWindow(on_off);
  ///m_but_ProfileWrite.EnableWindow(on_off);
  m_but_ProfileRead.EnableWindow(on_off);
  m_but_ReflowRun.EnableWindow(on_off);
  ///m_but_FileSave.EnableWindow(on_off);
  m_but_FromBegin.EnableWindow(on_off);
  m_but_Analysis.EnableWindow(on_off);

  //Combo
  m_combo_SelectProfile.EnableWindow(on_off);
} 

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::ControlsGrop(int on_off)
{ 
  //Controls	  
  m_Progress_Write.EnableWindow(on_off);   
  m_Scroll.EnableWindow(on_off);

  //Buttons
  //m_but_ShowGraph.EnableWindow(on_off);
  m_but_FileShow.EnableWindow(on_off);
  m_but_ReflowRun.EnableWindow(on_off);
  m_but_FileSave.EnableWindow(on_off);
  m_but_ProfileRead.EnableWindow(on_off);
  m_but_ProfileWrite.EnableWindow(on_off);
  m_but_FromBegin.EnableWindow(on_off);
  m_but_Analysis.EnableWindow(on_off);

  //Combo
  m_combo_SelectProfile.EnableWindow(on_off);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::Controls_Update(void)
{ 
 /*
 int value = pDevice->Mode.PCR_InProgress;	
 ButtonSoldering(value);

 if(pDevice->Power>1)
 {
   CString txt;
   txt =  "Device Online";
   m_monitor.SetWindowTextA(txt);
 } */
	
  m_check_show_log_console.SetCheck(pDevice->log_config&0x01);
  m_combo_SelectProfile.SetCurSel(pDevice->reflow_profile);
}
