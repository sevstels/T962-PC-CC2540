//==============================================================================
//File name:   "trace.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2017, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _DEBUG_TRACE_H_
#define _DEBUG_TRACE_H_

#ifdef  __cplusplus
extern "C" 
{
#endif //end __cplusplus

  void _trace(const char *format, ...);

#ifdef __cplusplus
}
#endif //end __cplusplus
 
//---- for Linux   
#ifdef __linux__

#ifdef  _DEBUG
#define TRACE _trace
#define TRACE do{} while(0)
#endif

#else

//---- for Windows 
#if defined _DEBUG | defined DEBUG
#define DBG_TRACE _trace
#else	
#define DBG_TRACE
#endif 

#endif

#endif //_ _H_
