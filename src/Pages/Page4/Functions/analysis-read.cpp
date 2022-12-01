//==============================================================================
//File name:    "analysis-read.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page4.h"
#include "cmd.h"
 //---- Using Dll
//#include "candll.h"
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

//------------------------------------------------------------------------------
//Function: 
//------------------------------------------------------------------------------
unsigned char CRC_Calc(void *pBuffer, unsigned short size)
{
  unsigned char temp = 0; 
  unsigned short i = 0;
  unsigned char *pBuf = (unsigned char*)pBuffer;
    
  //----- Calc crc
  do {
    temp += *pBuf++;
    i++;
  }
  while(i<size);

  //return crc
  return ~temp;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int CPage4::AnalysisRead(void)
{
  CString str =_T("Read:"), str1, str_crc, arr="", arr1, crc, crc1, num;
  m_monitor.Clear();

  //----
  //это максимальный размер буфера в EEPROM устройства
  unsigned char dev_crc, pc_crc;
  unsigned char DataBufer[2048]; //0x800
  char *pBuf = (char*)DataBufer;
  memset(DataBufer, 0, sizeof(DataBufer));
  
  //(time,ch1[4],ch2[4],ch3[4],ch4[4],ch5[4],ch6[4])
  //2+6*2*4=50 число байт в еденичной строке записи
  int data_points_max = sizeof(DataBufer)/50;
  //сколько сэмплов войдет в память eeprom
  int block_length_max = data_points_max*50;

  //---- Preparation Progress Bar for programm process show 
  if(pProgress!= NULL)
  {
    pProgress->SetRange(0,(short)(block_length_max<<1));//from to	
    pProgress->SetPos(0);			        //start position 
    pProgress->SetStep(1);	                //Set the 1 step increment
    pProgress->SetBarColor(RGB(0, 0, 255)); //color
    pProgress->SetBkColor(RGB(0xff, 0xff, 0xff)); //color
    pProgress->ShowWindow(TRUE);            //Show window
  }
  
  m_monitor.SetWindowText(_T("Wait, get data..."));
  m_monitor.UpdateWindow();
  m_but_Analysis.EnableWindow(FALSE);

  //-------------------------------------------------------------------
  //---- Read Analysis array from device EEPROM 
  //-------------------------------------------------------------------
  unsigned char cmd[3]; 
  int result;
  int sample_points = 0;
  unsigned char index = 0;
  unsigned char answer[8];
  unsigned char RxLength = 3;
  unsigned char TxLength = 1;
  unsigned short data_length_bytes = 0;
  unsigned short data_sample_bytes = 50;

  cmd[0] = CMD_PCR_GET_ANALYSIS;
  cmd[1] = 0;//(unsigned char)index;
  cmd[2] = 0;//(unsigned char)(index>>8);
  memset(answer, 0, sizeof(answer));

  //--- получить длину массива  
  result = 0;/// CAN_ReadDevice(pCAN, CAN_device_ID, 0,
                ///         (unsigned char*)answer, RxLength,  
                   ///       cmd, TxLength, 200);
  //---- check
  if(result<0)
  {
    CString msg;
    m_monitor.SetWindowText(_T("Error"));
    m_monitor.UpdateWindow();

    msg = "Device not responding, try again";
    AfxMessageBox(_T(msg), MB_ICONWARNING);
    m_but_Analysis.EnableWindow(TRUE);
    return -1;
  }
  
  //длина массива для считывания
  memcpy(&data_length_bytes, answer, 2);
  //crc	массива
  dev_crc = answer[2];
  sample_points = data_length_bytes/data_sample_bytes;

  //---- check
  if(data_length_bytes<2 || data_length_bytes==0xFFFF)//5*2*2
  {
    CString msg;
   /// m_monitor.SetWindowText(_T("Should do PCR"));
    m_monitor.UpdateWindow();
    if(data_length_bytes==0xFF)
    { 
      msg = "Should run PCR process\r\nPresently the data buffer is empty";
    }
    else msg.Format("Not enough data to analyze: %d samples", sample_points);

    AfxMessageBox(_T(msg), MB_ICONWARNING);
    m_but_Analysis.EnableWindow(TRUE);
    return -1;
  }

  //-------------------------------------------
  //Prepare memory for data 
  //-------------------------------------------
  //data_sample_bytes
  unsigned char data[50];
  static CArray <struct struct_analys, struct struct_analys> ArrayAnalys;
  struct struct_analys one_sample;
  ArrayAnalys.RemoveAll();

  for(;;)
  {
    cmd[0] = CMD_PCR_READ_ANALYSIS;
    cmd[1] = index;
    cmd[2] = (unsigned char)data_sample_bytes;
    
    //--- считать массив построчно
    result = 0; ///CAN_ReadDevice(pCAN, CAN_device_ID, 0,
                   ///        (unsigned char*)data, data_sample_bytes,  
                      ///      cmd, 3, 300);
    //save data
    if(result==1)
    {
      memcpy(&one_sample, data, sizeof(struct_analys));
	  ArrayAnalys.Add(one_sample);
      //----    
	  memcpy(pBuf, data, data_sample_bytes);
      pBuf += data_sample_bytes;
    }

    index++;
    
	//Completed
    if(index>=sample_points) 
         break;
  }

  //------------------------------------------------------	
  //---- Check data array by CRC
  //------------------------------------------------------
  //Calk crc for PC data buffer
  pc_crc = CRC_Calc(DataBufer, data_length_bytes);
  str_crc.Format("%d", dev_crc);

  //Check CRC equality 
  if(dev_crc==pc_crc)
  {
     //num.Format(" %d points", index);
	 num.Format(" %d points", sample_points);
     str += num;
     str += "\r\nData block: OK\r\nChecksum: (CRC) OK";
     //str += str_crc;
     m_monitor.SetWindowText(str);			
     //if CRC is OK set it
     //pDevice->PCR_writed[m_PCR_number-1] = file_length;
     ///DEBUG CRC ::AfxMessageBox(_T(crc));
	 result = 1;
  }
  else
  {
     //if CRC is not OK set it
     //pDevice->PCR_writed[m_PCR_number-1] = 0;
     //err msg to user
     ///m_monitor.SetWindowText(_T("Error!\r\nChecksum: (CRC) is wrong!"));
	 str = "File is missing, or the\r\nresult is not ready.\r\n";
	 //str = "Start the measurement and wait until 2 measurements are completed";
	 m_monitor.SetWindowText(_T(str));
     m_but_Analysis.EnableWindow(TRUE);		  
     result = -1;
  }

  m_but_Analysis.EnableWindow(TRUE);

  //===============================================
  //Save sample points like text
  //===============================================
  LogFile_DevicePCR(&ArrayAnalys); 

  //===============================================
  //Create picture, Show graph
  //===============================================
  AnalysisGraph(&ArrayAnalys);

  ArrayAnalys.RemoveAll();
  return result;
}
