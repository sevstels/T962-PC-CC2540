//==============================================================================
//File name:   "cc2540.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "cc2540.h"
#include "trace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Use "BTool" application for command description

//------------------------------------------------------------------------------
//Function
/*
-Type           : 0x01 (Command)
-OpCode         : 0xFE80 (UTIL_Reset)
-Data Length    : 0x01 (1) byte(s)
 ResetType      : 0x00 (0) (Hard Reset)
Dump(Tx):
0000:01 80 FE 01 00
-Type           : 0x04 (Event)
-EventCode      : 0x00FF (Event)
-Data Length    : 0x06 (6) bytes(s)
 Event          : 0x067F (1663) (GAP_HCI_ExtentionCommandStatus)
 Status         : 0x01 (1) (Failure)
 OpCode         : 0xFE80 (UTIL_Reset)
 DataLength     : 0x00 (0)
Dump(Rx):
0000:04 FF 06 7F 06 01 80 FE 00 */
//------------------------------------------------------------------------------
int CC2540::CMD_DeviceReset(void)
{
  unsigned char cmd[] = {0x01, 0x80, 0xFE, 0x01, 0x00};

  event_filter = 0x067F;

  SendCMD((char*)cmd, sizeof(cmd), __func__);
  
  return Wait_Event(0x067F, 2000);
}


//------------------------------------------------------------------------------
//Function
/*
-Type           : 0x01 (Command)
-OpCode         : 0xFE00 (GAP_DeviceInit)
-Data Length    : 0x26 (38) byte(s)
 ProfileRole    : 0x08 (8) (Central)
 MaxScanRsps    : 0x19 (25)
 IRK            : 00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00
 CSRK           : 00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00
 SignCounter    : 0x00000001 (1)
Dump(Tx):
0000:01 00 FE 26 08 19 00 00 00 00 00 00 00 00 00 00 
0010:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
0020:00 00 00 00 00 00 01 00 00 00*/   
//------------------------------------------------------------------------------
int CC2540::CMD_DeviceInit(void)
{
  unsigned 
  char cmd[] = {0x01, 0x00, 0xFE, 0x26, 0x08, 0x19, 0x00, 0x00, 
		        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   	        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
				0x00, 0x00};

  event_filter = 0x0600;

  SendCMD((char*)cmd, sizeof(cmd), __func__);
  
  return Wait_Event(0x0600, 2000); //GAP_DeviceInitDone
}

//------------------------------------------------------------------------------
//Function
/*
-Type           : 0x01 (Command)
-OpCode         : 0xFE31 (GAP_GetParam)
-Data Length    : 0x01 (1) byte(s)
 ParamID        : 0x15 (21) (Minimum Link Layer Connection Interval, 
                  When Using Connection Establishment 
                  Proc (mSec). TGAP_CONN_EST_INT_MIN)
Dump(Tx):
0000:01 31 FE 01 15 */
//------------------------------------------------------------------------------
int CC2540::CMD_MinConnectionInterval(void)
{
  unsigned char cmd[] = {0x01, 0x31, 0xFE, 0x01, 0x15};
  event_filter = 0x067F;
  SendCMD((char*)cmd, sizeof(cmd), __func__);
  int ret = Wait_Event(0x067F, 200); 
  return ret; 
}

//------------------------------------------------------------------------------
//Function
/*
-Type           : 0x01 (Command)
-OpCode         : 0xFE31 (GAP_GetParam)
-Data Length    : 0x01 (1) byte(s)
 ParamID        : 0x16 (22) (Maximum Link Layer Connection Interval, 
                  When Using Connection Establishment 
                  Proc (mSec). TGAP_CONN_EST_INT_MAX)
Dump(Tx):
0000:01 31 FE 01 16 */
//------------------------------------------------------------------------------
int CC2540::CMD_MaxConnectionInterval(void)
{
  unsigned char cmd[] = {0x01, 0x31, 0xFE, 0x01, 0x16};
  event_filter = 0x067F;
  SendCMD((char*)cmd, sizeof(cmd), __func__);
  int ret = Wait_Event(0x067F, 200); 
  return ret; 
}

