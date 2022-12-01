#include "stdafx.h"
#include "Page6.h"
#include "cmd.h"

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
//Thread: 
//==============================================================================
//Thread function
static DWORD DFU_PROGRAMM(LPVOID lParam)
{
 //Get pointer to Class
 CPage6 *pDlg = (CPage6*)lParam;
 ASSERT(pDlg->IsKindOf(RUNTIME_CLASS(CPage6)));
 
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
///           pDlg->Parse_DeviceAnswer(Buf);
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
void CPage6::ThreadLaunch(void)
{
 //Stop threads 
 ThreadStop();
 
 //THREAD_PRIORITY_ABOVE_NORMAL   //на один пункт выше нормального
 //Restart all threads THREAD_PRIORITY_HIGHEST
 pThread = AfxBeginThread((AFX_THREADPROC)DFU_PROGRAMM, 
                           this, THREAD_PRIORITY_HIGHEST);//THREAD_PRIORITY_ABOVE_NORMAL
 pThread->m_bAutoDelete = FALSE; 
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPage6::ThreadStop(void)
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