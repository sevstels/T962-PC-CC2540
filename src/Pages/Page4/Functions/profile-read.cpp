//==============================================================================
//File name:    "profile-read.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page4.h"
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
int CPage4::DeviceProfileRead(void)
{ /*
  //AVR FLASH page size match, for data buffer
  short DataBufer[2048];

  CString str=_T("Read:"), str1, str_crc, arr, arr1, crc, crc1, num;
  arr = "";
  CPoint s;
  m_monitor.Clear();
//  m_waiting_time = 6;
		file_length = 1000;

  //---- Check Data array
  if(file_selected!=1) 
   {
     m_monitor.SetWindowText(_T("Select the PCR profile file for transfer!"));
     m_monitor.UpdateWindow();
     return -1;
   }

  //---- Check Data array
  if(file_parsed!=1) 
   {
     m_monitor.SetWindowText(_T("PCR profile file have errors!"));
     m_monitor.UpdateWindow();
     return -1;
   }

  //VARS
  int ret, errors=0;
  unsigned char dev_crc, pc_crc;
  int array_index = 0;

  //Limit array length
  if(file_length>2000)
  { 
	file_length = 2000;
    m_monitor.SetWindowText(_T("Warning: File to long.\r\nWrite only 2000 points."));
  }

  //Signal_number - internal AVR signal buffer variable index
  if(m_PCR_number==0)
   {
      m_monitor.SetWindowText(_T("Error: select 'PCR profile number'!"));
      m_monitor.UpdateWindow();
      return -1;
   } 

  //---- Preparation Progress Bar for programm process show 
  if(pProgress!= NULL)
  {
    pProgress->SetRange(0,(file_length<<1));//from to	
    pProgress->SetPos(0);			        //start position 
    pProgress->SetStep(1);	                //Set the 1 step increment
    pProgress->SetBarColor(RGB(0, 0, 255)); //color
    pProgress->SetBkColor(RGB(0xff, 0xff, 0xff)); //color
    pProgress->ShowWindow(TRUE);            //Show window
  }
  
  m_monitor.SetWindowText(_T("Wait, data transfer..."));
  m_monitor.UpdateWindow();
  m_but_ProfileWrite.EnableWindow(FALSE);

  //-------------------------------------------------------------------
  //---- Send init FLASH write condition + data parameters
  //-------------------------------------------------------------------
  ret = FLASH_WriteIni(pCAN, CAN_device_ID, m_PCR_number, file_length);
  if(ret<0)
   {
      m_monitor.SetWindowText(_T("Error: Remote device not replay."));
      m_monitor.UpdateWindow();
	  m_but_ProfileWrite.EnableWindow(TRUE);
      return -1;
   }  
  if(ret==0)
   {
      m_monitor.SetWindowText(_T("Error: Device internal problem."));
      m_monitor.UpdateWindow();
	  m_but_ProfileWrite.EnableWindow(TRUE);
      return -1;
   } 

  //-------------------------------------------------------------------
  //---- Fill data array and ret crc or error
  //-------------------------------------------------------------------
  CString old_txt; m_txt_PCRnumber.GetWindowTextA(old_txt);
  ret = FLASH_WriteArray(pCAN, CAN_device_ID, (unsigned char*)DataBufer, 
                         file_length, pProgress, &m_txt_PCRnumber);//&m_txt_msg);
  m_txt_PCRnumber.SetWindowTextA(old_txt);
  if(ret<0)
   {
      m_monitor.SetWindowText(_T("Error: Remote device not replay."));
      m_monitor.UpdateWindow();
	  m_but_ProfileWrite.EnableWindow(TRUE);
      return -1;
   }

  //------------------------------------------------------	
  //---- Check all data array by CRC
  //------------------------------------------------------
  dev_crc = (unsigned char)ret;  
            
  //Calk crc for PC data buffer
  pc_crc = CRC((unsigned char*)DataBufer, file_length<<1, &crc);
    
  str_crc.Format("%d", dev_crc);

  //Check CRC equality 
  if(dev_crc==pc_crc)
    {
      num.Format(" %d points", file_length);
      str += num;
      str += "\r\nData block: OK\r\nChecksum: (CRC) OK";
      //str += str_crc;

      //Print TAU+REF variables
      m_monitor.SetWindowText(str);			
            
      //if CRC is OK set it
      pDevice->PCR_writed[m_PCR_number-1] = file_length;

      //Set RIGHT CRC - Array ok writed
      CAN_SendCommand(pCAN, CAN_device_ID, 0, CMD_FLASH_WRITE_CRC, 
		               m_PCR_number-1, dev_crc);
      
      ///DEBUG CRC ::AfxMessageBox(_T(crc));
    }
   else
     {
       //if CRC is not OK set it
       pDevice->PCR_writed[m_PCR_number-1] = 0;
       //err msg to user
       m_monitor.SetWindowText(_T("Error: Data array CRC is wrong!"));
     }

  //update temperature indicator
 // m_restart_temperature = 1;
  m_but_ProfileWrite.EnableWindow(TRUE);   */
  return 1;
}