//==============================================================================
//File name:    "pid-tune.cpp"
//Purpose:      Source File, CPage2 page
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page2.h"
#include "cmd.h"
#include "files-cpp.h"

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

//------------------------------------------------------------------
//File format: *.txt
//------------------------------------------------------------------
// Num;   Data      Time  ;  PV  ; MV(or SP)
//14079;15.6.2012 15:36:19;448.75;49.43
//14080;15.6.2012 15:36:20;448.75;49.43

//------------------------------------------------------------------
//File format: *.csv
//------------------------------------------------------------------
//№     Время   MV   Время    PV
//Num;; Data ;   Time     S;   PV  ;;   Data ;   Time     S;MV(or SP)
//0;;13.05.11;09:35:50.000S;55.0000;;13.05.11;09:35:50.000S;52.9828
//1;;13.05.11;09:35:51.000S;55.0000;;13.05.11;09:35:51.000S;52.9841

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------  
void CPage2::PID_TestFileTxt(void)
{
  //-----------------------------------------
  SYSTEMTIME time;
  GetLocalTime(&time); 
  int year, month, day, hour, min, sec, msec; 
  year = (WORD)time.wYear;
  month =(BYTE)time.wMonth; 
  day  = (BYTE)time.wDay;
  hour = (BYTE)time.wHour;
  min =  (BYTE)time.wMinute;
  sec =  (BYTE)time.wSecond;
  msec = 0;//(WORD)time.wMilliseconds;
  
  int temperature = 23, voltage = 100;
  CString txt, for_save;
  for_save.Empty();

  for(int i=0; i<PID_sample_counter; i++)
  {
   txt.Format("%06d; %02d%s%02d%s%d %02d%s%02d%s%02d.%03d; %.3f; %.3f\r\n", 
              i, day,".", month,".", year,
              hour,":", min,":", sec, msec,
              temperature, voltage);
   
   for_save += txt;
   
   msec += 10;
   if(msec>=1000){msec = 0; sec++;}
  }

  unsigned long data_size = for_save.GetLength();
  Save_File("pid-samples.txt",(char*)for_save.GetBuffer(), 0, data_size);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------  
void CPage2::PID_TuneOpenFile(void)
{
  CString txt, file;
  txt = "";
  unsigned long data_length = txt.GetLength();
  //---- 1
  file = "identification-code.txt";
  Save_File(file,(char*)txt.GetBuffer(),0,data_length);
  //---- 2
  file = "identification-code.csv";
  Save_File(file,(char*)txt.GetBuffer(),0,data_length);
  //---- 3
  file = "identification-tmpr.csv";
  Save_File(file,(char*)txt.GetBuffer(),0,data_length);

  PID_sample_counter = 0;
  PID_tune_log = 1;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------  
void CPage2::PID_TuneSaveSample(int adc_value, int actuator_value, int period_ms)
{ /*
  //-----------------------------------------
  SYSTEMTIME time;
  GetLocalTime(&time); 
  int year, month, day, hour, min, sec, msec; 
  year = (WORD)time.wYear;
  month =(BYTE)time.wMonth; 
  day  = (BYTE)time.wDay;
  hour = (BYTE)time.wHour;
  min =  (BYTE)time.wMinute;
  sec =  (BYTE)time.wSecond;
  msec = (WORD)time.wMilliseconds;
  
  float device_actuator; 
  float	device_temperature;
  int int_device_actuator; 
  int int_device_temperature;
  unsigned int data_length;
  CString txt, str, file, file_name; 
  file = "identification-code.";
  
  //convert to real value
  device_temperature = (float)(1023-adc_value);// pTMPR->Calc((unsigned short)adc_value); 
  device_actuator = (float)actuator_value;
  int_device_temperature = 1023-adc_value;
  int_device_actuator = actuator_value; 
  
  //Format for *.txt version
  txt.Empty();
  //14079;15.6.2012 15:36:19;448.75;49.43
  /*txt.Format("%04d;%02d%s%02d%s%d %02d%s%02d%s%02d.%03d;%.2f;%.2f\r\n",//float
              PID_sample_counter, day,".", month,".", year,
              hour,":", min,":", sec, msec,
              device_temperature, device_actuator);*/
	/*
  txt.Format("%04d;%02d%s%02d%s%d %02d%s%02d%s%02d.%03d;%d;%d\r\n",//int 
              PID_sample_counter, day,".", month,".", year,
              hour,":", min,":", sec, msec,
              int_device_temperature, int_device_actuator);

  //---- Save like *.txt
  file_name = file + "txt";
  data_length = txt.GetLength();
  Add_to_File(file_name,(char*)txt.GetBuffer(), data_length);
  

  //Format for *.csv version
  txt.Empty();
  //1;;13.05.11;09:35:51.000S;55.0000;;13.05.11;09:35:51.000S;52.9841
  str = "%04d;;%02d.%02d.%2.d;%02d:%02d:%02d.%03dS;";
  str +="%.4f;;%02d.%02d.%2.d;%02d:%02d:%02d.%03dS;%.4f\r\n"; 
  txt.Format(str, PID_sample_counter, 
			      day, month, year, hour, min, sec, msec, 
			      device_actuator,
			      day, month, year, hour, min, sec, msec,
                  device_temperature);

  //---- Save like *.csv
  file_name = file + "csv";
  data_length = txt.GetLength();
  Add_to_File(file_name,(char*)txt.GetBuffer(), data_length);

  //---- 3
  file = "identification-tmpr.";
  device_temperature = (float)pTMPR->CalcTemperature((unsigned short)adc_value); 
  //Format for *.csv version
  txt.Empty();
  //1;;13.05.11;09:35:51.000S;55.0000;;13.05.11;09:35:51.000S;52.9841
  str = "%04d;;%02d.%02d.%2.d;%02d:%02d:%02d.%03dS;";
  str +="%.4f;;%02d.%02d.%2.d;%02d:%02d:%02d.%03dS;%.4f\r\n"; 
  txt.Format(str, PID_sample_counter, 
			      day, month, year, hour, min, sec, msec, 
			      device_actuator,
			      day, month, year, hour, min, sec, msec,
                  device_temperature);

  //---- Save like *.csv
  file_name = file + "csv";
  data_length = txt.GetLength();
  Add_to_File(file_name,(char*)txt.GetBuffer(), data_length);
 */
}
