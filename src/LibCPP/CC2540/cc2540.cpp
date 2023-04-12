//==============================================================================
//File name:   "cc2540.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "cc2540.h"
#include "trace.h"

CC2540 *pTI;
static CCriticalSection CC2540TX_CS;
//------------------------------------------------------------------------------ 
//Constructor 
//------------------------------------------------------------------------------
CC2540::CC2540()
{
  pTI = this;
  adapter_opened = 0;
  event_filter = 0;
  dev_detected = 0;
  dev_connected = 0;
  dev_name_str = "##";
}

//------------------------------------------------------------------------------
//Destructor 
//------------------------------------------------------------------------------
CC2540::~CC2540()
{
  CMD_Disconnect();
  Close();
}
  
//------------------------------------------------------------------------------
//Function:	Place RXhandler function before(above) AddRxHandler call
//------------------------------------------------------------------------------
int CC2540::AddRxHandler(void(*pHandler)(char*, int))
{
  if(pHandler==NULL) return 0;
  pRXhandler = pHandler;
  return 1;
}

//------------------------------------------------------------------------------
//Function:	Place RXhandler function before(above) AddRxHandler call
//------------------------------------------------------------------------------
void CC2540::RemoveHandlers(void)
{
  pRXhandler = NULL;
  pEventsHandler = NULL;
}

//------------------------------------------------------------------------------
//Function 
//------------------------------------------------------------------------------
int CC2540::SendCMD(char *pCmd, int length, char *pTxt)
{  
  unsigned short OpCode = pCmd[2];
  OpCode <<= 8;
  OpCode |= pCmd[1];
    
  #ifdef _DEBUG
  std::string msg = "PC -> ";
  msg += pTxt;
  msg += "\n";
  DBG_TRACE(msg.c_str());
  #endif

  ev_EventAccepted.ResetEvent();

  int ret = TxCOM(pCmd, length);
  if(ret!=length) {Error(pTxt); return -1;}

  //Wait cmd ACK 
  ev_ResponseAccepted.ResetEvent();
  int RxTimeout = 200;

  long state = WaitForSingleObject(ev_ResponseAccepted.m_hObject, RxTimeout);
  if(state==0)
  {
	return 1;
  }

  return -1;
}

//------------------------------------------------------------------------------
//Function 
//------------------------------------------------------------------------------
int CC2540::CheckCOM(void)
{
  char data[] = {1,1,1,1};
  int find = -1;
  
  if(COM.opened==1)
  {
    int ret = CMD_SupervisionTimeout();
    if(ret!=1){COM.Close(); return -1;}	
	else return 2;
  }	

  Sleep(800);

  //----  
  int result = COM.PortList();  
  if(result!=1) return -1;

  int number = COM.ports_list.size();
  std::string port;
  port = "COM";
  port += std::to_string((long long)COM.port_number);

  for(int i=0; i<number; i++)
  {
	result = COM.ports_list.at(i).find(port);
	if(result>=0){ find = 1; break;}
  }

  return find;
}

//------------------------------------------------------------------------------
//Function 
//------------------------------------------------------------------------------
int CC2540::TxCOM(char *pBuf, int length)
{ 
  CC2540TX_CS.Lock();
  int send = COM.WrFile(pBuf, length);
  CC2540TX_CS.Unlock();

  return send;
}

void DeviceInformation(char *pBuf, int length);

//------------------------------------------------------------------------------
//Function:	COM Rx Handler
//------------------------------------------------------------------------------
void CC2540::RxCOM(char *pBuf, int length)
{
  if(pTI == NULL) return;
	
  /*		
  //protocol debug
  for(int i=0; i<length; i++)
  {
    CString txt;  
    txt.Format("%0.2X ", (unsigned char)pBuf[i]);
    DBG_TRACE(txt);
  }
 
  DBG_TRACE("\n"); 
   */

  short cmd;
  int data_length;
  int rx_event = pTI->HCI_Decoder(pBuf, length, cmd, data_length);
  
  if(rx_event==3)
  {
    //---- ≈сли приложение установило свой обработчик
    //data block accepted
    if(pTI->pRXhandler!=NULL){pTI->pRXhandler((char*)pTI->data, data_length);}
  }

  //scan data
  if(rx_event==2)
  {	 
	//scan completed
	if(cmd==0x0106)
	{ 
	  //GAP_DeviceInformation
	  if(pTI->pEventsHandler!=NULL)
	    {pTI->pEventsHandler(BT_DEV_INFO,(char*)"Scan Done", 4);}
	}

	if(cmd==0x7f06)
	{
	  if(pTI->pRXhandler!=NULL)
	    {pTI->pRXhandler((char*)pTI->data, length);}	
	}
  }
}

