#include "stdafx.h"
#include "lpc21isp.h"
#include "lpcprog.h"
#include <string>

extern LPC_DEVICE_TYPE LPCtypes[];

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int Chip_Erase(ISP_ENVIRONMENT *IspEnvironment)
{
  char tmpString[128];
  char Answer[128];

  DebugPrintf( "Chip Erase: ");
  int last_sector = LPCtypes[IspEnvironment->DetectedDevice].FlashSectors-1;
  sprintf(tmpString, "P %d %d\r\n", 0, last_sector); 
  
  //----------------------------------------------
  int result = BT_Send(tmpString);
  if(!result){ return (NO_ANSWER_QM);}
  
  //���� �����
  result = BT_Receive(Answer, sizeof(Answer), 5000);
  if(result==1)
  {
	std::string answ(Answer); 
	result = answ.find("\r\n0\r\n");
	if(result<0)
	{ 
      DebugPrintf("Wrong answer on Erase-Command\r\n");
      return 1000; //(WRONG_ANSWER_ERAS + GetAndReportErrorNumber(Answer));
	}
  }  

  DebugPrintf("OK\r\n");

  return 0;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int Sector_Erase(int sector)
{
  char tmpString[128];
  char Answer[128];
  int prepare_counter = 0;
  int erase_counter = 0;
  int empty_counter = 0;

  //----------------------------------------------------------
  //������� ������ 
  //----------------------------------------------------------
  DebugPrintf(" Erase");
	
  //Erase sector(s) E <start sector number> <end sector number>       
  sprintf(tmpString, "E %ld %ld\r\n", sector, sector);
  
  resend_secmd1:
  int result = BT_Send(tmpString);
  if(!result){Sleep(20); goto resend_secmd1;}
  	
  //���� �����
  resend_sccmd2:
  result = BT_Receive(Answer, sizeof(Answer), 5000);
  if(!result){Sleep(20); goto resend_sccmd2;}
  else
  {
	  std::string txt_answ(Answer);
	  result = txt_answ.find("\r\n0\r\n");	
	  if(result>0)
	  { 
	     //Get IAP Status code
		 DebugPrintf(" OK\r\n");
		 erase_counter = 0;
	  }
	  else
	  {
		if(sector==0) return 1;
		erase_counter++;
		if(erase_counter>10)
		{
		  DebugPrintf("Error!\r\n"); 
		  return -1;
		}

		Sleep(20);
		goto resend_secmd1;  
	  }
     }

  return 1;
}


//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int Sector_CheckEmpty(int sector)
{
  char tmpString[128];
  char Answer[128];
  int prepare_counter = 0;
  int erase_counter = 0;
  int empty_counter = 0;

  //----------------------------------------------------------	
  //ISP Blank check sector command
  //----------------------------------------------------------
  sprintf(tmpString, "I %ld %ld\r\n", sector, sector);
  resend_icmd1:
  int result = BT_Send(tmpString);
  if(!result){Sleep(20); goto resend_icmd1;}
  
  //���� �����
  resend_icmd2:
  result = BT_Receive(Answer, sizeof(Answer), 5000);
  if(!result){Sleep(20); goto resend_icmd2;}
  else
  { 
	//Get IAP Status code
	std::string txt_answ(Answer); 
	result = txt_answ.find("\r\n0\r\n");
	
	if(sector==0) result = 1;
	if(result>0)
	{
	  DebugPrintf("Empty");
	  return 1;
	}
	else
	{
	  DebugPrintf("No Empty");
	  return 0;
	}
  } 

  return -1;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int Sector_Prepare(int sector)
{
  char tmpString[32];
  char Answer[32];
  int prepare_counter = 0;
  int copy_counter = 0;
  int result;

  prepare_again_p:

  //----------------------------------------------------------	
  //����������� ������ � ������
  //----------------------------------------------------------
  DebugPrintf("Write ");
  sprintf(tmpString, "P %ld %ld\r\n", sector, sector);
  
  cmd_resend_p: 
  result = BT_Send(tmpString);
  if(!result){Sleep(20); DebugPrintf("CMD Err!\r\n"); goto cmd_resend_p;}
  
  //���� �����
  cmd_resend_p2:
  result = BT_Receive(Answer, sizeof(Answer), 10000);
  if(!result){Sleep(20); DebugPrintf("CMD Err!\r\n"); goto cmd_resend_p2;}
  else
  {
	std::string txt_answ(Answer); 
	//Get IAP Status code
	result = txt_answ.find("\r\n0\r\n");	
	if(result<0)
	{
	  DebugPrintf(" Error!\r\n");
      prepare_counter++;
      if(prepare_counter>10) return -1;
      goto prepare_again_p;
	}
	else{DebugPrintf( " OK");}
  }

  return 1;
} 

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int Copy_ToFlash(int flash_addr, int ram_addr, int length, int sector)
{
  char tmpString[32];
  char Answer[32];
  int prepare_counter = 0;
  int copy_counter = 0;
  int result;

  copy_again:
  
  //----------------------------------------------------------	
  //����������� �� RAM � FLASH
  //----------------------------------------------------------	
  //C <Flash address> <RAM address> <number of bytes>
  sprintf(tmpString, "C %ld %ld %ld\r\n", flash_addr, ram_addr, length);

  //��������
  cmd_resend3:
  result = BT_Send(tmpString);
  if(!result){Sleep(20); DebugPrintf("CMD Err!\r\n"); goto cmd_resend3;}
	 
  //���� �����
  cmd_resend4:
  result = BT_Receive(Answer, sizeof(Answer), 5000);
  if(!result){Sleep(20); DebugPrintf("CMD Err!\r\n"); goto cmd_resend4;}
  else
  {
	std::string boot_answ(Answer); 
	result = boot_answ.find(tmpString);
		
    if(result<0)
	{
      DebugPrintf("Error Copy to Flash Command\r\n");
      copy_counter++;
	  if(copy_counter>10) return -1;
	  goto copy_again;
	}
  }

  return 1;
} 

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int Sector_Verify(char *pCMD)
{
  char tmpString[32];
  char Answer[32];
  int counter = 0;
  int result;

  //----------------------------------------------------------	
  //����������� ������ � ������
  //----------------------------------------------------------
  DebugPrintf(", Verify ");
  
  result = BT_Send(pCMD);
  
  //���� �����
  result = BT_Receive(Answer, sizeof(Answer), 10000);
  if(!result){DebugPrintf("CMD Err!\r\n"); return -1;}
  else
  {
	std::string answ(Answer); 
	//Get IAP Status code
	result = answ.find("\r\n0\r\n");	
	if(result<0)
	{
	  DebugPrintf(" Error!\r\n");
      return -1;
	}
	else{DebugPrintf( " OK\r\n");}
  }	 

   return 1;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Write_ToRAM(int ram_addr, int length)
{
  char tmpString[32];
  char Answer[32];
  int result;

  //Write to RAM W <start address> <number of bytes>
  sprintf(tmpString, "W %ld %ld\r\n", ram_addr, length);

  int counter = 0;

  //��������
  cmd_wrresend:
  result = BT_Send(tmpString);
  if(!result){Sleep(20); DebugPrintf("CMD Err!\r\n"); goto cmd_wrresend;}
	 
  //���� �����
  cmd_wrresend2:
  result = BT_Receive(Answer, sizeof(Answer), 5000);
  if(!result){Sleep(20); DebugPrintf("CMD Err!\r\n"); goto cmd_wrresend2;}
  else
  {
	std::string boot_answ(Answer); 
	result = boot_answ.find("\r\n0\r\n");
		
    if(result<0)
	{
      DebugPrintf("Wrong answer on Write-Command\r\n");
      counter++;
	  if(counter>10) return -1;
	  goto cmd_wrresend;
	}
  }        

  return 1;
}

//-----------------------------------------------------------------------------
// Transfer blocks of 45 * 4 bytes to RAM
//-----------------------------------------------------------------------------
int Copy_ToRAM(ISP_ENVIRONMENT *IspEnvironment, long pos, int &line, int &CRC)
{
  int c, repeat, result, k = 0;
  char Answer[128]; 
  char tmpString[128];
  unsigned long tmpStringPos;
  
  // Each block 45 bytes
  for(int Block = 0; Block<4; Block++)  
  {
    //Uuencode one 45 byte block
    tmpStringPos = 0;
	 
	//Encode Length of block
    sendbuf[line][tmpStringPos++] = (char)(' ' + 45);   

    for(int BlockOffset = 0; BlockOffset < 45; BlockOffset++)
    {
      //RAM: Skip first 0x200 bytes, these are used by the download program in LPC21xx
      c = IspEnvironment->BinaryContent[pos + Block * 45 + BlockOffset + 0x200];
   
	  CRC += c;
      k = (k << 8) + (c & 255);
	
	  // Collecting always 3 Bytes, then do processing in 4 Bytes
      if((BlockOffset % 3) == 2)   
      {
        sendbuf[line][tmpStringPos++] = uuencode_table[(k >> 18) & 63];
        sendbuf[line][tmpStringPos++] = uuencode_table[(k >> 12) & 63];
        sendbuf[line][tmpStringPos++] = uuencode_table[(k >>  6) & 63];
        sendbuf[line][tmpStringPos++] = uuencode_table[ k        & 63];
      }
    }

    //DebugPrintf("\r\n");
    sendbuf[line][tmpStringPos++] = '\r';
    sendbuf[line][tmpStringPos++] = '\n';
    sendbuf[line][tmpStringPos++] = 0;
						
    char *SendSt = sendbuf[line];
    int length = strlen(sendbuf[line]);
  
    int repeet = 0;
    for(;;)
    {
      result = BT_Send(SendSt);
      //receive only for debug proposes
      result = BT_Receive(Answer, sizeof(Answer), 2000);						
      int ret = strncmp(SendSt, Answer, length); 
	  if(ret == 0)
	  {
		break;
	  }
	 
	  repeet++;

	  if(repeet>3)
	  {
        DebugPrintf("Error on send data\r\n");
        return -1;
	  }
    }

    //show progress
    pCPage6->Progress_Step();
    if(pCPage6->program_run!=1)
    {
  	  //user stop programming
  	  return -2;
    }
  	
	line++;

    //-----
    if(line==1)DebugPrintf("\r\n"); 
    DebugPrintf("%02d ", line);

    if(line == 20)
    {
      for(repeat = 0; repeat < 3; repeat++)
      {
       // DebugPrintf( "block_CRC = %ld\n", block_CRC);
       sprintf(tmpString, "%ld\r\n", CRC);

	   BT_Send(tmpString);
	   result = BT_Receive(Answer, sizeof(Answer), 2000);
	   if(result==1)
	   {
		 std::string boot_answ(Answer); 
	     result = boot_answ.find("OK");
	     if(result<0)
		 {	
			DebugPrintf( "Error on send block CRC\r\n");
	        line = 0;
	        CRC = 0;
			return -1;
		 }
	   }
	    
	  line = 0;
	  CRC = 0;
	  return 1;
    }
   }
  
  }

 return 1;
}
