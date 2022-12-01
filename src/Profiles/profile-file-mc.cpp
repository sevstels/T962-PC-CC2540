//==============================================================================
//File name:    "profile-file-mc.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include <math.h>
#include "cmd.h"
#include "profile-file.h"

#include "files-cpp.h"
#include "log-file.h"

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
//
//------------------------------------------------------------------------------
unsigned short GetSampleSetup(float code)
{
  //коды цветов LEDs, упаковывано в 4х байтный формат
  char db_code[4];
  unsigned short led_code;

  //скопировать
  memcpy(db_code, &code, 4);

  //если число больше 2048 - это код RGBY
  if(db_code[3] == PCR_HEAD_RGBY_SAMPLE)
  {
    //----
    led_code = db_code[1];
    led_code <<= 8;
    led_code |= db_code[0];  
	led_code |= 0x8000;

	//leds
    return led_code;
  }

  //ошибка
  return 0;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float SetSampleSetup(unsigned short led_code)
{ 
  //коды цветов LEDs, упаковать в 4х байтный формат
  float code;
  char db_code[4];
  memset(db_code, 0, 4);
  
  led_code |= 0x8000;
  //----
  db_code[3] = PCR_HEAD_RGBY_SAMPLE;
  db_code[1] = (char)(led_code>>8);
  db_code[0] = (char)led_code;

  //скопировать
  memcpy(&code, db_code, 4);

  return code;
}

//------------------------------------------------------------------------------
//for microcontroller
//------------------------------------------------------------------------------
unsigned short SetSampleSetupMC(float code)
{
  //коды цветов LEDs, упаковывано в 4х байтный формат
  char db_code[4];
  unsigned short mc_code;

  //скопировать
  memcpy(db_code, &code, 4);
  
  //----
  mc_code = db_code[1];
  mc_code <<= 8;
  mc_code |= db_code[0];
  
  //если число больше 0x8000 - это код LED RGBY
  mc_code |= 0x8000;	
   
  TRACE1("MC LED Code: 0x%X\n", mc_code);

  return mc_code;
}

//------------------------------------------------------------------------------
//Pack pcr profile to microcontroller embedded file
//------------------------------------------------------------------------------
int CTPRF::CreateControllerFile(void)
{


  /*
  int header_length = 7;  //short, 14 bytes
  int file_crc_length = 1;//short, 2 bytes
  int flag_led_sample = 0;

  //data_length 1 step = x2bytes
  const int buf_length = header_length +  //[0] first 2 bytes
                         data_length +	  // + point`s data block size
                         file_crc_length; //[n] last 2 bytes
  
  //if file to short, exit
  if(data_length<2) return -1;

  //If need, clean mem from old data
  if(pFileBuf!=NULL)
  { 
    delete [] pFileBuf; 
    pFileBuf = NULL;
  }
  
  //Allocation memory for buffer
  pFileBuf = (unsigned short*) new unsigned short[buf_length];
  memset((void*)pFileBuf, 0xFF, buf_length<<1);
  pDevFileBuf = pFileBuf;
 
  //file header settings
  unsigned short  led_sample_numbers = 0;
  unsigned short  led_last_sample_pos = 0;
  unsigned short  led_in_use = 0;
  unsigned short  temperature_points_numbers = 0;
 
  unsigned short *pBuf = pFileBuf;
  //unsigned short *pInfo;
  //start crc calc from [1]
  //const char *pCrcBlock = (char*)&pFileBuf[1];
  unsigned short *pCRC = &pFileBuf[buf_length-1]; 

  //debug
  unsigned long address = (unsigned long)pBuf;
  address += buf_length<<1;

  //=========================================
  //File header
  //=========================================
  //[0], Header ID code, 2 bytes
  *pBuf++ = 0xA519;
  //[1], Header length, 2 bytes
  *pBuf++ = (unsigned short) header_length;
  //[2], PCR points length, 2 bytes
  *pBuf++ = (unsigned short) data_length;
  //[3], Total LED sample numbers, 2 bytes
  *pBuf++ = led_sample_numbers;
  //[4], Last LED sample position
  *pBuf++ = led_last_sample_pos;
  //[5], Total LED channels numbers in use, 2 bytes
  *pBuf++ = led_in_use;
  //[6], Total temperature points in profile
  *pBuf++ = temperature_points_numbers;
  //---- header end
  //pBuf[7]			    //First data point position here
  //......				//Data Block Body
  //pBuf[buf_length-1]  //Data Block CRC here, last 2 bytes
  
  TRACE("\n");
  CString txt, dig;
  txt = "Parsed: " + FileName + " file\n\n";
  txt += "PCR temperature table for 14bit ADC resolution:\n\n";
  
  //---------------------------------------
  //Open new log file
  //---------------------------------------
  CLOGFILE LOG_PROFILE;
  LOG_PROFILE.Ini("PCR_Profile", "txt");

  //all time create new file
  LOG_PROFILE.NewFile(); 
  LOG_PROFILE.TimeStamp();

  //---------------------------------------
  //Header for *.txt version
  //--------------------------------------- 
  //Write header
  LOG_PROFILE.Write(txt.GetBuffer());
  txt.Empty();

  //=========================================
  //conversion to short
  //=========================================
  for(int i=0; i<data_length; i++)
  {	
    txt = "PCR Point: ";
    dig.Format("%03d ", i+1);
    txt += dig;
    //TRACE1("PCR Point: %03d ", i);
    
    float pcr_temper = *pData++;
    txt += "Tempr: ";
    dig.Format("%2.3f ", pcr_temper);
    txt += dig;
    TRACE1("Tempr: %2.3f ", pcr_temper);
    
    //==== Поиск кода LEDs ======
    unsigned short code;
    if(pcr_temper>1000)
    {
      //код найден, упаковать для микроконтроллера
      code = SetSampleSetupMC(pcr_temper);
      flag_led_sample = 1;
	  led_sample_numbers++;
	  led_in_use |= (unsigned char)code;
	  led_last_sample_pos = (unsigned short)i;
    }
    else
    {
      //==== Conversion to ADC code for PID ======
      ///double temerature = pcr_temper;
      ///code = TMPR.CalcADCcode(temerature);
      code = pTMPR->GetNearADCcode(pcr_temper);
      flag_led_sample = 0;
	  temperature_points_numbers++;
    }

    if(code==0xFFFF)
    {AfxMessageBox(_T("Temperture Conversion Error"), MB_ICONSTOP); return -1;}

   *pBuf++ = code;
    
   if(flag_led_sample==0)
   {
    txt += "File code: ";
    dig.Format("%04d ", code);
    txt += dig;
    ///TRACE1("File code: %04d ", code);

    double dev_tempr = pTMPR->CalcTemperature(code);
    txt += "Device Tempr: ";
    dig.Format("%2.3f  ", dev_tempr);
    txt += dig;
    ///TRACE1("Device Tempr: %2.3f  ", dev_tempr);
    
    double err = pcr_temper - dev_tempr;
    if(err>0)txt += "Error: +";
    if(err<0)txt += "Error: -";
    if(err==0)txt += "Error:  ";
    dig.Format("%.3f\n", abs(err));
    txt += dig;	
    ///TRACE1("Error: %.3f\n", abs(err));
    TRACE(txt);
    LOG_PROFILE.Write(txt.GetBuffer());
   }
  }
  //Max error

  //=========================================
  //Add file additional information 
  //=========================================
  pFileBuf[3] = led_sample_numbers;
  pFileBuf[4] = led_last_sample_pos+1;
  pFileBuf[5] = led_in_use;
  pFileBuf[6] = temperature_points_numbers;
  
  txt = "\n";
  dig.Format("LED sample numbers:\t\t%d\n", led_sample_numbers);
  txt += dig;	
  dig.Format("Last sample LED position:\t%d\n", led_last_sample_pos+1);
  txt += dig;
  dig.Format("LED in use map:\t\t\t%d\n", led_in_use);
  txt += dig;
  dig.Format("Temperature points number:\t%d\n", temperature_points_numbers);
  txt += dig;
  dig.Format("Total profile points number:\t%d\n", data_length);
  txt += dig;

  LOG_PROFILE.Write(txt.GetBuffer());

  //=========================================
  //Add CRC 
  //=========================================
  unsigned short crc;
  //crc = shCRC((char*)&pFileBuf[1], (data_length+2)<<1);
  crc = shCRC((char*)&pFileBuf[1], (buf_length-2)<<1);
 *pCRC = crc;
  txt.Format("\nFile block crc:\t\t\t0x%X\n", crc);
  TRACE(txt);

  //total file length in bytes 
  controller_file_length = buf_length<<1; 
  dig.Format("File length bytes:\t\t%d\n", controller_file_length);  
  TRACE(dig);
  txt += dig;

  LOG_PROFILE.Write(txt.GetBuffer());
  */
  return 1;
}
