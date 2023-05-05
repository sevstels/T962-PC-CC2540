//==============================================================================
//File name:    "lpc21isp.cpp"
//Purpose:      Source File
//Version:      2.00
//https://github.com/capiman/lpc21isp
//https://github.com/Senseg/lpc21isp
//Based on code from:
//https://github.com/lnls-dig/lpc21isp/blob/4fbb2a2a9d06677bad9c3f32f3a7247735cc2154/lpc21isp.c
//==============================================================================
#include "stdafx.h"
#include "malloc.h"
#include "lpc21isp.h"
#include "lpcprog.h"
#include "datatx.h"


CDataTX *pBLE = NULL;
void SetTXpointer(void *p_BT)
{
  pBLE = (CDataTX *)p_BT;
}

int debug_level = 2;

static const unsigned int SectorTable_210x[] =
{
    8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192,
    8192, 8192, 8192, 8192, 8192, 8192, 8192
};

static const unsigned int SectorTable_2103[] =
{
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096
};

static const unsigned int SectorTable_2109[] =
{
    8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192
};

static const unsigned int SectorTable_211x[] =
{
    8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192,
    8192, 8192, 8192, 8192, 8192, 8192, 8192,
};

static const unsigned int SectorTable_212x[] =
{
    8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192,
    65536, 65536, 8192, 8192, 8192, 8192, 8192, 8192, 8192
};

// Used for devices with 500K (LPC2138 and LPC2148) and
// for devices with 504K (1 extra 4k block at the end)
static const unsigned int SectorTable_213x[] =
{
     4096,  4096,  4096,  4096,  4096,  4096,  4096,  4096,
    32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768,
    32768, 32768, 32768, 32768, 32768, 32768,  4096,  4096,
     4096,  4096,  4096,  4096
};

static int unsigned SectorTable_RAM[]  = { 65000 };

