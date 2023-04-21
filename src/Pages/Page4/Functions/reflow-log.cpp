//==============================================================================
//File name:    "reflow-log.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page4.h"
#include "cmd.h"
#include "string.h"

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

//-----------------------------------------------------------------------------
//Data convertor to text
//-----------------------------------------------------------------------------  
void CPage4::LogFile_Reflow(HANDLE pSamples)
{  
  ReFlowData TD;
  memcpy(&TD, pSamples, sizeof(TD));

  int struct_length = sizeof(TD);
  //if(struct_length!=length) return;

  //Get time
  LOG_RFLW.TimeStamp();
  
  CString txt, txt_led;

  //--------------------------------------
  //Check file on disk
  //--------------------------------------
  int ret = LOG_RFLW.CheckFile();
  if(ret==FALSE)
  {	
    //Create header
	txt.Format("%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\r\n", 
	           "Time", "Heater L", "Heater R", 
			   "TS-1", "TS-2", "TS-3", "TS-4", 
			   "T-Set", "T-Actual", "PWR-Heat" ,"PWR-Fan",
			   "T-ColdJ", "Mode");
	//Create new file
	LOG_RFLW.NewFile();
	
	//Write data
    LOG_RFLW.Write(txt.GetBuffer());
  }

  char *pMode = "UNKNOWN";
  if(TD.mode==RFLW_MODE_STANDBY) pMode = "STANDBY";
  if(TD.mode==RFLW_MODE_BAKE) pMode = "BAKE";
  if(TD.mode==RFLW_MODE_REFLOW) pMode = "REFLOW";
  if(TD.mode==RFLW_MODE_BAKE_PREHEAT) pMode = "BAKE-PREHEAT";

  //---------------------------------------
  //Samples Data
  //--------------------------------------- 
  const char *format = "%02d:%02d.%02d;%03.1f;%03.1f;%03.1f;%03.1f;%03.1f;%03.1f;%3u;%03.1f;%3u;%3u;%03.1f;%s\n"; 
  txt.Format(format,
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

  //Replace . to , for Excel
  txt.Replace('.', ',');

  //Write data
  LOG_RFLW.Write(txt.GetBuffer());
}
