//==============================================================================
//File name:   "vcom.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "vcom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "trace.h"
#include "show-err.h"

// ---- WaitCommEvent  Flags
// EV_BREAK Во время ввода данных было обнаружено прерывание. 
// EV_CTS Сигнал готовности к приему (CTS) изменил состояние. 
// EV_DSR Сигнал готовности модема (DSR) изменил состояние. 
// EV_ERR Произошла ошибка состояния линии. 
// Ошибкой состояния линии являются CE_FRAME, CE_OVERRUN и CE_RXPARITY. 
// EV_RING Был обнаружен индикатор вызова. 
// EV_RLSD Сигнал RLSD (детектор принимаемого линейного сигнала) изменил состояние. 
// EV_RXCHAR Символ был принят и помещен в буфер ввода данных. 
// EV_RXFLAG Символ события был принят и помещен в буфер ввода данных. 
// Символ события определяется  в структуре DCB устройства, которое 
// обращается к последовательному порту, используя функцию SetCommState. 
// EV_TXEMPTY Был отправлен последний символ из буфера вывода данных. 
// https://www.silabs.com/documents/public/application-notes/an197.pdf

OVERLAPPED OVR_RX;
OVERLAPPED OVR_TX;

//------------------------------------------------------------------------------
//Thread: Serial port (RS232) Data input to PC
//------------------------------------------------------------------------------ 
DWORD CVCOM_Thread_Rx(LPVOID lParam)
{
  DWORD dwReceived;  
  DWORD dwError;
  DWORD dwEventMask = EV_RXCHAR;  
  
  //Contains information used in asynchronous (or overlapped) input and output (I/O)
  memset(&OVR_RX, 0, sizeof(OVR_RX));
  OVR_RX.hEvent = CreateEvent(NULL, //атрибут защиты
                              FALSE,//тип сброса TRUE - ручной
                              FALSE,//начальное состояние TRUE - сигнальное
                              NULL);//имя обьекта

  CVCOM *pClass = (CVCOM*)lParam;
  CEvent & Q_event = pClass->ev_InpThread_Quit;
  HANDLE Handles[] = {OVR_RX.hEvent, Q_event};

  //===========================================
  //Thread loop
  //===========================================
  for(;;)
  {
    ResetEvent(OVR_RX.hEvent);
      
    //Wait COM port communication events by mask
    if(!WaitCommEvent(pClass->hCOM, &dwEventMask, &OVR_RX))
    {
       //Check exit
       if(WaitForMultipleObjects(2, Handles, FALSE, INFINITE) == 
          WAIT_OBJECT_0+1) 
          break;
    }

    //EV_BREAK, finding interrupt
    if(dwEventMask & EV_BREAK) break;
        
    //EV_RXCHAR Символ был принят и помещен в буфер ввода данных 
    else if(dwEventMask & EV_RXCHAR)
    {
      ClearCommError(pClass->hCOM, &dwError, &pClass->COMST);
      ResetEvent(OVR_RX.hEvent);
         
      //Пришел символ из порта
      GetOverlappedResult(pClass->hCOM, &OVR_RX, &dwReceived, FALSE);
      if(dwReceived>0 && pClass->COMST.cbInQue>0)
      {
        //Читаем блоки из порта  
        pClass->ThreadRx();
      }
    }
  }
  //===========================================
  CloseHandle(OVR_RX.hEvent);
  return 0;
} 

