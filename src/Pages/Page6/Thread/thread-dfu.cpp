//==============================================================================
//File name:   "thread-dfu.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page6.h"
#include "cmd.h"
#include "lpc21isp.h"
#include "files-cpp.h"

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

int thread_end(void)
{
 TRACE("DFU Thread Exit\n");
 //pDlg->pThread = NULL;
 //---- End Tread loop
 AfxEndThread(0, TRUE);
 return 0;
}

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
  //CEvent &ev_Quit = pDlg->ev_Quit;
  //CEvent &ev_Data = pDlg->ev_DataAccepted;
  //CEvent &ev_Exit = pDlg->ev_ExitRequest;
  //CEvent &ev_FB   = pDlg->ev_WaitEnd;

  //Synchronisation events
  //HANDLE evHandles[] = {ev_Quit, 
  //	                    ev_Data, 
  //	   				    ev_Exit};
 
  //DWORD event_numbers = 3;
  //pDlg->exit_request = 1;

  //unsigned char Buf[16];
  //int more_bytes;
  //int exit_request = 0;

  TRACE("DFU Thread Run\n");

  //----------------------------------------------------------
  //
  //----------------------------------------------------------
  CString str, file_path, file_name, txt;

  //Standart file dialog
  CFileDialog fileDialog(NULL, NULL, _T("*.bin"), 
                         NULL, _T("firmware file (*.bin)"));
  //Call dlg window
  INT_PTR dlg = fileDialog.DoModal();

  if(dlg==IDOK)
  {
     file_path = fileDialog.GetPathName();
     file_name = fileDialog.GetFileName();
  }

  //---------------------------------------
  //
  //---------------------------------------
  //file_name = "t962fw.bin";
   
  int exist = CheckFile(file_path);
  if(exist!=1)
  {
    str = "Error: No Firmware file";
    pDlg->m_edit_info.SetWindowText(_T(str));
    return thread_end();
  }

  unsigned long length = Get_FileSize(file_path);
  if(length<200)
  {
    str = "Error: Firmware file wrong";
    pDlg->m_edit_info.SetWindowText(_T(str));
    return thread_end();
  }

  char *pBuf = (char*) new char[length];
  if(pBuf==NULL)  return thread_end();

  unsigned long read_length = Read_File(file_path, pBuf, 0, length);
  if(read_length != length)  return thread_end();


  //---------------------------------------
  //Programming
  //---------------------------------------
  ISP_ENVIRONMENT isp;
  memset(&isp, 0, sizeof(ISP_ENVIRONMENT));
          
  isp.BinaryContent = (unsigned char*)pBuf;
  isp.BinaryLength = length;
  isp.ProgramChip = 1;
  isp.WipeDevice = 1;
  isp.Verify = 1;

  int ret = NxpDownload(&isp);
  if(ret!=0)
  {
	if(ret==-2)
	{
	  DebugPrintf("\r\nProgramming interrupted");
	}
	//show Error
	else pDlg->Progress_Error();
  }
  
  delete [] pBuf;
  pBuf = NULL;
  
  pDlg->program_run = 0;
  pDlg->pTab->EnableWindow(TRUE);
  pDlg->m_but_fw_update.SetWindowTextA("FW Update");
  pDlg->pThread = NULL;

  return thread_end();
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
                           this, THREAD_PRIORITY_ABOVE_NORMAL);
 pThread->m_bAutoDelete = TRUE; 
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPage6::ThreadStop(void)
{
 if(pThread!=NULL)
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
   //WaitForSingleObject(ev_WaitEnd.m_hObject, 5000);

   ev_Quit.SetEvent();
   if(pThread!=NULL) WaitForSingleObject(pThread->m_hThread, INFINITE);
   
   if(pThread!=NULL) delete pThread;
   pThread = NULL;
 }

 ev_ExitRequest.ResetEvent();
 ev_Quit.ResetEvent();
}