//------------------------------------------------------------------------------
//Function
/*
-Type           : 0x01 (Command)
-OpCode         : 0xFE31 (GAP_GetParam)
-Data Length    : 0x01 (1) byte(s)
 ParamID        : 0x1A (26) (Link Layer Connection Slave Latency, When Using 
                  Connection Establishment Proc (mSec) TGAP_CONN_EST_LATENCY)
Dump(Tx):
0000:01 31 FE 01 1A */
//------------------------------------------------------------------------------
int CC2540::CMD_SlaveLatency(void)
{
  unsigned char cmd[] = {0x01, 0x31, 0xFE, 0x01, 0x1A};
  event_filter = 0x067F;
  SendCMD((char*)cmd, sizeof(cmd), __func__);
  int ret = Wait_Event(0x067F, 200); 
  return ret; 
}

//------------------------------------------------------------------------------
//Function
/*
-Type           : 0x01 (Command)
-OpCode         : 0xFE31 (GAP_GetParam)
-Data Length    : 0x01 (1) byte(s)
 ParamID        : 0x19 (25) (Link Layer Connection Supervision Timeout, 
                  When Using Connection Establishment 
                  Proc (mSec). TGAP_CONN_EST_SUPERV_TIMEOUT)
Dump(Tx):
0000:01 31 FE 01 19 */                                 
//------------------------------------------------------------------------------
int CC2540::CMD_SupervisionTimeout(void)
{
  unsigned char cmd[] = {0x01, 0x31, 0xFE, 0x01, 0x19};
  event_filter = 0x067F;
  SendCMD((char*)cmd, sizeof(cmd), __func__);
  int ret = Wait_Event(0x067F, 200); 
  return ret; 
}

//------------------------------------------------------------------------------
//Function 	Find remote devices
//------------------------------------------------------------------------------
/*
-Type           : 0x01 (Command)
-OpCode         : 0xFE04 (GAP_DeviceDiscoveryRequest)
-Data Length    : 0x03 (3) byte(s)
 Mode           : 0x01 (1) (General)
 ActiveScan     : 0x01 (1) (Enable)
 WhiteList      : 0x00 (0) (Disable)
Dump(Tx):
0000:01 04 FE 03 01 01 00 */
//------------------------------------------------------------------------------
int CC2540::CMD_Scan(void)
{
  unsigned char cmd[] = {0x01, 0x04, 0xFE, 0x03, 0x03, 0x01, 0x00};
  
  dev_detected = 0;
  int ret = SendCMD((char*)cmd, sizeof(cmd), __func__);

  return ret;
}

//------------------------------------------------------------------------------
//Function 
//------------------------------------------------------------------------------
/*
-Type           : 0x01 (Command)
-OpCode         : 0xFE05 (GAP_DeviceDiscoveryCancel)
-Data Length    : 0x00 (0) byte(s)
Dump(Tx):
0000:01 05 FE 00 */
//------------------------------------------------------------------------------
int CC2540::CMD_ScanCancel(void)
{
  unsigned char cmd[] = {0x01, 0x05, 0xFE, 0x00};
  event_filter = 0x0601;
  SendCMD((char*)cmd, sizeof(cmd), __func__);
  int ret = Wait_Event(0x0601, 200); 
  return ret;
}

//------------------------------------------------------------------------------
/*
-Type           : 0x04 (Event)
-EventCode      : 0x00FF (Event)
-Data Length    : 0x2C (44) bytes(s)
 Event          : 0x0601 (1537) (GAP_DeviceDiscoveryDone)
 Status         : 0x00 (0) (Success)
 NumDevs        : 0x05 (5)
 Device #0
 EventType      : 0x00 (0) (Connectable Undirect Advertisement)
 AddrType       : 0x01 (1) (Static)
 Addr           : D8:F3:93:7B:6A:09
 Device #1
 EventType      : 0x00 (0) (Connectable Undirect Advertisement)
 AddrType       : 0x01 (1) (Static)
 Addr           : D2:84:28:66:73:9A
 Device #2
 EventType      : 0x00 (0) (Connectable Undirect Advertisement)
 AddrType       : 0x01 (1) (Static)
 Addr           : D3:BE:4D:D4:DE:D9
 Device #3
 EventType      : 0x00 (0) (Connectable Undirect Advertisement)
 AddrType       : 0x01 (1) (Static)
 Addr           : C6:04:9C:27:9A:BD
 Device #4
 EventType      : 0x00 (0) (Connectable Undirect Advertisement)
 AddrType       : 0x03 (3) (PrivateResolve)
 Addr           : 46:55:42:C3:13:D9
Dump(Rx):
0000:04 FF 2C 01 06 00 05 00 01 09 6A 7B 93 F3 D8 00 ..,.......j{....
0010:01 9A 73 66 28 84 D2 00 01 D9 DE D4 4D BE D3 00 ..sf(.......M...
0020:01 BD 9A 27 9C 04 C6 00 03 D9 13 C3 42 55 46    ...'........BUF  */	