//------------------------------------------------------------------------------
//Thread: Serial port (RS232) Data output from PC
//------------------------------------------------------------------------------ 
DWORD CVCOM_Thread_Tx(LPVOID lParam)
{
  DWORD TxBytes;  
  DWORD dwError, dwResult, dwToWrite, dwWritten;
  DWORD dwEventMask = NULL;//EV_RXCHAR;  
  
  //Contains information used in asynchronous (or overlapped) input and output (I/O)
/*  memset(&OVR_TX, 0, sizeof(OVR_TX));
  OVR_TX.hEvent = CreateEvent(NULL, //атрибут защиты
                              FALSE,//тип сброса TRUE - ручной
                              FALSE,//начальное состояние TRUE - сигнальное
                              NULL);//имя обьекта	 */

  CVCOM *pClass = (CVCOM*)lParam;
  CEvent &Q_event = pClass->ev_OutThread_Quit;
  CEvent &Tx_event = pClass->ev_TxRun;
  HANDLE Handles[] = {OVR_TX.hEvent, Tx_event, Q_event};

  //===========================================
  //Thread loop
  //===========================================
  for(;;)
  {
	
	//Check exit
    dwResult = WaitForMultipleObjects(3, Handles, FALSE, INFINITE);

	switch(dwResult)
	{
	  case WAIT_OBJECT_0:
	  dwResult = GetOverlappedResult(pClass->hCOM, &OVR_TX, &TxBytes, FALSE);
	  pClass->TX_bytes = TxBytes;
	  pClass->ev_TxEnd.SetEvent();
      break;

	  case WAIT_OBJECT_0+1:
	  dwResult = WriteFile(pClass->hCOM, pClass->pBufTX, pClass->TX_length, &dwWritten, &OVR_TX);  
	  break;	

	  case WAIT_OBJECT_0+2:
	  //===========================================
      CloseHandle(OVR_TX.hEvent);
	  //End Tread
      //AfxEndThread(0, TRUE);
      return 0;	
	}
  }

  //===========================================
  CloseHandle(OVR_TX.hEvent);
  //End Tread
  //AfxEndThread(0, TRUE);
  return 0;
} 

//------------------------------------------------------------------------------
//Function:	read com буфер
//------------------------------------------------------------------------------  
int CVCOM::ThreadRx(void)
{
  DWORD dwBytes;
  char *pBuf;
  //Create temporary buffer
  pBuf = new char[COMST.cbInQue];
  
  //---- Read data from driver
  BOOL ret = ReadFile(hCOM, pBuf, COMST.cbInQue, &dwBytes, &OVR_RX);
  
  if(ret==1)
  {
    //If an application has installed its own handler
	//Если приложение установило свой обработчик
    //data block accepted
    if(pRXhandler!=NULL){pRXhandler(pBuf, dwBytes);} 
    //Просто читаем байты в NULL 
    else{TRACE1("\nRead COM: d", dwBytes);}
  }

  //Delete temporary buffer
  //Удалить временный буфер
  delete [] pBuf;
  pBuf = NULL;

  return dwBytes;
}

//------------------------------------------------------------------------------ 
//Constructor 
//------------------------------------------------------------------------------
CVCOM::CVCOM()
{
  opened = 0;
  port_number = 0;
  hCOM = NULL; 
  pRXhandler = NULL;
  pInThread = NULL;
  pOutThread = NULL;
}

