//==============================================================================
//File name:    "datatx.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
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

  int result = 0, tx_block, tx_send = 0;
  PROTOCOL.FreeMem();
  int block_length = PROTOCOL.CalcBlockLength((unsigned short)length);
  char *pBufOut = (char*) new char[block_length];
  PROTOCOL.CreateDataBlock(pBufOut, pBuf, cmd, (unsigned short)length);
  char *pData = pBufOut;
  tx_block = block_length;
  
  /* protocol debug
  char ddd[256];
  memcpy(ddd, pBufOut, block_length);

  TRACE("CMD: {");
  for(int i=0; i<block_length; i++)
  {
   	  unsigned char d = pBufOut[i];
	  TRACE("0x%02X, ", d);
  }
  TRACE("};\r\n"); */ 
 
  //If selected BLE channel 
  if(block_length>=10)
  {
    int tx_length; 

	for(;;)
	{
	  if(block_length>=10) tx_length = 10;
	  else tx_length = block_length;

	  result = pBLE->DataTx(pData, tx_length); 
	  tx_send += result;
	  block_length -= tx_length;
	  pData += tx_length;
 	  
	  Sleep(50);

	  if(block_length==0)
		  break;
	}
  }
  else
  {
   //Send a data block to the connected BT device
   tx_send = pBLE->DataTx(pBufOut, block_length);
  }


  delete [] pBufOut;
  pBufOut = NULL;
   
  TX_CriticalSection.Unlock();

  if(tx_send!=tx_block) return -1;
  else return 1;
}