//------------------------------------------------------------------------------
/*
-Type           : 0x01 (Command)
-OpCode         : 0xFE0A (GAP_TerminateLinkRequest)
-Data Length    : 0x03 (3) byte(s)
 ConnHandle     : 0xFFFE (65534)
 DiscReason     : 0x13 (19) (Remote User Terminated Connection)
Dump(Tx):
0000:01 0A FE 03 FE FF 13 */
//------------------------------------------------------------------------------ 
int CC2540::CMD_ConnectCancel(void)
{
  unsigned char cmd[] = {0x01, 0x0A, 0xFE, 0x03, 0xFE, 0xFF, 0x13};
  event_filter = 0x0605;
  SendCMD((char*)cmd, sizeof(cmd), __func__);

  int ret = Wait_Event(0x0605, 2000); 
  return ret;
}

//------------------------------------------------------------------------------
//Function Enable Notify, Data Rx from remote device
/*
-Type           : 0x01 (Command)
-OpCode         : 0xFD8A (GATT_ReadCharValue)
-Data Length    : 0x04 (4) byte(s)
 ConnHandle     : 0x0000 (0)
 Handle         : 0x0003 (3)
Dump(Tx):
0000:01 8A FD 04 00 00 03 00 */                        
//------------------------------------------------------------------------------
//Attribute value read, handle: 0x03, value (0x): 54-2D-39-36-32 	
int CC2540::CMD_ReadAttr(void)
{
  unsigned char cmd[] = {0x01, 0x8A, 0xFD, 0x04, 0x00, 0x00, 0x03, 0x00};
  return SendCMD((char*)cmd, sizeof(cmd), __func__);
}

//------------------------------------------------------------------------------
//Function Enable Notify, Data Rx from remote device
//------------------------------------------------------------------------------
//https://www.silabs.com/documents/public/user-guides/UG208-BLEGUI-Application-User-Guide.pdf
/* page 23 Notify
When a characteristic is configured to be notified, a server will 
automatically notify the characteristic value changes to the client.
To start the notifications the client needs to write 0x01 into that 
characteristic. After this is doneconfiguration the server will 
automatically notify the characteristic value changes.
-Type           : 0x01 (Command)
-OpCode         : 0xFD92 (GATT_WriteCharValue)
-Data Length    : 0x06 (6) byte(s)
 ConnHandle     : 0x0000 (0)
 Handle         : 0x000C (12)
 Value          : 01:00
Dump(Tx):
0000:01 92 FD 06 00 00 0C 00 01 00 */
int CC2540::CMD_NotifyEnable(void)
{
  unsigned char cmd[] = {0x01, 0x92, 0xFD, 0x06, 0x00, 0x00, 0x0C, 0x00, 0x01, 0x00};
  event_filter = 0x0513; 
  SendCMD((char*)cmd, sizeof(cmd), __func__);

  int ret = Wait_Event(0x0513, 4000); 
  return ret;
}

