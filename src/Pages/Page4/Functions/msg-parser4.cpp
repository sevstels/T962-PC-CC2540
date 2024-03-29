//==============================================================================
//File name:    "msg-parser4.cpp"
//Purpose:      Source File, CPage2 page
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page4.h"
#include "cmd.h"
#include "arrays.h"

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

#define PROFILE_HEADER_WRONG    2
#define PROFILE_LENGTH_WRONG    3
#define PROFILE_FILE_DAMAGE     4
#define PROFILE_CANT_START_PID  5

static int counter;

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------  
void CPage4::Parser(char *pBuf)
{
  unsigned char data[32];
  memcpy(data, pBuf, 30);
  CString txt, dig;
  static CString msg;

  //----
  if(data[0]==OVEN_ERROR)
  {
	txt = "Device Error!";
	m_monitor.SetWindowTextA(txt);
	//----

	switch(data[1])
	{	  
	  //----
	  case PROFILE_HEADER_WRONG:
      txt = "Absent temperature profile file\n";
	  txt+= "Write profile file to device";
	  break;	
	  
	  //----
	  case PROFILE_LENGTH_WRONG:
      txt = "Temperature profile to short\n";
	  txt+= "Write correct profile file";
	  break;
	  
	  //----
	  case PROFILE_FILE_DAMAGE:
      txt = "Temperature profile damaged\n";
	  txt+= "Write profile file again";
	  break;
	}
	//----
	if(EVNT_log)pConsole->Msg(LOG_ERR, LOG_TIME,"PCR","%s","Device Error, Stop\n");

    //show err
	AfxMessageBox(_T(txt), MB_ICONSTOP);
	TRACE("PCR Device error\n");
	
	//----
	OnButtonReflowRun();

	return;
  }

  //----
  if(data[0]==PCR_PREPARATION)
  {
    TRACE("PCR PREPARATION\n");
	if(EVNT_log) pConsole->Clear();
	if(EVNT_log) pConsole->Msg(LOG_WARN, LOG_TIME,"PCR", "%s", "Preparation started\n");
	if(PID_log)  PID_LogGraphIni();
	if(PID_log)  PID_LogFileCreate();
	if(PID_log)  PCR_LogFileCreate();
	return;
  }
  
  //----
  if(data[0]==PCR_SAMPLE_BLOCK_END)
  {
	msg ="\n";
	//----
	//LogFile_LivePCR(&ams_data_samples_led);
	if(EVNT_log) pConsole->Msg(LOG_OK, LOG_TIME,"---","%s",msg.GetBuffer());
	return;
  }

  //----
  if(data[0]==EVT_REFLOW_DONE)
  {
	m_monitor.SetWindowTextA("Reflow completed");
	DSPL.Scroll_Handler(5, 0);

	//Stop Reflow process
	ButtonSoldering(0);

	//��� ����� ������ �� �� ������� �� ������ �������
	//��������� ��������� ������
	m_but_ReflowPause.EnableWindow(FALSE);
	m_but_FromBegin.EnableWindow(TRUE);
	m_combo_SelectProfile.EnableWindow(TRUE);
	m_Reflow_enabled = 0;//���������
	m_Reflow_pause = 0;//����� ���
	from_begin = 1;//�������� � ������

	//----
	msg = "Reflow completed\n\n";
	///if(EVNT_log)pConsole->Msg(LOG_WARN, LOG_TIME,"REFLOW","%s",msg.GetBuffer());
	TRACE("REFLOW COMPLETED\n");
	return;
  }

  //----
  if(data[0]==PCR_PAUSE)
  {
	unsigned short timeout;
    memcpy(&timeout, &data[1], 2);
	//----
	if(timeout==0) txt = "Process Paused\n"; 
	else{txt.Format("Pause %d Sec...\n", timeout/1000);}
	
	m_monitor.SetWindowTextA(txt);
	//----
	//msg = "Process interrupted\n\n";
	if(EVNT_log)pConsole->Msg(LOG_ERR, LOG_TIME,"PCR","%s",txt.GetBuffer());
    
	//Stop PCR process
	m_but_FromBegin.EnableWindow(TRUE);
	
	//TRACE("Process interrupted\n");
	TRACE(txt);
	return;
  }
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------  
void CPage4::ParseProfile(char *pBuf, int length)
{
  //Parse array and convert
  PCRF.Ini(&DSPL.profile);
  DSPL.index_cycles.RemoveAll();
  PCRF.pIndex_cycles = &DSPL.index_cycles;
  int result = PCRF.FromArray(pBuf, length);

  if(result==1)
  {
    DSPL.Scroll_Setup();
    DSPL.ev_Redraw.SetEvent();
	//----
	m_but_FileSave.EnableWindow(TRUE);
	m_but_ProfileWrite.EnableWindow(TRUE);
	//----
	m_monitor.SetWindowText(_T("Read Profile: OK"));
	load_profile = 1;
	pProgress->SetBarColor(RGB(0, 128, 0)); //color
	pProgress->SetRange(0,(short)40);//from to
    pProgress->SetPos(39); 

  }
  else{m_monitor.SetWindowText(_T("Read Profile: Error"));}
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::ParseReflowStep(char *pBuf, int length)
{
  ReFlowData DATA;
  memcpy(&DATA, pBuf, length); 
  
  //����� ����� 
  unsigned short point = DATA.min*60+DATA.sec;

  //��������� ���������� ����������� pcb
  float tempr = DATA.sensor_heater[0];

  //----
  CString txt, msg;
  //txt.Format("Point: %3.d", point);
  //----
  msg.Format("Time: %3.ds, %3.1f�C\n", point, tempr);
  m_monitor.SetWindowTextA(msg);
	
  //----
  ///msg.Format("P: %3.d T: %3.2fC\n", point+1, tempr);
  ///if(EVNT_log)pConsole->Msg(LOG_OK, LOG_TIME,"PCR","%s",msg.GetBuffer());
  TRACE2("Time: %d Set: %02.1f\n", point, tempr);
  DSPL.Scroll_Handler(5, point);
	
  //----
  if(PID_log) PID_LogGraph(point, (int)tempr);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPage4::ParseReflowLog(char *pBuf, int length)
{
  ReFlowData TD;
  memcpy(&TD, pBuf, sizeof(TD));

  int struct_length = sizeof(TD);
  if(struct_length!=length) return;
   
  char *pMode = "UNKNOWN";
  if(TD.mode==RFLW_MODE_STANDBY) pMode = "STANDBY";
  if(TD.mode==RFLW_MODE_BAKE) pMode = "BAKE";
  if(TD.mode==RFLW_MODE_REFLOW) pMode = "REFLOW";
  if(TD.mode==RFLW_MODE_BAKE_PREHEAT) pMode = "BAKE-PREHEAT";


  char buf[256];
  memset(buf, 0, sizeof(buf));
  const char *format = "\n%02d:%02d.%02d  %03.1f  %03.1f | %03.1f  %03.1f  %03.1f  %03.1f | %3u  %03.1f  %3u  %3u  %03.1f  %s"; 
  
  _snprintf_s(buf, sizeof(buf), format, 
	        TD.min, TD.sec, TD.msec,
			TD.sensor_heater[0],
			TD.sensor_heater[1],
	        TD.sensor_pcb[0],
  			TD.sensor_pcb[1],
  			TD.sensor_pcb[2],
			TD.sensor_pcb[3],
			TD.set_point,
			TD.temperature_pcb,
			TD.heat, TD.fan,
			TD.cold_junction_temp,
			pMode);

  //TRACE1("%s", t);  
  pConsole->Write("%s", buf);

  //Fill T-Graph buffers
  PointINT p;  
  p.x = (TD.min*60)+TD.sec;
  ///TRACE1("p.x: %d \n", p.x);
  p.y = (int)TD.sensor_heater[0];
  ReflowLog[1].push_back(p);
  
  //----
  p.y = (int)TD.sensor_heater[1];
  ReflowLog[2].push_back(p);
  
  //----
  p.y = (int)TD.sensor_pcb[0];  
  ReflowLog[3].push_back(p);
  
  //----
  p.y = (int)TD.sensor_pcb[1];   
  ReflowLog[4].push_back(p);
  
  //----
  p.y = (int)TD.sensor_pcb[2];   
  ReflowLog[5].push_back(p);
  
  //----
  p.y = (int)TD.sensor_pcb[3];   
  ReflowLog[6].push_back(p);

  //----
  p.y = (int)TD.temperature_pcb;   
  ReflowLog[7].push_back(p);
  
  //----
  p.y = (int)TD.heat;   
  ReflowLog[8].push_back(p);

  //----
  p.y = (int)TD.fan;   
  ReflowLog[9].push_back(p);

  //---- SHOW PCB Temperature
  pPage3->pParams->Heater_Temperature = TD.temperature_pcb;
  pPage3->Show_Temperature();
  //----
  pPage3->pParams->pwm_heater = TD.heat;
  pPage3->Show_HeaterPower();
  //----
  pPage3->pParams->pwm_fan = TD.fan;
  pPage3->Show_FanSpeed();

  //-----
  if(TD.msec==0)
  {
    //����� ����� 
    unsigned short point = TD.min*60+TD.sec;

    //���������� ����������� pcb
    float tempr = TD.temperature_pcb;

    //----
    CString txt, msg;
    //txt.Format("Point: %3.d", point);
    //----
    msg.Format("Time: %3.d Sec, %3.1f�C\n", point, tempr);
    m_monitor.SetWindowTextA(msg);
	
    //----
    ///msg.Format("P: %3.d T: %3.2fC\n", point+1, tempr);
    ///if(EVNT_log)pConsole->Msg(LOG_OK, LOG_TIME,"PCR","%s",msg.GetBuffer());
    ///TRACE2("Time: %d Set: %02.1f\n", point, tempr);
    DSPL.Scroll_Handler(5, point);
	
    //----
    if(PID_log) PID_LogGraph(point, (int)tempr);
  }
}