//------------------------------------------------------------------------------
/*Type           : 0x04 (Event)
-EventCode      : 0x00FF (Event)
-Data Length    : 0x17 (23) bytes(s)
 Event          : 0x060D (1549) (GAP_DeviceInformation)
 Status         : 0x00 (0) (Success)
 EventType      : 0x00 (0) (Connectable Undirect Advertisement)
 AddrType       : 0x01 (1) (Static)
 Addr           : C6:04:9C:27:9A:BD
 Rssi           : 0xC1 (193)
 DataLength     : 0x0A (10)
 Data           : 02:01:06:06:09:54:2D:39:36:32
Dump(Rx):
0000:04 FF 17 0D 06 00 00 01 BD 9A 27 9C 04 C6 C1 0A ..........
0010:02 01 06 06 09 54 2D 39 36 32                   .....T-962	*/
//------------------------------------------------------------------------------
void CC2540::DeviceInformation(char *pBuf, int length)
{
 /*
 char ddd[]	= {0x04, 0xFF, 0x17, 0x0D, 0x06, 0x00, 0x00, 0x01, 0xBD, 0x9A, 
	 0x27, 0x9C, 0x04, 0xC6, 0xC1, 0x0A, 0x02, 0x01, 0x06, 0x06, 0x09, 0x54, 0x2D, 0x39, 0x36, 0x32}; 
  pBuf = ddd;	*/
 
 char Status = pBuf[5];     // 0x00 (0) (Success)
 char EventType = pBuf[6];  // 0x00 (0) (Connectable Undirect Advertisement)
 char AddrType = pBuf[7];   // 0x01 (1) (Static)
 char Addr[6];              // C6:04:9C:27:9A:BD
 memcpy(Addr, &pBuf[8], 6);
 char Rssi = pBuf[14];      // 0xC1 (193)
 char DataLength = pBuf[15];// 0x0A (10)

 char dataa[128];
 memcpy(dataa, &pBuf[16], DataLength);
 
 std::string txt(dataa);

 int position = txt.find(dev_name_str);
 
 if(position>=0)
 {
   dev_rssi = Rssi;
   memcpy(&dev_address, Addr, 6);
   dev_detected = 1;
	  
   if(pEventsHandler!=NULL){pEventsHandler(BT_DEV_INFO,(char*)NULL, 0);}
 }
}

//------------------------------------------------------------------------------
//Function 
//------------------------------------------------------------------------------
int CC2540::SetAddress(char *pAddr)
{
  dev_bd_addr_str = pAddr;
  int ret = BD_StringToAddr(&dev_bd_addr_str, (char*)dev_address);
  
  if(ret!=1) Error("Address format wrong!\r\nExample: C6:04:9C:27:9A:BD");
  return ret;
}

//------------------------------------------------------------------------------
//Function 
//------------------------------------------------------------------------------
int CC2540::Open(int com_port)
{ 
  //Check for serial port name 
  int result = COM.Open(com_port, 115200);
  
  //Can`t open port
  if(result!=1) return -1;

  //Add handler
  COM.AddRxHandler(RxCOM);

  //If BT dongle already was connected 
  dev_connected = 1;
  CMD_Disconnect();

  //HCI cmd 
  result = CMD_DeviceInit();
  if(result!=1){Error("BT adapter not found\r\nSelect the right COM"); return -1;}

  result = CMD_MinConnectionInterval();
  if(result!=1) return -1;
 
  result = CMD_MaxConnectionInterval();
  if(result!=1) return -1;
   
  result = CMD_SlaveLatency();
  if(result!=1) return -1;

  result = CMD_SupervisionTimeout();
  if(result!=1) return -1;

  adapter_opened = 1;
  
  return 1;  
}

//------------------------------------------------------------------------------
//Function 
//------------------------------------------------------------------------------
int CC2540::Connect(void)
{ 
/*dev_address[0] = 0xBD;
  dev_address[1] = 0x9A;
  dev_address[2] = 0x27;
  dev_address[3] = 0x9C;
  dev_address[4] = 0x04;
  dev_address[5] = 0xC6;*/

  int result = BD_StringToAddr(&dev_bd_addr_str, (char*)dev_address);
  if(result!=1) {return -1;}

  result = CMD_Connect((char*)dev_address);
  if(result!=1) 
  {
	CMD_ConnectCancel();
	return -1;
  }
  
  Sleep(400); 
  result = CMD_NotifyEnable();
  if(result!=1) return -1;

  return 1;  
}

//------------------------------------------------------------------------------
//Function 
//------------------------------------------------------------------------------   
void CC2540::Close(void)
{ 
  CMD_Disconnect();
  
  //Close current COM port
  COM.Close();
}

//------------------------------------------------------------------------------
//Function 
//------------------------------------------------------------------------------
int CC2540::Wait_Event(short EvCode, int timeout) 
{		
  //ev_EventAccepted.ResetEvent();

  long state = WaitForSingleObject(ev_EventAccepted.m_hObject, timeout);
  if(state==0)
  {
	if(EvCode==l_opcode) return 1;
	else return -1;
  }

  return -1;
}

//------------------------------------------------------------------------------
//Function 
//------------------------------------------------------------------------------ 
inline void CC2540::Error(char *pErrTxt) 
{
  std::string msg, err;
  msg  = pErrTxt;
  msg += "\n";
  
  ::MessageBox(NULL, msg.c_str(), "CC2540 Error", 
               MB_ICONERROR|MB_TASKMODAL);
}

