//==============================================================================
//File name:    "datatx.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "datatx.h"
#include "protocol-ex.h"

static CCriticalSection TX_CriticalSection;

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------  
CDataTX::CDataTX()
{
  pBLE = NULL;
}
 
CDataTX::~CDataTX()
{
  pBLE = NULL;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------  
void CDataTX::SethBLE(HANDLE hBLE)
{
  pBLE = (CC2540*) hBLE;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------  
int CDataTX::Tx(unsigned char cmd, char *pBuf, int length)
{
  TX_CriticalSection.Lock();

  int result = 0;
  PROTOCOL.FreeMem();
  int block_length = PROTOCOL.CalcBlockLength((unsigned short)length);
  char *pBufOut = (char*) new char[block_length];
  PROTOCOL.CreateDataBlock(pBufOut, pBuf, cmd, (unsigned short)length);

  //If selected BLE channel 
  //Send a data block to the connected BT device
  if(pBLE!=NULL)
	 result = pBLE->DataTx(pBufOut, block_length); 

  delete [] pBufOut;
  pBufOut = NULL;
   
  TX_CriticalSection.Unlock();

  if(result!=block_length) return -1;
  else return 1;
}