//------------------------------------------------------------------------------
//Destructor 
//------------------------------------------------------------------------------
CVCOM::~CVCOM()
{
  if(opened==1) Close();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CVCOM::ParsePortNumber(LPCSTR txt)
{
  CString msg;
  double value;
  int result = 1;
 
  if(strlen(txt)<1) result =-1; 
  
  //read or not, empty 
  if(result<1)
  {
    msg = "Can`t read COM number, enter a digit...";  
    ::AfxMessageBox(msg);
    return -1;
  }	

  //read value in any format
  result = sscanf_s(txt,"%Lf",(double*)&value);
  
  if(result<1)
  {
    msg = "This is not digit!\r\nEnter a number...";  
    ::AfxMessageBox(msg);
    return -1;
  }

  if(value<1 || value>255)
   {
      msg.Format("Value must be\r\nbetween: %d... %d", 1, 255);      
      ::AfxMessageBox(msg);
      return -1;
   }
  
  return (int) value;
}

//------------------------------------------------------------------------------
//Open Serial COM Port
//------------------------------------------------------------------------------
int CVCOM::Open(int PortNumber, long BaudRate)
{
  if(opened==1) return 1;

  //----
  int result;
  CString com_patch, txt, err_txt, com_name, baud;

  //Patch to port
  if(PortNumber<=9) com_patch = "\\";
  else com_patch = "\\\\.\\";
  txt.Format("%d", PortNumber);
  com_name = "COM" + txt;
  com_patch += com_name;

  //Open Serial COM port and get handle	 
  hCOM = CreateFile(com_patch, GENERIC_READ | GENERIC_WRITE, 
                    FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
					FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

  if(hCOM == INVALID_HANDLE_VALUE)
  {
     //Error here, Show error
     std::string err_txt;
     err_txt += __func__;
     err_txt += " COM";
     err_txt += std::to_string((long long)PortNumber);
     err_txt += ": ";
     //GetLastError
     ShowError((LPTSTR)err_txt.c_str());	
     return -1;
  }

  //clear com
  PurgeComm(hCOM,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);

  DCB dcb;
  memset(&dcb, 0, sizeof(DCB));
  dcb.DCBlength = sizeof(DCB);
  result = GetCommState(hCOM, &dcb);
  if(result!=1) return -1;

  //set parameters
  dcb.BaudRate = (DWORD)BaudRate;
  dcb.ByteSize = 8;
  dcb.Parity = NOPARITY;
  dcb.StopBits = ONESTOPBIT;
  dcb.fDsrSensitivity = FALSE;
  dcb.fOutxCtsFlow = FALSE;
  dcb.fDtrControl = DTR_CONTROL_DISABLE;
  dcb.fRtsControl = RTS_CONTROL_DISABLE;	

  dcb.fAbortOnError = FALSE;//TRUE;
  dcb.fBinary = TRUE;
  dcb.fParity = FALSE;
  dcb.fInX = FALSE;
  dcb.fOutX = FALSE;
  dcb.XonChar = 0; 
  dcb.XoffChar = (unsigned char)0xFF;
  dcb.fErrorChar = FALSE;
  dcb.fNull = FALSE;
  dcb.fOutxCtsFlow = FALSE;
  dcb.fOutxDsrFlow = FALSE;
  dcb.XonLim = 128;	//4096
  dcb.XoffLim = 128;//4096
  
  //set DCB
  result = SetCommState(hCOM, &dcb);
  if(result!=1) return -1;

  //show settings  
  DBG_TRACE("\nSerial settings:\n\t----------------\n");
  DBG_TRACE("BaudRate:%ld\n", dcb.BaudRate);
  DBG_TRACE("ByteSize:%d\n", dcb.ByteSize);
  DBG_TRACE("Parity:%d\n", dcb.Parity);
  DBG_TRACE("StopBits:%d\n", dcb.StopBits);
  DBG_TRACE("fOutxCtsFlow:%d\n", dcb.fOutxCtsFlow);
  DBG_TRACE("fRtsControl:%d\n \n", dcb.fRtsControl);
  
  //Monitor the occurrence of port-related events
  //Осуществлять слежение за возникновением связанных с портом событий
  result = SetCommMask(hCOM, EV_RXCHAR );//EV_BREAK | | EV_TXEMPTY
  if(result!=1){return -1;}
  
  //set the size of the receiving and transmitting queues
  //установить размеры очередей приёма и передачи
  //result = SetupComm(hCOM, 1500, 1500);
  //if(result!=1){return -1;}
   /*
  // Timeouts in msec.
  COMMTIMEOUTS CommTimeOuts;
  //таймаут между двумя символами
  CommTimeOuts.ReadIntervalTimeout = 0; 
  //общий таймаут операции чтения
  CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
  //константа для общего таймаута операции чтения
  CommTimeOuts.ReadTotalTimeoutConstant = 0;
  //общий таймаут операции записи
  CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
  //константа для общего таймаута операции записи
  CommTimeOuts.WriteTotalTimeoutConstant = 0;

  result = SetCommTimeouts(hCOM, &CommTimeOuts);
  if(result!=1){Close(); return -1;}
  */

  result = ThreadsLaunch(THREAD_PRIORITY_HIGHEST);
  if(result<0) return -1;

  port_number = PortNumber; 
  opened = 1;

  //initialisation for data exchange
  Clear();
  
  return 1;
}

//------------------------------------------------------------------------------
//Function:	launch communication thrades
//------------------------------------------------------------------------------
//THREAD_PRIORITY_ABOVE_NORMAL   //на один пункт выше нормального
//THREAD_PRIORITY_BELOW_NORMAL   //на один пункт ниже нормального
//THREAD_PRIORITY_HIGHEST        //на два пункта выше нормального
//THREAD_PRIORITY_IDLE           //базовый приоритет равный 1
//THREAD_PRIORITY_LOWEST         //на два пункта ниже нормального
//THREAD_PRIORITY_NORMAL         //нормальный приоритет
//THREAD_PRIORITY_TIME_CRITICAL  //приоритет равный 15
int CVCOM::ThreadsLaunch(int priority)
{
  //Stop, if thread exist 
  ThreadsStop();

  //----
  ev_InpThread_Quit.ResetEvent();
  pInThread = AfxBeginThread((AFX_THREADPROC) CVCOM_Thread_Rx, this, priority);
  if(pInThread==NULL){ return -1;}
  pInThread->m_bAutoDelete = FALSE;

  //----
/*  ev_OutThread_Quit.ResetEvent();
  pOutThread = AfxBeginThread((AFX_THREADPROC) CVCOM_Thread_Tx, this, priority);
  if(pOutThread==NULL){ return -1;}
  pOutThread->m_bAutoDelete = FALSE;   */


  return 1;
}

//------------------------------------------------------------------------------
//Function:	stop communication Thrades
//------------------------------------------------------------------------------
void CVCOM::ThreadsStop(void)
{   
  //Stop Rx
  if(pInThread!=NULL)
  {	
    ev_InpThread_Quit.SetEvent();
    WaitForSingleObject(pInThread->m_hThread, INFINITE);
    delete pInThread;
    pInThread = NULL;
  }

  //Stop Rx
  if(pOutThread!=NULL)
  {	
    ev_OutThread_Quit.SetEvent();
    //WaitForSingleObject(pOutThread->m_hThread, INFINITE);
    //delete pOutThread;
    pOutThread = NULL;
  }
}

//------------------------------------------------------------------------------
//Function:	Place RXhandler function before(above) AddRxHandler call
//------------------------------------------------------------------------------
int CVCOM::AddRxHandler(void(*pHandler)(char*, int))
{
  if(pHandler==NULL) return 0;
  pRXhandler = pHandler;
  return 1;
}

//------------------------------------------------------------------------------
//Function:	Send data without Thread
//------------------------------------------------------------------------------
int CVCOM::WrFile(char *pBuffer, int length)
{
  if(opened!=1) return -1;
  DWORD wrBytes, TxBytes=0, signal;
  BOOL result;
  /*
 
  pBufTX = pBuffer;
  TX_length = length;

  ResetEvent(ev_TxEnd);
  ev_TxRun.PulseEvent();

  //ожидание завершения асихронной операции и получение результата
  signal = WaitForSingleObject(ev_TxEnd.m_hObject, 4000);
  if(signal == WAIT_OBJECT_0)
  {
	return TX_bytes;
  }

  return -1;
*/	

  //создание событие для асинхронной записи
  memset(&OVR_TX, 0, sizeof(OVR_TX));
  //ResetEvent(OVR_TX.hEvent);
  OVR_TX.hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	
  //начало асинхронной записи
  //Write data block to Serial port driver 
  result = WriteFile(hCOM,    //Serial port handle
                     pBuffer, //Data buffer pointer
                     length,  //Data block size 
                     &wrBytes,//Writed bytes number 
                     &OVR_TX);
  if(result == 0)
  {
	if(GetLastError() != ERROR_IO_PENDING) 
	{
       CloseHandle(OVR_TX.hEvent);
	   return -1;
	}
  }

  //ожидание завершения асихронной операции и получение результата
  signal = WaitForSingleObject(OVR_TX.hEvent, INFINITE);
  if(signal == WAIT_OBJECT_0)
  {
	result = GetOverlappedResult(hCOM, &OVR_TX, &TxBytes, FALSE);
  }

  //Закрытие дескриптора сигнального объекта
  CloseHandle(OVR_TX.hEvent);
  		  
  if(result!=TRUE) return -1; 
  else return (int) TxBytes;  
}

//------------------------------------------------------------------------------
//Function:	clear COM port
//------------------------------------------------------------------------------
void CVCOM::Clear(void)
{
  if(hCOM==NULL) return;
  LPDWORD comerr = 0;

  //initialisation for data exchange
  if(hCOM!=NULL) PurgeComm(hCOM, PURGE_RXABORT|PURGE_TXABORT|
                                 PURGE_RXCLEAR|PURGE_TXCLEAR);
  if(hCOM!=NULL) ClearCommError(hCOM, comerr, 0);
  if(hCOM!=NULL) ClearCommBreak(hCOM);
}

//------------------------------------------------------------------------------
//Close COM procedure, 
//Завершение работы с устройством, остановка потока 
//------------------------------------------------------------------------------
void CVCOM::Close(void)
{
  if(opened==0) return;
  
  pRXhandler = NULL;	 
  
  //Delete COM Rx Thread
  ThreadsStop();

  //if interface in use
  if(opened!=0)
  {
	 //Lock access from application
  	 opened = 0;

     if(hCOM!=NULL)
     {
       EscapeCommFunction(hCOM, SETBREAK);
	   
	   //Clear driver Rx and Tx buffers
       Clear();

	   //Close OS driver port
       CloseHandle(hCOM);
	   hCOM = NULL;
     } 
   }
}

//------------------------------------------------------------------------------
//COM List
//------------------------------------------------------------------------------
int CVCOM::PortList(void)
{
  int r = 0;
  HKEY hkey = NULL;

  //Открываем раздел реестра, в котором хранится иинформация о COM портах
  r = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM\\"), 0, KEY_READ, &hkey);
  if (r != ERROR_SUCCESS) return -1;
  
  unsigned long CountValues = 0, MaxValueNameLen = 0, MaxValueLen = 0;
  
  //Получаем информацию об открытом разделе реестра
  RegQueryInfoKey(hkey, NULL, NULL, NULL, NULL, NULL, NULL, &CountValues, &MaxValueNameLen, &MaxValueLen, NULL, NULL);
  
  ++MaxValueNameLen;
  
  //Выделяем память
  TCHAR *bufferName = NULL, *bufferData = NULL;
  
  bufferName = (TCHAR*)malloc(MaxValueNameLen * sizeof(TCHAR));
  if(!bufferName)
  {
    RegCloseKey(hkey);
    return -1;
  }
  
  bufferData = (TCHAR*)malloc((MaxValueLen + 1)*sizeof(TCHAR));
  if(!bufferData) 
  { 
    free(bufferName); 
    RegCloseKey(hkey);
    return -1;
  }
  
  unsigned long NameLen, type, DataLen;
  //Цикл перебора параметров раздела реестра
  for (unsigned int i = 0; i < CountValues; i++)
  {
    NameLen = MaxValueNameLen;
    DataLen = MaxValueLen;
    r = RegEnumValue(hkey, i, bufferName, &NameLen, NULL, &type, (LPBYTE)bufferData, &DataLen);
    if ((r != ERROR_SUCCESS) || (type != REG_SZ))
      continue;    
    
	std::string txt(bufferData);
	ports_list.push_back(txt);
  }
  //Освобождаем память
  free(bufferName);
  free(bufferData);
  //Закрываем раздел реестра
  RegCloseKey(hkey);

  return 1;
}