//------------------------------------------------------------------------------
//Function:
/*-Type         : 0x01 (Command)
-OpCode         : 0xFD92 (GATT_WriteCharValue)
-Data Length    : 0x08 (8) byte(s)
 ConnHandle     : 0x0000 (0)
 Handle         : 0x000E (14)
 Value          : 33:44:55:66
Dump(Tx):
0000:01 92 FD 0A 00 00 0E 00 0F AC B3 00 00 91  */
//------------------------------------------------------------------------------
int CC2540::DataTx(char *pBuf, int length)
{
  if(dev_connected !=1) return -1;
  
  char cmd[30];
  cmd[0] = 0x01; //Command
  cmd[1] = 0x92; //OpCode
  cmd[2] = 0xFD; //OpCode
  cmd[3] = 4 + length; //Data Length 
  cmd[4] = 0x00; //ConnHandle
  cmd[5] = 0x00; //ConnHandle
  cmd[6] = 0x0E; //Nordic UART Handle 
  cmd[7] = 0x00; //?
  memcpy(&cmd[8], pBuf, length);

  #ifdef _DEBUG
  std::string msg = "TX -> ";
  msg += __func__;
  msg += "\n";
  DBG_TRACE(msg.c_str());
  #endif

  CC2540TX_CS.Lock();
  int send = COM.WrFile(cmd, 8+length);
  CC2540TX_CS.Unlock();

  send -= 8;

  return send;
}

//------------------------------------------------------------------------------
//Function 
//------------------------------------------------------------------------------
int find_block(char *pBuf, int length, int &position, short &cmd)
{
  unsigned char byte;
  unsigned char rx_data[8];
  int block_length, data_length=0;
  memset(rx_data, 0, sizeof(rx_data));
  cmd = 0;
  position = 0;
  
  for(int i=0; i<length; i++)
  {

    //====== protocol parser =====
    //04 FF 0E 1B 05   00 00 00 08   0B 00 69 6F 6E 3A 20 58      
    rx_data[0] = rx_data[1]; //Header, Protocol start byte 1 
    rx_data[1] = rx_data[2]; //Header, Protocol start byte 2
    rx_data[2] = rx_data[3]; //Data length
    rx_data[3] = rx_data[4]; //Command code 
    rx_data[4] = *pBuf++;    //Command code  

    if(rx_data[0]== 0x04 &&//Protocol start byte 1  
       rx_data[1]== 0xFF)  //Protocol start byte 2
    {
	   block_length = rx_data[2];
	   cmd = rx_data[3];
       cmd <<= 8; 
       cmd |= rx_data[4];
	   position = i-4;
	   return block_length;
    }
  }  
  
  return -1;
}

//------------------------------------------------------------------------------
int CC2540::HCI_Decoder(char *pBuf, int length, short &cmd, int &data_length)
{
  unsigned short opcode;
  unsigned char  byte, status;
  unsigned char  rx_data[8];
  int block_length, datalength=0, position, i=0,n=0, total=0, ret=0, next=0;
  memset(rx_data, 0, sizeof(rx_data));
  cmd = 0;
  memset(data, 0, 256);

  for(;;)
  {
   datalength = find_block(&pBuf[i], length, position, cmd);
   block_length = datalength + 3;

   memset(block, 0, 1024);
   memcpy(block, &pBuf[i], block_length);

   /*
   //Debug protocol
   for(int i=0; i<block_length; i++)
   {
     CString txt;  
     txt.Format("%0.2X ", (unsigned char)block[i]);
     DBG_TRACE(txt);
   }
 
   DBG_TRACE("\n"); 
   */
   
   if((datalength+3)>=length) next = 0; 
   else next = 1; 
   length -= datalength+3;
   i += datalength + 3;

   if(datalength>0)
   {
	  switch(cmd)
	  {
		//===== Data block
	    case 0x1B05:
		memcpy(&data[n], &block[position+11], datalength-8);
		n += datalength-8;
		data_length = n;
		ret = 3;
		break;

		//===== CMD Ack
	    default:
	    /*Type         : 0x04 (Event)
        -EventCode     : 0x00FF (Event)
        -Data Length   : 0x06 (6) bytes(s)
        Event          : 0x067F (1663) (GAP_HCI_ExtentionCommandStatus)
        Status         : 0x00 (0) (Success)
        OpCode         : 0xFE00 (GAP_DeviceInit)
        DataLength     : 0x00 (0) */
		//04 FF 06 7F 06 00 00 FE 00
		opcode =  block[4];
		opcode <<=8;
		opcode |= block[3];
		status =  block[5];
		
		l_opcode = opcode;
        l_status = status;
		
		if(opcode==0x060d)
		{
		  DeviceInformation((char*)block, datalength);
		  break;
		}

		ret = 2;
		if(event_filter==opcode) ev_EventAccepted.SetEvent();
		if(event_filter==1) ev_EventAccepted.SetEvent();
		break;
	  }

	//проверка наличи€ следующего блока
	//check if the next block is present
	if(next==0) 
		return ret;  
   }
   else 	   
   {
	 return n;
   }
 }

 return -1;
}
