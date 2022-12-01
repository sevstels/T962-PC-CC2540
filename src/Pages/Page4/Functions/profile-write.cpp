//==============================================================================
//File name:    "profile-write.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page4.h"
#include "cmd.h"
//#include "flash_avr.h"

 //---- Using Dll
//#include "candll.h"

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
//Function: calculation data CRC
//------------------------------------------------------------------------------
unsigned char CPage4::CRC(char *pBuffer, int length)
{  
  unsigned char temp = 0;
  
  //Calc crc
  do {
       //Get char variable from FLASH and load to buffer
       temp += *pBuffer++;
       length--;
     }
  while(length!=0);

  //crc
  return ~temp;
}

unsigned char CPage4::CRC8(char *pBuf, unsigned short length)
{
  unsigned char temp = 0; 
  unsigned short i = 0;
  
  //----- Create CRC for data block
  do {temp += *pBuf++; i++;}
  while (i < length);

  //Return crc
  return ~temp;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int CPage4::DeviceProfileWrite(void)
{
  int tx_block_num;
  CString str, str1;
  m_monitor.Clear();

  //Set file length in 
  int length = PCRF.pProfile->size();

  //---- Check Data array
  if(length<2 || length>pDevice->profile_length) 
   {
     str = "Profile error!";
	 if(length<2) str += "\r\nA very short";
	 if(length>length>pDevice->profile_length)
	  {
		 str1.Format(" %d" , pDevice->profile_length);
		 str += "\r\nLength over";
		 str += str1;
		 str += "points";
	  }
	 m_monitor.SetWindowText(_T(str));
	 m_monitor.UpdateWindow();
	 str += "\r\nFix it for continue.";
	 AfxMessageBox(_T(str), MB_ICONSTOP);
     return -1;
   }
			
  //VARS
  int ret, errors=0;
  unsigned char dev_crc, pc_crc, crc;
  int array_index = 0;

  m_monitor.SetWindowText(_T("Erase..."));
  m_monitor.UpdateWindow();
  pProgress->SetPos(0);

  //Erase
  pBT->Tx(CMD_ERASE_CUSTOM_PROFILE, NULL, 0);
  Sleep(1000);

  //div 4 передается 4 точки профиля
  tx_block_num = length>>2;

  //---- Preparation Progress Bar for programm process show 
  if(pProgress!= NULL)
  {
    pProgress->SetRange(0, tx_block_num);       //from to	
    pProgress->SetPos(0);			        //start position 
    pProgress->SetStep(1);	                //Set the 1 step increment
    pProgress->SetBarColor(RGB(0, 140, 0)); //color
    pProgress->SetBkColor(RGB(0xff, 0xff, 0xff)); //color
    pProgress->ShowWindow(TRUE);            //Show window
  }

  //Begin   
  m_but_ProfileWrite.EnableWindow(FALSE);
  m_monitor.SetWindowText(_T("Data transfer..."));
  m_monitor.UpdateWindow();


  //crc	 
  Point2D tpoint;
  unsigned short data[48];
  memset(data, 0, 96);

  for(int i=0; i<length; i++)
  {
   	tpoint =  PCRF.pProfile->at(i); 
	unsigned short point = (unsigned short)tpoint.y;
  	data[i] = point;

  }
  //----
  crc = CRC8((char*)&data, 96);

  //-------------------------------------------------------------------
  //---- Send
  //-------------------------------------------------------------------
  unsigned char index = 0;
  unsigned char offset = 0;
  unsigned char Buffer[2+8];
  for(int i=0; i<tx_block_num; i++)
  {
	unsigned short data[4] = {0,0,0,0};
	
	//----
	if(length>index)
	{
	  tpoint = PCRF.pProfile->at(index++);
	  data[0] = (unsigned short)tpoint.y;
	}
	
	//----
	if(length>index)
	{	 
	  tpoint = PCRF.pProfile->at(index++);
	  data[1] = (unsigned short)tpoint.y;  
	}

	//----
	if(length>index)
	{
	  tpoint = PCRF.pProfile->at(index++);
	  data[2] = (unsigned short)tpoint.y; 
	}

	//----
	if(length>index)
	{
	  tpoint = PCRF.pProfile->at(index++);
	  data[3] = (unsigned short)tpoint.y;
	}
	
	Buffer[0] = offset;
	offset += 8;
    Buffer[1] = 8; //length 
	memcpy(&Buffer[2], &data, 8);
	
	block_writed = 0;
	 	
	//Send block
	pBT->Tx(CMD_WRITE_PROFILE, (char*)Buffer, sizeof(Buffer));

	pProgress->StepIt();
	
	Sleep(500);

	//----
	if(index>=length)
	   break;

		 /*
	for(int i=0; i<20; i++)
	{
	  Sleep(50);
	  if(block_writed==1) break;
	}

	if(block_writed==0)
	{
      m_monitor.SetWindowText(_T("Write Error!"));
      m_monitor.UpdateWindow(); 
      m_but_ProfileWrite.EnableWindow(TRUE);
	  return -1;
	}	*/

  }

  m_monitor.SetWindowText(_T("Check.."));
  m_monitor.UpdateWindow(); 
  m_but_ProfileWrite.EnableWindow(TRUE);


  //Check crc
  pBT->Tx(CMD_CHECK_PROFILE_CRC, (char*)&crc, 1);


  return 1;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::CheckProfileCRC(char *pBuffer)
{
  char result;
  result = *pBuffer;

  if(result==1) 
  {
    m_monitor.SetWindowText(_T("Write Profile OK.."));
  }
  else
  {
    m_monitor.SetWindowText(_T("Profile Error!"));
  }

  m_monitor.UpdateWindow();   
}
