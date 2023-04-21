//==============================================================================
//File name:    "pcr-log.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page4.h"
#include "cmd.h"
#include "string.h"
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

//-----------------------------------------------------------------------------
//Data convertor
//-----------------------------------------------------------------------------
void CPage4::PID_LogGraphIni(void)
{ 
  PidLog.clear();
}

//-----------------------------------------------------------------------------
//Data convertor
//-----------------------------------------------------------------------------
void CPage4::PID_LogGraph(int point, int adc_code)
{
 /* ///
  //---- Calc TEC/PID temperature
  int tec_code = pDevice->ADC_value[CHN_TEC_TEMPER];
  float temperature = pTMPR->CalcTemperature((unsigned short)tec_code);

  PointFLT pt;
  pt.x = point;
  pt.y = temperature;
  PidLog.push_back(pt);
*/
  //---
  //GRPH.ParserDots(&PidLog);
}

//-----------------------------------------------------------------------------
//Data convertor
//-----------------------------------------------------------------------------
void CPage4::PID_LogFileCreate(void)
{
  CString txt;
  txt  = "#      Checking dynamic temperature error\n";
  txt += "#      Time    PCR set value    measure\n\n"; 
  unsigned long data_length = txt.GetLength();
  bool ret = Save_File("pid-log.txt",(char*)txt.GetBuffer(),0, data_length);
  PID_1sec_old = 25;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPage4::PCR_CheckLogFile(void)
{
  //Проверим, существует ли лог-файл  1
  int result = CheckFile("pcr-log.txt");
  if(result<1)
  {
    CString txt;
    txt  = "#      Checking intensiry\n";
    txt += "#      Time    Temperature    Intensity\n\n"; 
    unsigned long data_length = txt.GetLength();
    Save_File("pcr-log.txt",(char*)txt.GetBuffer(),0, data_length);
  }

  //Проверим, существует ли лог-файл  2  
  result = CheckFile("pcr-log.csv");
  if(result<1)
  {
    CString txt;
    txt  = "#      Checking intensiry\n";
    txt += "#      Time    Temperature    Intensity\n\n"; 
    unsigned long data_length = txt.GetLength();
    Save_File("pcr-log.csv",(char*)txt.GetBuffer(),0, 0);
  }
}

//-----------------------------------------------------------------------------
//Data convertor
//-----------------------------------------------------------------------------
void CPage4::PCR_LogFileCreate(void)
{
  CString txt;
  txt  = "#      Checking intensiry\n";
  txt += "#      Time    Temperature    Intensity\n\n"; 
  unsigned long data_length = txt.GetLength();
  Save_File("p-log.txt",(char*)txt.GetBuffer(),0, data_length);
  Save_File("p-log.csv",(char*)txt.GetBuffer(),0, 0);
}

//-----------------------------------------------------------------------------
//Data convertor
//-----------------------------------------------------------------------------
void CPage4::PID_LogFile(int point, int adc_code)
{  /*
  //---- Calc TEC/PID temperature
  int tec_code = pDevice->ADC_value[CHN_TEC_TEMPER];
  float tec_temperature = pTMPR->CalcTemperature((unsigned short)tec_code); 
  
  //---- Calc PCR Set temperature
  float pcr_temperature = pTMPR->CalcTemperature((unsigned short)adc_code);   
  
  //----
  CString txt, str, file = "pid-log.";
  
  //Format for *.txt version
  txt.Empty();
  str = "Sec: %4d\tPCR: %.3f\tTEC: %.3f\n"; 
  txt.Format(str, point, 
			      pcr_temperature,
				  PID_1sec_old);
                  //tec_temperature);

  PID_1sec_old = tec_temperature;

  //---- Add to file like *.txt
  file_name = file + "txt";
  int data_length = txt.GetLength();
  Add_to_File(file_name,(char*)txt.GetBuffer(), data_length);	*/
}

//-----------------------------------------------------------------------------
//Data convertor
//-----------------------------------------------------------------------------
void CPage4::PCR_LogFile(int intensity, char leds)
{ /*
  //---- Calc TEC/PID temperature
  int tec_code = pDevice->ADC_value[CHN_TEC_TEMPER];
  float tec_temperature = pTMPR->CalcTemperature((unsigned short)tec_code); 
  
  //----
  CString txt, str, led, file = "pcr-log.";
  
  led.Empty();	
  if((leds&0x01)==1) led += "R";
  if(((leds>>1)&0x01)==1) led += "G";
  if(((leds>>2)&0x01)==1) led += "B";
  if(((leds>>3)&0x01)==1) led += "Y";

  //----------------------------------------
  //Format for *.txt version
  //----------------------------------------
  txt.Empty();
  str = "Temperature: %.3f;\tIntens: %03d\tLEDs: "+led+"\n";
  txt.Format(str, tec_temperature, intensity);

  //---- Add to file like *.txt
  file_name = file + "txt";
  int data_length = txt.GetLength();
  Add_to_File(file_name,(char*)txt.GetBuffer(), data_length);
  
  //---------------------------------------
  //Format for *.csv version
  //---------------------------------------
  txt.Empty();
  str = "%.3f;%03d;"+led+"\n"; 
  txt.Format(str, tec_temperature, intensity);

  //---- Add to file like *.txt
  file_name = file + "csv";
  data_length = txt.GetLength();
  Add_to_File(file_name,(char*)txt.GetBuffer(), data_length);
  */
}
