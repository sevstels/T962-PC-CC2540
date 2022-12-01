//==============================================================================
//File name:    "controls2.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page2.h"
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
void CPage2::Controls_Ini(void)
{
  m_check_LogFix.SetCheck(FALSE);
  m_radio_tuning.SetCheck(FALSE);
  m_radio_normal.SetCheck(TRUE);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage2::Controls_Enable(BOOL on_off)
{
  //Edit controls
  m_KP.EnableWindow(on_off);
  m_KI.EnableWindow(on_off);
  m_KD.EnableWindow(on_off);
  m_REF.EnableWindow(on_off);

  m_PID_Period.EnableWindow(on_off);
  m_FAN_Gain.EnableWindow(on_off);
  m_check_LogFix.EnableWindow(on_off);
  m_radio_tuning.EnableWindow(on_off);
  m_radio_normal.EnableWindow(on_off);

  //Buttons
  m_but_set_default.EnableWindow(on_off);
  m_but_PID_test.EnableWindow(on_off);   
  m_but_PID_Manual.EnableWindow(on_off);
  m_but_test_step.EnableWindow(on_off);
  m_but_test_linerity.EnableWindow(on_off);
  m_but_PID_AutoTune.EnableWindow(on_off);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage2::Controls_PIDEnable(bool on_off)
{
  //Edit controls
  m_KP.EnableWindow(on_off);
  m_KI.EnableWindow(on_off);
  m_KD.EnableWindow(on_off);
  m_REF.EnableWindow(on_off);
     
  //Check Box`s
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage2::Controls_PIDDisable(void)
{
  //Edit controls
  m_KP.EnableWindow(0);
  m_KI.EnableWindow(0);
  m_KD.EnableWindow(0);
  m_REF.EnableWindow(0);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage2::Controls_PIDWind(bool on_off)
{
  //Edit controls
  m_REF.EnableWindow(on_off);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage2::Controls_Update(void)
{
  CString txt;
  float  buf;
  double dbuf, dstep=0, div=0;
  BOOL on_off;
  
  //---- REF
  txt.Format("%.2f", pDevice->PID.REF);
  m_REF.SetWindowTextA(txt);
  m_REF.UpdateWindow();
  	 
  //---- P
  buf = (float)pDevice->PID.P;
  txt.Format("%.2f", buf);
  m_KP.SetWindowTextA(txt);
  m_KP.UpdateWindow();
  //m_KP.EnableWindow(on_off);

  //---- KI
  buf = (float)pDevice->PID.I;
  txt.Format("%.4f", buf);
  m_KI.SetWindowTextA(txt);
  m_KI.UpdateWindow();
  //m_KI.EnableWindow(on_off);

  //---- D
  buf = (float)pDevice->PID.D;
  txt.Format("%.2f", buf);
  m_KD.SetWindowTextA(txt);
  m_KD.UpdateWindow();
  //m_KD.EnableWindow(on_off);

  //---- Period
  txt.Format("%d", pDevice->PID.period);
  m_PID_Period.SetWindowTextA(txt);
  m_PID_Period.EnableWindow(FALSE);
   
  txt.Format("%.1f", pDevice->fan_gain);
  m_FAN_Gain.SetWindowTextA(txt);

  //PID Log
  m_check_LogFix.SetCheck(TRUE);
  PID_log = m_check_LogFix.GetCheck();
}
