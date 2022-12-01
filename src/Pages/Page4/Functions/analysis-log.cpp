//==============================================================================
//File name:    "analysis-log.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2021, Akimov Vladimir  E-mail: decoder@rambler.ru	
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
void CPage4::ParseLED(unsigned char leds, CString *pTxt)
{
  //----
  char R,G,B,Y;
  
  CString txt;
  
  //----
  R =  leds&0x01;
  G = (leds>>1)&0x01;
  B = (leds>>2)&0x01;
  Y = (leds>>3)&0x01;
  
  //----	        
  if(R==1) txt += "R";
  if(G==1) txt += "G";
  if(B==1) txt += "B";
  if(Y==1) txt += "Y";

  //if all disabled 
  if(R==0 && G==0 && B==0 && Y==0)txt += "OFF";
    
 *pTxt += txt;
}

//-----------------------------------------------------------------------------
//Data convertor to text
//-----------------------------------------------------------------------------
void CPage4::LogFile_DevicePCR(HANDLE pArray)
{ 
  CArray <struct struct_analys, struct struct_analys> *pArr;
  pArr = (CArray <struct struct_analys, struct struct_analys>*)pArray;
  
  //---------------------------------------
  //Open new log file
  //---------------------------------------
  //all time create new file
  LOG_EEP.NewFile(); 
  LOG_EEP.TimeStamp();

  //---------------------------------------
  //Header for *.csv version
  //---------------------------------------
  CString txt, txt_time_sec, txt_time, txt_led;
  txt.Format("%s;%s;%s;%s;%s;%s;%s;%s;%s\r\n", 
	          "Time Sec", "Time", "LEDx", "Chn 1", 
			  "Chn 2", "Chn 3", "Chn 4", "Chn 5", "Chn 6");
  
  //Write header
  LOG_EEP.Write(txt.GetBuffer());
  txt_time_sec.Empty(); 
  txt_time.Empty(); 
  txt_led.Empty();

  //-----
  unsigned int sample_number = pArr->GetCount();
  char leds=0;

  //parse data
  for(unsigned int i=0; i<sample_number; i++)
  {
	//time cell
	unsigned short time_sec = pArr->GetAt(i).x;

	for(int j=0; j<4; j++)
	{
	  txt_led = "  ";

	  int ch1 = pArr->GetAt(i).ch[j][0];
	  int ch2 = pArr->GetAt(i).ch[j][1];
	  int ch3 = pArr->GetAt(i).ch[j][2];
      int ch4 = pArr->GetAt(i).ch[j][3];
	  int ch5 = pArr->GetAt(i).ch[j][4];
      int ch6 = pArr->GetAt(i).ch[j][5];
	 
	  switch(j)
	  {
	    case 0: leds = 0x01; break;//R  
	    case 1: leds = 0x02; break;//G
	   	case 2: leds = 0x04; break;//B
		case 3: leds = 0x08; break;//Y
	  }

	  ///ParseLED(leds, &txt_led);
	  txt_led.Format("%d",j+1);

	  if(j==0)
	  { 
		txt_time_sec.Format("%d", time_sec);

		int min = time_sec/60;
	    int sec = time_sec -(min*60);
		
		if(min==0)
		{
		   txt_time.Format("%d", sec);
		}
		else
		{
		   txt_time.Format("%d:%d", min, sec);
		}
	  }
	  else {txt_time = ""; txt_time_sec = "";}

	  txt.Format("%s;%s;%s;%d;%d;%d;%d;%d;%d\r\n", 
		         txt_time_sec, txt_time, txt_led, 
				 ch1,ch2,ch3,ch4,ch5,ch6);
	  //Write data
      LOG_EEP.Write(txt.GetBuffer());
	} 
	  
	//Write empty string	
	txt = "\r\n";
    LOG_EEP.Write(txt.GetBuffer());
  }	 

  LOG_EEP.ShowFile();
}

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
