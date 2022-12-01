//==============================================================================
//File name:   "cc2540.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _CC2540_H_
#define _CC2540_H_

#include <string>   //для string
#include <vector>   //для vector
#include "vcom.h"

#define  __func__	__FUNCTION__
#define  __to_pc	"PC <- "
#define  __from_pc  "PC -> "

#define BT_DEV_ARRIVED      4
#define BT_DEV_EXIT         5
#define BT_DEV_CONNECTED    6
#define BT_DEV_DISCONNECTED 7
#define BT_DEV_CLOSE_CONN   8
#define BT_DEV_INFO         9
#define BT_DEV_SCAN_DONE    10

#pragma once

#ifdef  __cplusplus
extern "C" 
{
#endif //end __cplusplus

//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CC2540
{
  //Construction
  public:

  CC2540();
 ~CC2540();
   
  CVCOM COM;

  //Operations
  public:
  int  Open(int com_port);
  int  Scan(void); 
  int  Connect(void); 
  void Close(void);
  int  SetAddress(char *pAddr);

  //HCI commands
  int  CMD_Scan(void);
  int  CMD_ScanCancel(void);
  int  CMD_Connect(char *pAddr);
  int  CMD_ConnectCancel(void);
  int  CMD_ReadAttr(void);
  int  CMD_DeviceInit(void);
  int  CMD_Disconnect(void);
  int  CMD_NotifyEnable(void);
  int  CMD_SlaveLatency(void);
  int  CMD_SupervisionTimeout(void);
  int  CMD_MinConnectionInterval(void);
  int  CMD_MaxConnectionInterval(void);
  
  int  DataTx(char *pBuf, int length);
  int  DataRx(char *pBuf);
  int  HCI_Decoder(char *pBuf, int length, short &cmd, int &data_length);
  int  Decoder(char *pBuf, int length, short &cmd);
  void DeviceInformation(char *pBuf, int length);

  //Data BT Rx handler function pointer
  //указатель на функцию обработчик  
  void (*pRXhandler)(char*, int);
  //Set callback for Data BT Rx handler
  //Place RXhandler function before(above) AddRxHandler call
  int AddRxHandler(void(*pHandler)(char*, int));

  //Application BT Events handler, function pointer
  //Must be the 'static void' defenition!
  void (*pEventsHandler)(int, char*, int);
  //Place BTevents function before(above) AddBTeventsHandler call
  int  AddEventsHandler(void(*pEventsHandler)(int, char*, int));

  int  AddFilter_DeviceName(char *pDeviceName);
  void RemoveHandlers(void);
  void BD_AddrToString(char *pAddrArr, std::string *pTxt);
  int  BD_StringToAddr(std::string *pTxt, char *pAddrArr);
  void ReverseArray(char *pArray, int len);

  protected:
  CEvent ev_ResponseAccepted;
  CEvent ev_EventAccepted;

  int Wait_Event(short EvCode, int timeout);
  int SendCMD(char *pCmd, int length, char *pTxt);
  //VCOM port functions
  int TxCOM(char* pData, int length); 
  static void RxCOM(char *pBuf, int length);
  inline void Error(char *pErrTxt);
  int ack(char *pBuf, short &opcode);
  

  public:
  //connected to remote device
  int dev_connected;
  //adapter opened
  int adapter_opened;

  //Bluetooth device
  std::string dev_name_str;
  std::string dev_bd_addr_str;
  //for scan
  char dev_detected;
  //signal level
  signed char dev_rssi;
  //Target MAC address xx:xx:xx:xx:xx:xx
  unsigned char dev_address[6];
  //data exchange buffer
  unsigned char data[512];

  unsigned short l_opcode;
  unsigned char  l_status;
  unsigned short event_filter;
  unsigned char block[1024];

};

#ifdef __cplusplus
}
#endif //end __cplusplus

#endif

