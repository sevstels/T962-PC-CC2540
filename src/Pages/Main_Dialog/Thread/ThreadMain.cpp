#include "stdafx.h"
#include "MainDlg.h"
#include "cmd.h"
//#include "candll.h"

//---- Mem Leakage Debug
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//----

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//==============================================================================
//Thread: Monitoring CAN devices, for receive CAN data
//==============================================================================
//Thread function
static DWORD CAN_BUS_MONITORING(LPVOID lParam)
{
 //Get pointer to Class
 CMainDlg *pDlg = (CMainDlg*)lParam;
 //ASSERT(pDlg->IsKindOf(RUNTIME_CLASS(CMainDlg)));
 
 //Synchronisation objects
 CEvent &ev_Quit = pDlg->ev_Quit;
 CEvent &ev_Data = pDlg->ev_DataAccepted;
 CEvent &ev_Exit = pDlg->ev_ExitRequest;
 CEvent &ev_FB   = pDlg->ev_WaitEnd;

 //Synchronisation events
 HANDLE evHandles[] = {ev_Quit, 
	                   ev_Data, 
					   ev_Exit};
 
 DWORD event_numbers = 3;
 //pDlg->exit_request = 1;

 unsigned char Buf[16];
 int more_bytes;
 int exit_request = 0;

 //---- launch Tread loop 
 for(;;)
  {
    //waiting events
    DWORD dwEvent = WaitForMultipleObjects(event_numbers, evHandles, FALSE, INFINITE);
				
	switch(dwEvent)
	{			
       //---- Exit EVENT
       case WAIT_OBJECT_0:
       //==================================================
       //Exit from thread by "Quit" event
       //==================================================
	   TRACE("Main Thread Exit\n");
       //---- End Tread
       AfxEndThread(0, TRUE);
       return 0;		
		
	   //---- EVENT from flag_bufferNotEmpty
       case WAIT_OBJECT_0+1:
       //================================ 
       //Пришли данные, читать
       //================================
       for(;;)
	   {
	     //Prepare exit from main thread
	     //Если закрываем программу - 
	     //запретить заход в новый цикл обработки данных
	     /*
		 if(pDlg->exit_request==2)
	     {
		   //wait only ev_Quit
	   	   //event_numbers = 1;
		   //more_bytes = 0;

		   //pDlg->ev_WaitEnd.SetEvent();
	     }		   
		 */
		 if(exit_request==0)
		 {
		   //Get data from USB circle buffer
	       more_bytes = -1; ///CAN_ReadData(pDlg->pCAN, Buf, MONITORING_CHN, 0, 8);
	       //Handle data 
           pDlg->Parse_DeviceAnswer(Buf);
		   //Sleep(300);
		   //more_bytes = 0;
	       //memset(Buf, 0, sizeof(Buf));
		   ///TRACE(".");
		
		   //exit if circle buffer is empty
		   if(more_bytes<1) break;
		 }
	   }
       break;
       
	   //---- Exit Ini
       case WAIT_OBJECT_0+2:
		    exit_request = 1;
          	event_numbers = 1;
			TRACE("Main Thread Exit Request\n");
			ev_FB.SetEvent();
	   break;
	}
  }

 TRACE("Main Thread Exit\n");
 //---- End Tread loop
 AfxEndThread(0, TRUE);
 return 0;
} 

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CMainDlg::ThreadLaunch(void)
{
 //Stop threads 
 ThreadStop();
 
 //THREAD_PRIORITY_ABOVE_NORMAL   //на один пункт выше нормального
 //Restart all threads THREAD_PRIORITY_HIGHEST
 pThread = AfxBeginThread((AFX_THREADPROC)CAN_BUS_MONITORING, 
                           this, THREAD_PRIORITY_HIGHEST);//THREAD_PRIORITY_ABOVE_NORMAL
 pThread->m_bAutoDelete = FALSE; 
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CMainDlg::ThreadStop(void)
{
 if(pThread)
 {	  /*
   //if thread already was launched
   if(exit_request==1)
   {
	 //команда завершение цикла обработки данных 
     //exit_request = 2;
	 //иницировать цикл обработки данных
	 //ev_DataAccepted.SetEvent();
   	 //ждем завершения цикла, таймаут если данных нет
	 //WaitForSingleObject(ev_WaitEnd.m_hObject, 10000);
	 //delete pThread;
	 //pThread = NULL;
   }*/
   //else
   //{
  ev_ExitRequest.SetEvent();
   
  //ждем завершения цикла, таймаут если данных нет
  WaitForSingleObject(ev_WaitEnd.m_hObject, 5000);

  ev_Quit.SetEvent();
  WaitForSingleObject(pThread->m_hThread, INFINITE);
  delete pThread;
  pThread = NULL;
   //}
 }

 ev_ExitRequest.ResetEvent();
 ev_Quit.ResetEvent();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CMainDlg::Parse_DeviceAnswer(unsigned char *pBuf)
{	
  if(pBuf[0] != HEADER_MONITORING) return;
  pPage4->PID_log = pPage2->PID_log;

  switch (pBuf[1])
  {  
	//----
    case DEVICE_ADC_DATA:
    case DEVICE_ADC_DATA2:
       	 Parse_Monitoring(&pBuf[1]);
    return; 
	
    //----
    case PAGE_PID:
        // pPage2->ParsePID(&pBuf[2]);	
    return;
    
    //----
    case PAGE_MAIN_DLG:
		 Parse_Device(&pBuf[1]);
    return;

  }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CMainDlg::Parse_Monitoring(unsigned char *pBuf)
{
  int header = *pBuf++;

  switch(header)
  {
    //first data block
    case DEVICE_ADC_DATA:
	     memcpy(Device.ADC_value, pBuf, 6);
	     return;

    //second data block
	case DEVICE_ADC_DATA2:
		 memcpy(&Device.ADC_value[3], pBuf, 6);
		 break;
  }
  
  //int jtag = Device.Mode.JTAG_enabled; 
  ///jtag=0;

  //TRACE("Parse_Monitoring Error!\n");
  //ADC signal processing
 // pPage1->LOG.Write_TimeStamp();
 //pPage1->adc_txt.Empty();
  
  /*
  //If JTAG Enabled data is wrong value
  if(jtag==0)
  {
	
    pPage1->CalcIntensity();
    pPage1->CalcMonitor();
	//----
	pPage1->CalcTEC_temperature();
    pPage1->CalcLED_current();
    pPage1->CalcPWR_voltage();
	pPage1->CalcTEC_current();
    //pPage1->CalcHeatSink_temperature();
  }
  else
  {
	//если используется TECv2
	if(Device.Device_type==DEVICE_TECv2_CONTROLLER)
	{
      //----
      //pPage1->CalcLED_current();	  
	  pPage1->CalcTEC_temperature();
	  pPage1->CalcTEC_current();
	  pPage1->CalcTEC_voltage();
	  pPage1->CalcHeatSink_temperature();
      pPage1->CalcPWR_voltage();
	}
	else
	{	
	  pPage1->m_list_adc.DeleteString(5);
	  pPage1->m_list_adc.DeleteString(4);
	  pPage1->m_list_adc.DeleteString(3);
	  pPage1->m_list_adc.DeleteString(2);
      pPage1->m_list_adc.InsertString(2,"Can`t read: Ch3..Ch6");
	  pPage1->m_list_adc.InsertString(3,"JTAG Enabled");
	  
	}
  }	  */

  //--------------------------------------------
  //если используется TECv2
  //--------------------------------------------
  if(Device.Device_type==DEVICE_TECv2_CONTROLLER)
  {	 /*
      //----
      //pPage1->CalcIntensity();
	  //pPage1->CalcLED_current();	  
	  pPage1->CalcTEC_temperature();
	  pPage1->CalcTEC_current();
	  pPage1->CalcTEC_voltage();
	  pPage1->CalcHeatSink_temperature();
      pPage1->CalcPWR_voltage();
	  //pPage1->m_txt_adc.SetWindowTextA(pPage1->adc_txt);
	*/  
	  if(tab_opened==3)
	  {
	    //pPage4->Show_Temperature(0);
	    //pPage4->Show_TECcurrent(0);
	  }
  }
  else
  {	  
	  /*
	  pPage1->m_list_adc.DeleteString(5);
	  pPage1->m_list_adc.DeleteString(4);
	  pPage1->m_list_adc.DeleteString(3);
	  pPage1->m_list_adc.DeleteString(2);
      pPage1->m_list_adc.InsertString(2,"Can`t read: Ch3..Ch6");
	  pPage1->m_list_adc.InsertString(3,"JTAG Enabled");
	  */
  }



  //обновить содержание окна мониторинга
  //pPage1->m_list_adc.ResetContent();
  //pPage1->m_list_adc.InsertString(0, pPage1->list_adc_txt);

  //----
  //pPage3->Show_LEDsCurrent(jtag);
///  pPage4->Show_Temperature();
///  pPage4->Show_Current();
///  pPage4->Show_Voltage();

  //Increment string for xls file
  ///pPage1->Log_Increment();

  if(monitoring_ack>10) //4
  {
    ///CAN_SendCommand(pCAN,CAN_device_ID,0,CMD_MONITORING_ACK,0,0);
	monitoring_ack = 0;
  }
  else monitoring_ack++;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CMainDlg::Parse_Device(unsigned char *pBuf)
{
  unsigned char data[8];
  memcpy(data, pBuf, 7);
  CString txt, dig;

  //----
  if(data[1]==DEV_SETUP_UPDATE_REQUEST ||
	 data[1]==DEV_SET_DEFAULT_COMLETED)
  {
	TRACE("Device ask update application settings\n");
	int ret = 0;///pPage2->GetDeviceSetup(CAN_device_ID);
	if(ret)
	{
	  pPage1->Controls_Update();
      pPage2->Controls_Update();
	  pPage3->Controls_Update();
	  pPage4->Controls_Update();
	  pPage5->Controls_Update();
	  pPage6->Controls_Update();	
	}
  }

  //----
  if(data[1]==DEV_TEC_DCDC_ERROR)
  {
	TRACE("Device TEC DCDC can`t start\n");
	//pPage4->GetDeviceSetup(CAN_device_ID);
	AfxMessageBox(_T("TEC DCDC Error"), MB_ICONSTOP);
  }
}
