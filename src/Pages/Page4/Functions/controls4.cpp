//==============================================================================
//File name:    "controls5.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page4.h"
//#include "cmd.h"

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
  m_but_ShowGraph.EnableWindow(on_off);
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
void CPage4::Controls_Update(int number)
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

/*
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CLPESDlg::BoardControls_Update(void)
{  
  CString txt, msg, id;
  msg.Empty();
  txt = "Control Board ID:\t\t0x"; 
  
  id.Format("%X",CAN_board_ID);
  txt += id;
  txt += "\r\n";
  msg += txt;
  
  //--
  txt = "Control output:\t\t";
  if(Board.OutputMode == 0){txt += "Disabled"; m_check_SensorOutput.SetCheck(0);}
  else {txt += "Enabled"; m_check_SensorOutput.SetCheck(1);}  
  txt +="\r\n";
  msg += txt;
  
  //--
  txt = "Device mode:\t\t";
  if(Board.Mode == 0){txt += "Ready for use";}
  else {txt += "Focus calibr";}  
  txt +="\r\n";
  msg += txt;
  
  //--
  msg += "Attached Sensors:\r\n";
  for(int i=0; i<8; i++)
   {
      char ctxt[20];
      memset(ctxt,0,sizeof(ctxt));
      memcpy(ctxt, &(Board.AttachedName[i][0]), sizeof(ctxt));
      CString strtxt(ctxt);
      
      if(strtxt!="")
      {     
         txt.Format("In %d: %s", i, ctxt, Board.AttachedID[i]);
         msg += txt;
         if(Board.AttachedID[i]!=0){txt.Format(",\tID: 0x%X\r\n", Board.AttachedID[i]);}
         else txt = "\r\n";
         msg += txt;
      } 
   } 

  //--
  msg += "Test pulse Duration:\t\t";
  txt.Format("%d", Board.TestPulseDuration);                                     
  m_edit_duration.SetWindowTextA(txt);
  msg += txt + "uS\r\n";
  
  //--
  msg += "Test pulse Period:\t\t";
  txt.Format("%d", Board.TestPulsePeriod);                                     
  m_edit_period.SetWindowTextA(txt);
  msg += txt + "uS\r\n";
  
  msg += "Pulse Length monitor:\t";
  if(Board.PulseMonitoring==0){txt = "Disabled"; m_check_PulseMonitorng.SetCheck(0);}
  else {txt = "Enabled"; m_check_PulseMonitorng.SetCheck(1);}  
  msg += txt + "\r\n";
  
  //--
  txt.Format("Power supply:\t\t%2.2fv,", Board.Power);
  if(Board.Power < 10) txt += "Err!";
  else txt += " OK";
  txt +="\r\n";
  msg += txt;  
  
  //--
  msg += "Device Firmware:\t";
  txt.Format("ver%d.",(Board.Firmware_version>>4));                                     
  msg += txt;
  txt.Format("%d",(Board.Firmware_version)&0x0F);
  msg += txt;
  txt.Format("build:%d",Board.Firmware_build);
  msg += txt + "\r\n";
  
  m_edit_monitor.SetWindowTextA(msg);
  m_edit_monitor.UpdateWindow();

  if(Sensor.Firmware_version < SN_FIRMWARE_VERSION_LOWER)
   {
        AfxMessageBox(_T("Attention!\nThe Control Board operation under outdated \
      (old) firmware version!\nDevice required Firmware Update!"));
   }
} 
 
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CLPESDlg::SensorControls_Update(void)
{ 
  CString txt, msg, id;
  msg.Empty();
  txt = "Sensor ID:\t0x"; 
  
  id.Format("%X",CAN_sensor_ID);
  txt += id;
  txt += "\r\n";
  msg += txt;
  
  //--
  txt.Format("Integrator pos:\t%d\r\n", Sensor.TAU_value+1);
  msg += txt;
  m_slider_TAU.SetPos(Sensor.TAU_value);

  //--
  txt.Format("Reference pos:\t%d\r\n", Sensor.REF_value);
  msg += txt;
    m_slider_REF.SetPos(Sensor.REF_value);
  
  //--
  txt = "Control output:\t";
  if(Sensor.Output == 0){txt += "Disabled"; m_check_SensorOutput.SetCheck(0);}
  else {txt += "Enabled"; m_check_SensorOutput.SetCheck(1);}  
  txt +="\r\n";
  msg += txt;
  
  //--
  txt = "Device mode:\t";
  if(Sensor.Mode == 0){txt += "Ready for use";}
  else {txt += "Focus calibr";}  
  txt +="\r\n";
  msg += txt;
  
  //--
  txt = "Light Scale:\t";
  if((Sensor.Sound_Scale&0x01) == 0){txt += "Linear";}
  else {txt += "LOG, dB:0-100";}  
  txt +="\r\n";
  msg += txt;
  
  //--
  txt = "Test Sound:\t";
  if((Sensor.Sound_Scale&0x10) == 0x10){txt += "Enabled";}
  else {txt += "Disabled";}  
  txt +="\r\n";
  msg += txt;
              
  //--
  txt.Format("DC/DC Power:\t%2.2fv,", Sensor.Power);
  if(Sensor.Power < 7.4) txt += "Err!";
  else txt += " OK";
  txt +="\r\n";
  msg += txt;

  //--
  char sound, scale;
  sound = (Sensor.Sound_Scale>>4);
  scale = (Sensor.Sound_Scale&0x01);

  if(sound==1) m_check_Sound.SetCheck(1);
  else m_check_Sound.SetCheck(0);
  
  if(scale==1) m_check_Scale.SetCheck(1);
  else m_check_Scale.SetCheck(0);

  //--
  msg += "Photo Diode:\t";
  txt.Format("%s\r\n", Sensor.PhotoDiode);                                     
  msg += txt;

  //--
  msg += "Device Firmware:\t";
  txt.Format("ver %d.",(Sensor.Firmware_version>>4));                                     
  msg += txt;
  txt.Format("%d,",(Sensor.Firmware_version)&0x0F);
  msg += txt;
  txt.Format(" build %d",Sensor.Firmware_build);
  msg += txt + "\r\n";
  
  m_edit_monitor.SetWindowTextA(msg);
  m_edit_monitor.UpdateWindow();

  if(Sensor.Firmware_version < SN_FIRMWARE_VERSION_LOWER)
   {
        AfxMessageBox(_T("Attention!\nThe Sensor Device operation under outdated \
      (old) firmware version!\nDevice required Firmware Update!"));
   }
 
 // m_progress.ShowWindow(false);
 // m_but_Default.EnableWindow(true); 
}                                     */
