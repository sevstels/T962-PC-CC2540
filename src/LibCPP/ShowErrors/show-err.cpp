//==============================================================================
//File name:   "show-err.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2018, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "afxwin.h"

#include <windows.h>
#include <strsafe.h>
#include "show-err.h"


void ShowError(LPTSTR lpszFunction) 
{ 
  //Retrieve the system error message for the last-error code
  LPVOID lpMsgBuf;
  LPVOID lpDisplayBuf;
  DWORD dw = GetLastError(); 

  FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

  //Display the error message and exit the process
  lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
                 (lstrlen((LPCTSTR)lpMsgBuf) + 
                  lstrlen((LPCTSTR)lpszFunction) + 40)* sizeof(TCHAR)); 
    
  StringCchPrintf((LPTSTR)lpDisplayBuf, 
                   LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                   TEXT("%s Failed with error %d\r\n%s"), 
                   lpszFunction, dw, lpMsgBuf); 
  

  #ifdef NUKLEAR_GUI

  #else
  MessageBox(NULL,(LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 
  #endif
  
  LocalFree(lpMsgBuf);
  LocalFree(lpDisplayBuf); 
}