LPC_DEVICE_TYPE LPCtypes[] =
{
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, CHIP_VARIANT_NONE },  /* unknown */

   // id,        id2,  use id2, name of product,          flash size, ram size, total number of sector, max copy size, sector table, chip variant

   { 0x0004FF11, 0x00000000, 0, "2103",                           32,   8,  8, 4096, SectorTable_2103, CHIP_VARIANT_LPC2XXX },
   { 0xFFF0FF12, 0x00000000, 0, "2104",                          128,  16, 15, 8192, SectorTable_210x, CHIP_VARIANT_LPC2XXX },
   { 0xFFF0FF22, 0x00000000, 0, "2105",                          128,  32, 15, 8192, SectorTable_210x, CHIP_VARIANT_LPC2XXX },
   { 0xFFF0FF32, 0x00000000, 0, "2106",                          128,  64, 15, 8192, SectorTable_210x, CHIP_VARIANT_LPC2XXX },
   { 0x0201FF01, 0x00000000, 0, "2109",                           64,   8,  8, 4096, SectorTable_2109, CHIP_VARIANT_LPC2XXX },
   { 0x0101FF12, 0x00000000, 0, "2114",                          128,  16, 15, 8192, SectorTable_211x, CHIP_VARIANT_LPC2XXX },
   { 0x0201FF12, 0x00000000, 0, "2119",                          128,  16, 15, 8192, SectorTable_211x, CHIP_VARIANT_LPC2XXX },
   { 0x0101FF13, 0x00000000, 0, "2124",                          256,  16, 17, 8192, SectorTable_212x, CHIP_VARIANT_LPC2XXX },
   { 0x0201FF13, 0x00000000, 0, "2129",                          256,  16, 17, 8192, SectorTable_212x, CHIP_VARIANT_LPC2XXX },
   { 0x0002FF01, 0x00000000, 0, "2131",                           32,   8,  8, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0002FF11, 0x00000000, 0, "2132",                           64,  16,  9, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0002FF12, 0x00000000, 0, "2134",                          128,  16, 11, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0002FF23, 0x00000000, 0, "2136",                          256,  32, 15, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, ///+
   { 0x0002FF25, 0x00000000, 0, "2138",                          512,  32, 27, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0402FF01, 0x00000000, 0, "2141",                           32,   8,  8, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0402FF11, 0x00000000, 0, "2142",                           64,  16,  9, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0402FF12, 0x00000000, 0, "2144",                          128,  16, 11, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0402FF23, 0x00000000, 0, "2146",                          256,  40, 15, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0402FF25, 0x00000000, 0, "2148",                          512,  40, 27, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0301FF13, 0x00000000, 0, "2194",                          256,  16, 17, 8192, SectorTable_212x, CHIP_VARIANT_LPC2XXX },
   { 0x0301FF12, 0x00000000, 0, "2210",                            0,  16,  0, 8192, SectorTable_211x, CHIP_VARIANT_LPC2XXX }, /* table is a "don't care" */
   { 0x0401FF12, 0x00000000, 0, "2212",                          128,  16, 15, 8192, SectorTable_211x, CHIP_VARIANT_LPC2XXX },
   { 0x0601FF13, 0x00000000, 0, "2214",                          256,  16, 17, 8192, SectorTable_212x, CHIP_VARIANT_LPC2XXX },
   /*                           "2290"; same id as the LPC2210 */
   { 0x0401FF13, 0x00000000, 0, "2292",                          256,  16, 17, 8192, SectorTable_212x, CHIP_VARIANT_LPC2XXX },
   { 0x0501FF13, 0x00000000, 0, "2294",                          256,  16, 17, 8192, SectorTable_212x, CHIP_VARIANT_LPC2XXX },
   { 0x1600F701, 0x00000000, 0, "2361",                          128,  34, 11, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 4.1 -- 5 Sep 2012 */
   { 0x1600FF22, 0x00000000, 0, "2362",                          128,  34, 11, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 4.1 -- 5 Sep 2012 */
   { 0x0603FB02, 0x00000000, 0, "2364",                          128,  34, 11, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 01 -- 6 July 2007 */
   { 0x1600F902, 0x00000000, 0, "2364",                          128,  34, 11, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1600E823, 0x00000000, 0, "2365",                          256,  58, 15, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0603FB23, 0x00000000, 0, "2366",                          256,  58, 15, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 01 -- 6 July 2007 */
   { 0x1600F923, 0x00000000, 0, "2366",                          256,  58, 15, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1600E825, 0x00000000, 0, "2367",                          512,  58, 15, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0603FB25, 0x00000000, 0, "2368",                          512,  58, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 01 -- 6 July 2007 */
   { 0x1600F925, 0x00000000, 0, "2368",                          512,  58, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1700E825, 0x00000000, 0, "2377",                          512,  58, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x0703FF25, 0x00000000, 0, "2378",                          512,  58, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 01 -- 6 July 2007 */
   { 0x1600FD25, 0x00000000, 0, "2378",                          512,  58, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 01 -- 29 October 2007 */
   { 0x1700FD25, 0x00000000, 0, "2378",                          512,  58, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1700FF35, 0x00000000, 0, "2387",                          512,  98, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }, /* From UM10211 Rev. 03 -- 25 August 2008 */
   { 0x1800F935, 0x00000000, 0, "2387",                          512,  98, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1800FF35, 0x00000000, 0, "2388",                          512,  98, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1500FF35, 0x00000000, 0, "2458",                          512,  98, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1600FF30, 0x00000000, 0, "2460",                            0,  98,  0, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1600FF35, 0x00000000, 0, "2468",                          512,  98, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1701FF30, 0x00000000, 0, "2470",                            0,  98,  0, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX },
   { 0x1701FF35, 0x00000000, 0, "2478",                          512,  98, 28, 4096, SectorTable_213x, CHIP_VARIANT_LPC2XXX }
};


//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
unsigned long ReturnValueLpcRamStart(ISP_ENVIRONMENT *IspEnvironment)
{
  if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX)
  {
    return LPC_RAMSTART_LPC43XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC2XXX)
  {
    return LPC_RAMSTART_LPC2XXX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX)
  {
    return LPC_RAMSTART_LPC18XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC17XX)
  {
    return LPC_RAMSTART_LPC17XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC13XX)
  {
    return LPC_RAMSTART_LPC13XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC11XX)
  {
    return LPC_RAMSTART_LPC11XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC8XX)
  {
    return LPC_RAMSTART_LPC8XX;
  }
  DebugPrintf( "Error in ReturnValueLpcRamStart (%d)\n", LPCtypes[IspEnvironment->DetectedDevice].ChipVariant);

  return 1;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
unsigned long ReturnValueLpcRamBase(ISP_ENVIRONMENT *IspEnvironment)
{
  if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX)
  {
    return LPC_RAMBASE_LPC43XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC2XXX)
  {
    return LPC_RAMBASE_LPC2XXX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX)
  {
    return LPC_RAMBASE_LPC18XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC17XX)
  {
    return LPC_RAMBASE_LPC17XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC13XX)
  {
    return LPC_RAMBASE_LPC13XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC11XX)
  {
    return LPC_RAMBASE_LPC11XX;
  }
  else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC8XX)
  {
    return LPC_RAMBASE_LPC8XX;
  }
  DebugPrintf( "Error in ReturnValueLpcRamBase (%d)\n", LPCtypes[IspEnvironment->DetectedDevice].ChipVariant);

  return 1;
}
/*
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void ReceiveBT(const char *Ans, unsigned long MaxSize,
               unsigned long *RealSize, unsigned timeOutMilliseconds)
{
    unsigned char *Answer;
    unsigned char *endPtr;
    static char residual_data[128] = {'\0'};
	int length;

    Answer  = (unsigned char*) Ans;

    *RealSize = 0;
    endPtr = NULL;

	int result = BT_Receive(residual_data, sizeof(residual_data), timeOutMilliseconds);
	length = strlen(residual_data);
	memcpy((void*)Ans, residual_data, length);
	*RealSize = length;
} */

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void DebugPrintf(const char *fmt, ...)
{
  int lines;
  char pTemp[2000];
  memset(pTemp, 0, sizeof(pTemp));
  va_list ap;
      
  va_start(ap, fmt);
  //vprintf(fmt, ap);
  vsprintf(pTemp, fmt, ap);      
  va_end(ap);

  std::string msg(pTemp);
  
  pCPage6->txt_info += msg.c_str();
  
  pCPage6->m_edit_info.SetWindowTextA(pCPage6->txt_info);
  lines = pCPage6->m_edit_info.GetLineCount();
  pCPage6->m_edit_info.LineScroll(lines);
  //pCPage6->m_edit_info.UpdateWindow();
  ///TRACE(pTemp);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int BT_Send(const char *pString)
{
  int length = strlen(pString);
  pCPage6->ev_BootDataRx.ResetEvent();
  
  int result = pBLE->Tx(CMD_NRF_OVEN_PROG_DATA, (char*)pString, length);

  //100 ms delay after each block (makes lpc21isp to work with bad UARTs)
  Sleep(20); 

  return result;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int BT_Receive(void *pBuf, int size, unsigned timeout)
{  
  memset(pBuf, 0, size);
  
  DWORD ret = WaitForSingleObject(pCPage6->ev_BootDataRx.m_hObject, timeout);
  //TRACE1("%d\n", ret);

  int copy_length;
  if(ret==0)
  {
	int length = pCPage6->bootloader_msg.length();
	if(length<=size) copy_length = length;
	else { copy_length = size;}

	memcpy(pBuf, pCPage6->bootloader_msg.c_str(), copy_length);
	return 1;
  }

  /*
  if(ok==1) TRACE1("%s\n", buf2);
  */

  return -1;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
static void NxpOutputErrorMessage(unsigned char ErrorNumber)
{
    switch (ErrorNumber)
    {
    case   0:
        DebugPrintf( "CMD_SUCCESS\n");
        break;

    case   1:
        DebugPrintf( "INVALID_COMMAND\n");
        break;

    case   2:
        DebugPrintf( "SRC_ADDR_ERROR: Source address is not on word boundary.\n");
        break;

    case   3:
        DebugPrintf( "DST_ADDR_ERROR: Destination address is not on a correct boundary.\n");
        break;

    case   4:
        DebugPrintf( "SRC_ADDR_NOT_MAPPED: Source address is not mapped in the memory map.\n"
                       "                     Count value is taken into consideration where applicable.\n");
        break;

    case   5:
        DebugPrintf( "DST_ADDR_NOT_MAPPED: Destination address is not mapped in the memory map.\n"
                       "                     Count value is taken into consideration where applicable.\n");
        break;

    case   6:
        DebugPrintf( "COUNT_ERROR: Byte count is not multiple of 4 or is not a permitted value.\n");
        break;

    case   7:
        DebugPrintf( "INVALID_SECTOR: Sector number is invalid or end sector number is\n"
                       "                greater than start sector number.\n");
        break;

    case   8:
        DebugPrintf( "SECTOR_NOT_BLANK\n");
        break;

    case   9:
        DebugPrintf( "SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION:\n"
                       "Command to prepare sector for write operation was not executed.\n");
        break;

    case  10:
        DebugPrintf( "COMPARE_ERROR: Source and destination data not equal.\n");
        break;

    case  11:
        DebugPrintf( "BUSY: Flash programming hardware interface is busy.\n");
        break;

    case  12:
        DebugPrintf( "PARAM_ERROR: Insufficient number of parameters or invalid parameter.\n");
        break;

    case  13:
        DebugPrintf( "ADDR_ERROR: Address is not on word boundary.\n");
        break;

    case  14:
        DebugPrintf( "ADDR_NOT_MAPPED: Address is not mapped in the memory map.\n"
                       "                 Count value is taken in to consideration where applicable.\n");
        break;

    case  15:
        DebugPrintf( "CMD_LOCKED\n");
        break;

    case  16:
        DebugPrintf( "INVALID_CODE: Unlock code is invalid.\n");
        break;

    case  17:
        DebugPrintf( "INVALID_BAUD_RATE: Invalid baud rate setting.\n");
        break;

    case  18:
        DebugPrintf( "INVALID_STOP_BIT: Invalid stop bit setting.\n");
        break;

    case  19:
        DebugPrintf( "CODE READ PROTECTION ENABLED\n");
        break;

    case 255:
        break;

    default:
        DebugPrintf( "unknown error %u\n", ErrorNumber);
        break;
    }

    //DebugPrintf( "error (%u), see  NxpOutputErrorMessage() in lpc21isp.c for help \n\r", ErrorNumber);
}

/***************************** GetAndReportErrorNumber ***************************/
/**  Find error number in string.  This will normally be the string
returned from the microcontroller.
\param [in] Answer the buffer to search for the error number.
\return the error number found, if no linefeed found before the end of the
string an error value of 255 is returned. If a non-numeric value is found
then it is printed to stdout and an error value of 255 is returned.
*/
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
static unsigned char GetAndReportErrorNumber(const char *Answer)
{
    unsigned char Result = 0xFF; // Error !!!
    unsigned int i = 0;

    while (1)
    {
        if (Answer[i] == 0x00)
        {
            break;
        }

        if (Answer[i] == 0x0a)
        {
            i++;

            if (Answer[i] < '0' || Answer[i] > '9')
            {
                DebugPrintf( "ErrorString: %s", &Answer[i]);
                break;
            }

            Result = (unsigned char) (atoi(&Answer[i]));
            break;
        }

        i++;
    }

    NxpOutputErrorMessage(Result);

    return Result;
}

#include <stdio.h>

  //-----------------------------------------------------------------
  //vars Puffer for data to resend after "RESEND\r\n" Target responce
  char sendbuf0[128];
  char sendbuf1[128];
  char sendbuf2[128];
  char sendbuf3[128];
  char sendbuf4[128];
  char sendbuf5[128];
  char sendbuf6[128];
  char sendbuf7[128];
  char sendbuf8[128];
  char sendbuf9[128];
  char sendbuf10[128];
  char sendbuf11[128];
  char sendbuf12[128];
  char sendbuf13[128];
  char sendbuf14[128];
  char sendbuf15[128];
  char sendbuf16[128];
  char sendbuf17[128];
  char sendbuf18[128];
  char sendbuf19[128];

  char * sendbuf[20] = { sendbuf0,  sendbuf1,  sendbuf2,  sendbuf3,  sendbuf4,
                         sendbuf5,  sendbuf6,  sendbuf7,  sendbuf8,  sendbuf9,
                         sendbuf10, sendbuf11, sendbuf12, sendbuf13, sendbuf14,
                         sendbuf15, sendbuf16, sendbuf17, sendbuf18, sendbuf19};

  char uuencode_table[64];
  int kk;
  unsigned long block_CRC = 0;
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int NxpDownload(ISP_ENVIRONMENT *IspEnvironment)
{
//  unsigned long realsize;
  char Answer[128];
  char *endPtr;
  int found;
  unsigned long Sector;
  unsigned long SectorLength;
  unsigned long SectorStart, SectorOffset, SectorChunk;
  char tmpString[128];
  //unsigned long BlockOffset;
  //unsigned long Block;
  unsigned long Pos;
  unsigned long Id[2];
  unsigned long Id1Masked;
  unsigned long CopyLength;
  int i;
  //CRC over interrupt vector table
  unsigned long ivt_CRC;
  int repeat = 0;
  int result;
  kk = 0;

  //очистить буфер экрана 
  pCPage6->txt_info.Empty();
  pCPage6->Progress_Ini(IspEnvironment->BinaryLength);

  //===========================================================
  //Переключить процессор печи в режим программирования
  //===========================================================
  //Смотрим в сетапе у печки возможность перехода прямо в бут
/*  if(pCPage6->pDevice->Bootloader>0)
  {
  
  }*/
  /*
  result = pBLE->Tx(CMD_CHECK_BOOTLOADER, NULL, 0);
  if(!result){ return (NO_ANSWER_QM);}

  result = BT_Receive(Answer, sizeof(Answer), 1000);
  if(result==1)
  {
	 std::string boot_answ(Answer); 
	 result = boot_answ.find("BootOK");
	 if(result>=0)
	 {
		DebugPrintf("Jump to Bootloader mode\r\n");
	 }
  }
  else 
  {
     result = pBLE->Tx(CMD_NRF_OVEN_JUMP_TO_BOOTLOADER, NULL, 0);
     if(!result){ return (NO_ANSWER_QM);} 
	// DebugPrintf("Jump to Bootloader mode by Reset\r\n");
  }	*/	 

  result = pBLE->Tx(CMD_NRF_OVEN_ISP_MODE, NULL, 0);
 /* if(!result){ return (NO_ANSWER_QM);} 
  */
 // result = pBLE->Tx(CMD_NRF_OVEN_JUMP_TO_BOOTLOADER, NULL, 0);

  //===========================================================
  //ждем перехода в загрузку 
  Sleep(1000);
  DebugPrintf("Connect ");
  found = 0;

  //инициализация auto boud
  for(int i=0; i<40; i++)
  {	  
	 //посылаем ? для работы алгоритма
     result = BT_Send("?\r\n");
	 if(!result)
	 { 
	   return (NO_ANSWER_QM);
	 }
	 
	 //ждем ответ
	 result = BT_Receive(Answer, sizeof(Answer), 200);
	 if(result==1)
	 {
	    std::string boot_answ(Answer); 
		result = boot_answ.find("Synchronized");
		
		if(result>=0)
		{
		  found = 1;
		  break;
		}
	 }

	 DebugPrintf(".");
  }

  if(found==0)
  {
    DebugPrintf(" Error\r\n");
	  
	//no answer on '?'\n
	DebugPrintf("Chip no answer, Reset\r\n");
	//Reset device!
	///pBLE->Tx(CMD_NRF_OVEN_RESET, NULL, 0);
	Sleep(1000);
    return (NO_ANSWER_QM);
  }

  DebugPrintf( " OK\r\n");


  //===========================================================
  DebugPrintf("Synchronized: ");
  //проверка перехода процессора в boot mode по эху
  //53 79 6E 63 68 72 6F 6E 69 7A 65 64 0D 0A 
  found=0;
  result = BT_Send("Synchronized\r\n");
  if(!result){ return (NO_ANSWER_QM);}
 
  //ждем ответ for Synchronized = OK
  result = BT_Receive(Answer, sizeof(Answer), 1000);
  if(result==1)
  {
	 std::string boot_answ(Answer); 
     result = boot_answ.find("Synchronized\r\nOK");
		
	 if(result>=0)
	 {
		found = 1;
	 }
  }	

  if(found==0)
  {
    DebugPrintf( "Error, No answer, Reset\r\n");
    //Reset device!
	pBLE->Tx(CMD_NRF_OVEN_RESET, NULL, 0);
	Sleep(1000);
    return (NO_ANSWER_SYNC);
  }	

  DebugPrintf("OK\r\n");

  //===========================================================
  DebugPrintf("Set Oscillator Freq: ");  
  found = 0; 
  result = BT_Send("014740\r\n");
  if(!result){ return (NO_ANSWER_QM);}
  
  //ждем ответ
  result = BT_Receive(Answer, sizeof(Answer), 1000);
  if(result==1)
  {
	std::string answ(Answer); 
	result = answ.find("014740\r\nOK");	
	if(result>=0)
	{ 
	   found = 1;
	   DebugPrintf("14740 KHz\r\n"); 
	}
  }  
	
  if(found==0)
  {
    DebugPrintf( "Error, No answer\r\n");
	//Reset device!
	pBLE->Tx(CMD_NRF_OVEN_RESET, NULL, 0);
    return (NO_ANSWER_OSC);
  }

  //===========================================================
  DebugPrintf("Chip Unlock: ");

  BT_Send("U 23130\r\n");
  result = BT_Receive(Answer, sizeof(Answer), 1000);
  if(!result)
  {
    DebugPrintf( "Error\r\n");
	//Reset device!
	pBLE->Tx(CMD_NRF_OVEN_RESET, NULL, 0);
    return (UNLOCK_ERROR + GetAndReportErrorNumber(Answer));
  }
  DebugPrintf("OK\r\n");

  //===========================================================
  DebugPrintf( "BootCode version: ");

  BT_Send("K\r\n");
  result = BT_Receive(Answer, sizeof(Answer), 1000);
  if(result != 1)
  {
    DebugPrintf( "Error, No answer\r\n");
    return (NO_ANSWER_RBV);
  }
  else
  {
    std::string answ(Answer); 
	result = answ.find("\r\n0\r\n");	
	if(result>0)
	{ 
	  answ.erase(0, result+5);

	  int maj, min, build;
	  if(sscanf(answ.c_str(), "%d %d %d", &build, &min, &maj) == 2) 
	  {
        maj = min;
        min = build;
        build = 0;
		DebugPrintf( "%d.%d.%d\r\n", maj, min, build);
      }
      else
      {
        DebugPrintf("Unknown\r\n");
      }
	}
  }

  //===========================================================
  DebugPrintf("Part ID: ");
  
  BT_Send("J\r\n");
  result = BT_Receive(Answer, sizeof(Answer), 1000);
  memcpy(Answer, &Answer[6], 6);
  memset(&Answer[6], 0, 12);
  
  //----
  std::string id(Answer);
  endPtr = "\r\n";

  Id[0] = strtoul(id.c_str(), &endPtr, 10);
  Id[1] = 0UL;
  
  i = sizeof(LPCtypes)/sizeof(LPCtypes[0])-1;
  for (i; i > 0 && LPCtypes[i].id != Id[0]; i--){};
  IspEnvironment->DetectedDevice = i;
  
  if(LPCtypes[IspEnvironment->DetectedDevice].EvalId2 != 0)
  {
        /* Read out the second configuration word and run the search again */
        *endPtr = '\n';
        endPtr++;
        if ((endPtr[0] == '\0') || (endPtr[strlen(endPtr)-1] != '\n'))
        {
            /* No or incomplete word 2 */
			//debug this!!!
		    result = BT_Receive(Answer, sizeof(Answer), 1000);
        }

		//отладить это
        ///FormatCommand(endPtr, endPtr);
        if ((*endPtr == '\0') || (*endPtr == '\n'))
        {
            DebugPrintf( "incomplete answer on Read Part Id (second configuration word missing)\r\n");
            return (NO_ANSWER_RPID);
        }

        Id[1] = strtoul(endPtr, &endPtr, 10);
        *endPtr = '\0'; /* delete \r\n */

        Id1Masked = Id[1] & 0xFF;

        /* now search the table again */
        for (i = sizeof LPCtypes / sizeof LPCtypes[0] - 1; i > 0 && (LPCtypes[i].id != Id[0] || LPCtypes[i].id2 != Id1Masked); i--)
            /* nothing */;
        IspEnvironment->DetectedDevice = i;
    }

    if(IspEnvironment->DetectedDevice == 0)
	{
        DebugPrintf( "unknown");
    }
    else 
	{
        DebugPrintf( "LPC%s, %d kiB FLASH / %d kiB SRAM",
            LPCtypes[IspEnvironment->DetectedDevice].Product,
            LPCtypes[IspEnvironment->DetectedDevice].FlashSize,
            LPCtypes[IspEnvironment->DetectedDevice].RAMSize);
    }
    
	if(LPCtypes[IspEnvironment->DetectedDevice].EvalId2 != 0)
    {
        DebugPrintf( " (0x%08lX / 0x%08lX -> %08lX)\r\n", Id[0], Id[1], Id1Masked);
    }
    else
    {
        DebugPrintf( " (0x%08lX)\r\n", Id[0]);
    }

    if(!IspEnvironment->DetectOnly)
    {
        // Build up uuencode table
        uuencode_table[0] = 0x60;           // 0x20 is translated to 0x60 !

        for (i = 1; i < 64; i++)
        {
            uuencode_table[i] = (char)(0x20 + i);
        }

        if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC2XXX)
        {
            // Patch 0x14, otherwise it is not running and jumps to boot mode
            ivt_CRC = 0;

            // Clear the vector at 0x14 so it doesn't affect the checksum:
            for (i = 0; i < 4; i++)
            {
                IspEnvironment->BinaryContent[i + 0x14] = 0;
            }

            // Calculate a native checksum of the little endian vector table:
            for (i = 0; i < (4 * 8);) {
                ivt_CRC += IspEnvironment->BinaryContent[i++];
                ivt_CRC += IspEnvironment->BinaryContent[i++] << 8;
                ivt_CRC += IspEnvironment->BinaryContent[i++] << 16;
                ivt_CRC += IspEnvironment->BinaryContent[i++] << 24;
            }

            /* Negate the result and place in the vector at 0x14 as little endian
            * again. The resulting vector table should checksum to 0. */
            ivt_CRC = (unsigned long) (0 - ivt_CRC);
            for (i = 0; i < 4; i++)
            {
                IspEnvironment->BinaryContent[i + 0x14] = (unsigned char)(ivt_CRC >> (8 * i));
            }

            DebugPrintf("Position 0x14 patched: ivt_CRC = 0x%08lX\r\n", ivt_CRC);
        }
        else if(LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC43XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC18XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC17XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC13XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC11XX ||
                LPCtypes[IspEnvironment->DetectedDevice].ChipVariant == CHIP_VARIANT_LPC8XX)
        {
            // Patch 0x1C, otherwise it is not running and jumps to boot mode

            ivt_CRC = 0;

            // Clear the vector at 0x1C so it doesn't affect the checksum:
            for (i = 0; i < 4; i++)
            {
                IspEnvironment->BinaryContent[i + 0x1C] = 0;
            }

            // Calculate a native checksum of the little endian vector table:
            for (i = 0; i < (4 * 8);) {
                ivt_CRC += IspEnvironment->BinaryContent[i++];
                ivt_CRC += IspEnvironment->BinaryContent[i++] << 8;
                ivt_CRC += IspEnvironment->BinaryContent[i++] << 16;
                ivt_CRC += IspEnvironment->BinaryContent[i++] << 24;
            }

            /* Negate the result and place in the vector at 0x1C as little endian
            * again. The resulting vector table should checksum to 0. */
            ivt_CRC = (unsigned long) (0 - ivt_CRC);
            for (i = 0; i < 4; i++)
            {
                IspEnvironment->BinaryContent[i + 0x1C] = (unsigned char)(ivt_CRC >> (8 * i));
            }

            DebugPrintf("Position 0x1C patched: ivt_CRC = 0x%08lX\r\n", ivt_CRC);
        }
        else
        {
          DebugPrintf( "Internal error: wrong chip variant %d (detected device %d)\r\n", LPCtypes[IspEnvironment->DetectedDevice].ChipVariant, IspEnvironment->DetectedDevice);
          return -1;
        }
    }

    /* In case of a download to RAM, use full RAM for downloading
    * set the flash parameters to full RAM also.
    * This makes sure that all code is downloaded as one big sector */
    if ( (IspEnvironment->BinaryOffset >= ReturnValueLpcRamStart(IspEnvironment))
       &&(IspEnvironment->BinaryOffset + IspEnvironment->BinaryLength <= ReturnValueLpcRamStart(IspEnvironment)+(LPCtypes[IspEnvironment->DetectedDevice].RAMSize*1024)))
    {
        LPCtypes[IspEnvironment->DetectedDevice].FlashSectors = 1;
        LPCtypes[IspEnvironment->DetectedDevice].MaxCopySize  = LPCtypes[IspEnvironment->DetectedDevice].RAMSize*1024 - (ReturnValueLpcRamBase(IspEnvironment) - ReturnValueLpcRamStart(IspEnvironment));
        LPCtypes[IspEnvironment->DetectedDevice].SectorTable  = SectorTable_RAM;
        SectorTable_RAM[0] = LPCtypes[IspEnvironment->DetectedDevice].MaxCopySize;
    }

    if(IspEnvironment->DetectOnly) return (0);

    // Start with sector 1 and go upward... Sector 0 containing the interrupt vectors
    // will be loaded last, since it contains a checksum and device will re-enter
    // bootloader mode as long as this checksum is invalid.
    DebugPrintf("Will start programming at Sector 1\r\nSector 0 to ensure that checksum is written last\r\n");
    if(LPCtypes[IspEnvironment->DetectedDevice].SectorTable[0] >= IspEnvironment->BinaryLength)
    {
        Sector = 0;
        SectorStart = 0;
    }
    else
    {
        SectorStart = LPCtypes[IspEnvironment->DetectedDevice].SectorTable[0];
        Sector = 1;
    }
	/*
	//---------------------------------------------
	//Erase
	//---------------------------------------------
    if(IspEnvironment->WipeDevice == 1) 
	{ 
	  int ret = EraseChip(IspEnvironment);
	  if(ret !=0) return ret;
	}
    else
	{ 
	//   int ret = EraseSector(IspEnvironment);
	//   if(ret !=0) return ret;
    } */

	
  int ret;
 // ret = Chip_Erase(IspEnvironment);
  Sleep(100);
  /*
  for(i=0;i<9; i++){ DebugPrintf("\r\nSector %ld: ", i); ret = EraseSector(IspEnvironment, i);}
  // return 0;	 */
  //EraseSector(IspEnvironment, 0);
  
 // Sector = 8;

  //---------------------------------------------
  //Write loop
  //---------------------------------------------
  while(1)
  {
    DebugPrintf("\r\nSector %ld: ", Sector);
	
    //----
    if(Sector >= LPCtypes[IspEnvironment->DetectedDevice].FlashSectors)
    {
      DebugPrintf( "\r\nProgram too large, out of Flash memory.\r\n");
      return (PROGRAM_TOO_LARGE);
    }		
	
	//Проверить сектор на чистоту
	result = Sector_CheckEmpty(Sector);
	if(result!=1)
	{ 
	  //сектор грязный
	  result = Sector_Erase(Sector);
	  if(result!=1)  return 1000;
	}
	        
	SectorLength = LPCtypes[IspEnvironment->DetectedDevice].SectorTable[Sector];
    if(SectorLength > IspEnvironment->BinaryLength - SectorStart)
    {
      SectorLength = IspEnvironment->BinaryLength - SectorStart;
    }
 
	for(SectorOffset = 0; SectorOffset < SectorLength; SectorOffset += SectorChunk)
    {
      // Check if we are to write only 0xFFs - it would be just a waste of time..
      if(SectorOffset == 0)
	  {
        for(SectorOffset = 0; SectorOffset < SectorLength; ++SectorOffset)
        {
          if(IspEnvironment->BinaryContent[SectorStart + SectorOffset] != 0xFF)
          break;
        }

        // all data contents were 0xFFs        
		if(SectorOffset == SectorLength) 
        {
          DebugPrintf( "Whole sector contents is 0xFFs, skipping programming.");
          break;
        }

        // re-set otherwise        
		SectorOffset = 0; 
      }
	  
	  //Add a visible marker between segments in a sector
      if(SectorOffset > 0)
      {
        //means: partial segment copied 
        DebugPrintf( "|");  
      }

      //If the Flash ROM sector size is bigger than the number of bytes
      // we can copy from RAM to Flash, we must "chop up" the sector and
      // copy these individually.
      // This is especially needed in the case where a Flash sector is
      // bigger than the amount of SRAM.
      SectorChunk = SectorLength - SectorOffset;
      if(SectorChunk > (unsigned)LPCtypes[IspEnvironment->DetectedDevice].MaxCopySize)
      {
        SectorChunk = LPCtypes[IspEnvironment->DetectedDevice].MaxCopySize;
      }

      //Write multiple of 45 * 4 Byte blocks to RAM, but copy maximum of on sector to Flash
      //In worst case we transfer up to 180 byte too much to RAM
      //but then we can always use full 45 byte blocks and length is multiple of 4
      CopyLength = SectorChunk;
      if((CopyLength % (45 * 4)) != 0)
      {
        CopyLength += ((45 * 4) - (CopyLength % (45 * 4)));
      }

	  //Write to RAM W <start address> <number of bytes>
	  unsigned long ram_addr = ReturnValueLpcRamBase(IspEnvironment);
	  result = Write_ToRAM(ram_addr, CopyLength);
      if(result!=1)
      {
        DebugPrintf( "Wrong answer on Write-Command\n");
        return (WRONG_ANSWER_WRIT + GetAndReportErrorNumber(Answer));
      }

      Resend:
      int Line = 0;
	  unsigned long block_crc=0;

      //Transfer blocks of 45 * 4 bytes to RAM
      for(Pos = SectorStart + SectorOffset; 
		 (Pos < SectorStart + SectorOffset + CopyLength) && 
		 (Pos < IspEnvironment->BinaryLength); Pos += (45 * 4))
      {
		int ret = Copy_ToRAM(IspEnvironment, Pos, Line, block_crc);
		if(ret!=1){ return 1000;}
	  }
		
	  if(Line != 0)
      {
	    result = Check_CRC(block_crc);
        if(result<0)
	    {	
		  DebugPrintf( "Error on send block CRC\r\n");
		  return 1000;
	     }
	  }
  


      if((IspEnvironment->BinaryOffset < ReturnValueLpcRamStart(IspEnvironment)) || 
         (IspEnvironment->BinaryOffset >= ReturnValueLpcRamStart(IspEnvironment)+
		 (LPCtypes[IspEnvironment->DetectedDevice].RAMSize*1024)))
      {
        //Prepare command must be repeated before every write				 
		result = Sector_Prepare(Sector);
		if(result==0) goto Resend; 
	    if(result==-1) return (WRONG_ANSWER_PREP2);
		

        // Round CopyLength up to one of the following values: 512, 1024,
        // 4096, 8192; but do not exceed the maximum copy size (usually 8192, but chip-dependent)
        if(CopyLength < 512)
        {
          CopyLength = 512;
        }
        else if(SectorLength < 1024)
        {
          CopyLength = 1024;
        }
        else if (SectorLength < 4096)
        {
          CopyLength = 4096;
        }
        else
        {
          CopyLength = 8192;
        }

        //----
		if(CopyLength > (unsigned)LPCtypes[IspEnvironment->DetectedDevice].MaxCopySize)
        {
          CopyLength = LPCtypes[IspEnvironment->DetectedDevice].MaxCopySize;
        }

	    //Copy RAM to Flash				
		int cstart = IspEnvironment->BinaryOffset + SectorStart + SectorOffset;
		int cbase = ReturnValueLpcRamBase(IspEnvironment); 
		result = Copy_ToFlash(cstart, cbase, CopyLength);
		if(result !=1) return 1;

        if(IspEnvironment->Verify)
        {

          //Avoid compare first 64 bytes.
          //Because first 64 bytes are re-mapped to flash boot sector,
          //and the compare result may not be correct.
          if (SectorStart + SectorOffset<64)
          {
            sprintf(tmpString, "M %d %ld %ld\r\n", 64, ReturnValueLpcRamBase(IspEnvironment) + 
				   (64 - SectorStart - SectorOffset), CopyLength-(64 - SectorStart - SectorOffset));
          }
          else
          {		
            sprintf(tmpString, "M %ld %ld %ld\r\n", SectorStart + 
				    SectorOffset, ReturnValueLpcRamBase(IspEnvironment), CopyLength);
          }
					
		  result = Sector_Verify(tmpString);
          if(result!=1) 
		  {
            DebugPrintf( "Wrong answer on Compare-Command\r\n");
            //return (WRONG_ANSWER_COPY + GetAndReportErrorNumber(Answer));
          }  
         }
        }
       }

       if ((SectorStart + SectorLength) >= IspEnvironment->BinaryLength && Sector!=0)
        {
            Sector = 0;
            SectorStart = 0;
        }
        else if (Sector == 0)
		{
          break;
        }
        else 
		{
          SectorStart += LPCtypes[IspEnvironment->DetectedDevice].SectorTable[Sector];
          Sector++;
        }
    }

  //int prog_time_min = (tDone.wHour-tStart.wHour)*60 + tStart.wMinute-tDone.wMinute;
  if(IspEnvironment->Verify)
     DebugPrintf("\r\nDownload Finished\r\nVerified correct");
  else
     DebugPrintf("\r\nDownload Finished");

  /*
  if(IspEnvironment->DoNotStart == 0)
  {
    DebugPrintf("\r\nLaunching the new code");
    sprintf(tmpString, "G %ld A\r\n", IspEnvironment->StartAddress);
	BT_Send(tmpString);
  }	
  else DebugPrintf("\r\nPress 'Oven Reset' for Launch the new code");	 */
  
  DebugPrintf("\r\nPress 'Oven Reset' for Launch the new code");
  return 0;
}

/*
Sector 8: Empty
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20   1
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12  CRC OK
Write  OK, Verify  OK
|
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20   2
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12  CRC OK
Write  OK, Verify  OK
|
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20   3
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12  CRC OK
Write  OK, Verify  OK
|
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20   4
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12  CRC OK
Write  OK, Verify  OK
|
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20   5
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12  CRC OK
Write  OK, Verify  OK
|
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20   6
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12  CRC OK
Write  OK, Verify  OK
|
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20   7
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12  CRC OK
Write  OK, Verify  OK
|
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20   8
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 
01 02 03 04 05 06 07 08 09 10 11 12  CRC OK
Write  OK, Verify  Error!
Wrong answer on Compare-Command
ErrorString: OK
*/