//------------------------------------------------------------------------------
//Function 
/*
-Type           : 0x01 (Command)
-OpCode         : 0xFE09 (GAP_EstablishLinkRequest)
-Data Length    : 0x09 (9) byte(s)
 HighDutyCycle  : 0x00 (0) (Disable)
 WhiteList      : 0x00 (0) (Disable)
 AddrTypePeer   : 0x01 (1) (Static)
 PeerAddr       : C6:04:9C:27:9A:BD
Dump(Tx):
0000:01 09 FE 09 00 00 01 BD 9A 27 9C 04 C6 */
//------------------------------------------------------------------------------   
int CC2540::CMD_Connect(char *pAddr)
{
  DBG_TRACE("==== Connect to target ====\n");
  unsigned 
  char cmd[] = {0x01, 0x09, 0xFE, 0x09, 0x00, 0x00, 0x01,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	            //0xBD, 0x9A, 0x27, 0x9C, 0x04, 0xC6};

  if(pAddr==NULL) memcpy(&cmd[7], dev_address, 6);
  else memcpy(&cmd[7], pAddr, 6);
   
  BD_AddrToString((char*)&cmd[7], &dev_bd_addr_str); 

  event_filter = 0x0605; 
  SendCMD((char*)cmd, sizeof(cmd), __func__);

  int result = Wait_Event(0x0605, 4000); 
  if(result!=1) return -1;

  dev_connected = 1;

  //≈сли приложение установило свой обработчик
  //послать сообщение приложению
  if(pEventsHandler!=NULL)
  {
	if(result) pEventsHandler(BT_DEV_CONNECTED, NULL, 0);
  } 

  return result;
}
  
//------------------------------------------------------------------------------
//Function 
//------------------------------------------------------------------------------
/*-Type           : 0x01 (Command)
-OpCode         : 0xFE0A (GAP_TerminateLinkRequest)
-Data Length    : 0x03 (3) byte(s)
 ConnHandle     : 0x0000 (0)
 DiscReason     : 0x13 (19) (Remote User Terminated Connection)
Dump(Tx):
0000:01 0A FE 03 00 00 13   */
int CC2540::CMD_Disconnect(void)
{   
  // ThreadRx.Delete();
  DBG_TRACE("==== Disconnect from target ====\n");

  //Disconnect
  unsigned char cmd[] = {0x01, 0x0A, 0xFE, 0x03, 0x00, 0x00, 0x13};
   
  SendCMD((char*)cmd, sizeof(cmd), __func__);
  
  
  //if(result!=1) return -1;
  int result = 1;
  dev_connected = 0;

  //≈сли приложение установило свой обработчик
  //послать сообщение приложению
  if(pEventsHandler!=NULL)
  {
	if(result) pEventsHandler(BT_DEV_DISCONNECTED, NULL, 0);
  } 

  return result;
}

//------------------------------------------------------------------------------
//Function
/*Type           : 0x01 (Command)
-OpCode         : 0xFC01 (HCIExt_SetTxPower)
-Data Length    : 0x01 (1) byte(s)
 Tx Power       : 0x02 (2) (HCI_EXT_TX_POWER_0_DBM)
Dump(Tx):
0000:01 01 FC 01 02 */
//------------------------------------------------------------------------------
int CC2540::CMD_SetTxPower(char level)
{
  unsigned char cmd[] = {0x01, 0x01, 0xFC, 0x01, 0x02};
  cmd[4] = level;

  event_filter = 0x0401; 
  SendCMD((char*)cmd, sizeof(cmd), __func__);

  int ret = Wait_Event(0x0401, 200); 
  return ret;
}

//------------------------------------------------------------------------------
//Function
/*Type          : 0x01 (Command)
-OpCode         : 0xFC00 (HCIExt_SetRxGain)
-Data Length    : 0x01 (1) byte(s)
 Rx Gain        : 0x00 (0) (HCI_EXT_RX_GAIN_STD)
Dump(Tx):
0000:01 00 FC 01 00  */
//------------------------------------------------------------------------------ 
int CC2540::CMD_SetRxGain(char level)
{
  unsigned char cmd[] = {0x01, 0x00, 0xFC, 0x01, 0x00};
  cmd[4] = level;

  event_filter = 0x0400; 
  SendCMD((char*)cmd, sizeof(cmd), __func__);

  int ret = Wait_Event(0x0400, 200); 
  return ret;
}

//------------------------------------------------------------------------------
//Function
/*Type          : 0x01 (Command)
-OpCode         : 0x1405 (HCI_ReadRSSI)
-Data Length    : 0x02 (2) byte(s)
 ConnHandle     : 0xFFFE (65534)
Dump(Tx):
0000:01 05 14 02 FE FF  */
//------------------------------------------------------------------------------ 
int CC2540::CMD_ReadRSSI(void)
{
  unsigned char cmd[] = {0x01, 0x05, 0x14, 0x02, 0xFE, 0xFF};

  event_filter = 0x1009; 
  SendCMD((char*)cmd, sizeof(cmd), __func__);

  int ret = Wait_Event(0x1009, 200); 
  return ret;
}
