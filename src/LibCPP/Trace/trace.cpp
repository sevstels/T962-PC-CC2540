//==============================================================================
//File name:    "trace.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2013, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
//Тут предкомпиляция заканчивается
//#pragma hdrstop
#include "trace.h"

//------------------------------------------------------------------------------
//Windows version
//------------------------------------------------------------------------------
//#ifdef _WIN32					
#include <windows.h>

void _trace(const char *format, ...)
{
   #ifndef NDEBUG 
   char buffer[256];

   va_list argptr;
   va_start(argptr, format);
   wvsprintf((LPSTR)buffer, (LPSTR)format, argptr);
   vfprintf(stdout, format, argptr);
   va_end(argptr);
   fflush(stdout);
   OutputDebugString((LPSTR)buffer);
   #endif

}
//#endif
 
//------------------------------------------------------------------------------
//Linux version
//------------------------------------------------------------------------------
//Begin of Linux stuff      
#ifdef  __linux__	

void _trace(const char *format, ...)
{
   va_list argptr;
   va_start(argptr, format);
   vfprintf(stdout, format, argptr);
   va_end(argptr);
   
   fflush(stdout); 
}
#endif
