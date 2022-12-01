//==============================================================================
//File name:   "datatx.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2021, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _DATA_TX_H_
#define _DATA_TX_H_

#include "protocol-ex.h"
 
#include "cc2540.h"

#pragma once

class CDataTX
{
  public:
  CDataTX::CDataTX();
  CDataTX::~CDataTX();

  void SethBLE(HANDLE hBLE);
  int Tx(unsigned char cmd, char *pBuf, int length);

  ///CBTDONGLE *pBLE;
  CC2540 *pBLE;
  
  CProtocolEx PROTOCOL;
  //static CCriticalSection TX_CriticalSection;
};

#endif