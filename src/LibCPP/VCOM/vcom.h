//==============================================================================
//File name:   "vcom.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _CVCOM_H_
#define _CVCOM_H_

#include "stdafx.h"
#include <vector> 

#ifdef  __cplusplus
extern "C" 
{
#endif //end __cplusplus

#pragma once
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CVCOM
{
  //Construction
  public:
  CVCOM();
 ~CVCOM();

  //Operations
  public:
  int opened;	 //COM status: open/close
  int port_number;//Opened COM number 
  std::vector <std::string> ports_list;

  HANDLE hCOM;
  COMSTAT COMST;
  CWinThread *pInThread;   //Thread pointers
  CEvent ev_InpThread_Quit;//Thread Events
  CWinThread *pOutThread;  //Thread pointers
  CEvent ev_OutThread_Quit;//Thread Events
  CEvent ev_TxRun;
  CEvent ev_TxEnd;

  //----
  int ParsePortNumber(LPCSTR txt);
  int Open(int PortNumber, long BaudRate);
  int WrFile(char *pBuffer, int length);
  int RdFile(char *pBuffer, int length);
  int PortList(void);

  void Clear(void);
  void Close(void);
  		  
  //----
  int  ReadDevice(char *pRxBuf,
                  int Rx_length,
                  char *pTxBuf,
                  int Tx_length,
                  int timeout_ms);
  LPCVOID pBufTX;
  DWORD   TX_length;
  DWORD   TX_bytes;
  //--------------------------------------------------------
  //Data handler function pointer
  //указатель на функцию обработчик  
  void (*pRXhandler)(char*, int);

  //Set callback for Data Rx handler
  //Place RXhandler function before(above) AddRxHandler call
  int AddRxHandler(void(*pHandler)(char*, int));
  int ThreadRx(void);

  protected:
  //int handler;
  int ThreadsLaunch(int priority);
  void ThreadsStop(void);

};

#ifdef __cplusplus
}
#endif //end __cplusplus

#endif
