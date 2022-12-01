//==============================================================================
//File name:   "sample-log.cpp"
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
 /*
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPage4::SMPL_CheckLogFile(void)
{
  //Проверим, существует ли лог-файл  1
  int result = CheckFile("sample-log.txt");
  if(result<1) 
	 SMPL_LogFileCreate("sample-log.txt");

  //Проверим, существует ли лог-файл  2  
  result = CheckFile("sample-log.csv");
  if(result<1) 
	 SMPL_LogFileCreate("sample-log.csv");
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPage4::SMPL_LogFileCreate(CString file_name)
{ 
  CString txt = "";
/*  txt  = "#      Checking intensiry\n";
  txt += "#      Time    Temperature    Intensity\n\n"; 
  unsigned long data_length = txt.GetLength(); */
 /*
  Save_File(file_name,(char*)txt.GetBuffer(),0, 0);
  //Save_File("sample-log.csv",(char*)txt.GetBuffer(),0, 0);
}

//-----------------------------------------------------------------------------
//Data convertor
//-----------------------------------------------------------------------------
void CPage4::SMPL_LogFile(int intensity, char leds)
{
  //---- Calc TEC/PID temperature
  int tec_code = pDevice->ADC_value[CHN_TEC_TEMPER];
  float tec_temperature = pTMPR->CalcTemperature((unsigned short)tec_code); 
  
  //----
  CString txt, str, led, file = "sample-log.";
  
  led.Empty();	
  if((leds&0x01)==1)      led += "R";
  if(((leds>>1)&0x01)==1) led += "G";
  if(((leds>>2)&0x01)==1) led += "B";
  if(((leds>>3)&0x01)==1) led += "Y";

  //----------------------------------------
  //Format for *.txt version
  //----------------------------------------
  txt.Empty();
  CString time(pConsole->str_stamp.c_str());
  str = time + "\tIntens: %03d\tLEDs: "+led+"\n"; 
  txt.Format(str, intensity);

  //---- Add to file like *.txt
  file_name = file + "txt";
  int data_length = txt.GetLength();
  Add_to_File(file_name,(char*)txt.GetBuffer(), data_length);
  
  //---------------------------------------
  //Format for *.csv version
  //---------------------------------------
  txt.Empty();
  str = "%03d;"+led+"\n"; 
  txt.Format(str, intensity);

  //---- Add to file like *.txt
  file_name = file + "csv";
  data_length = txt.GetLength();
  Add_to_File(file_name,(char*)txt.GetBuffer(), data_length);
} */
