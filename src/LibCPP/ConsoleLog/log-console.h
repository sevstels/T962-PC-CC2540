//==============================================================================
//File name:   "log-concole.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _LOG_CONSOLE_H_
#define _LOG_CONSOLE_H_

#include <stdio.h>
#include <string>

//----
#define LOG_ERR    3
#define LOG_WARN   2
#define LOG_OK     1
#define LOG_INFO   0

//----
#define LOG_RED    LOG_ERR
#define LOG_YELLOW LOG_WARN
#define LOG_GREEN  LOG_OK
#define LOG_WHITE  LOG_INFO

#define LOG_TIME   1

//------------------------------------------------------------------------------
//class
//------------------------------------------------------------------------------
class CLOGW
{
  //Construction
  public:
  //----
  CLOGW();
 ~CLOGW();
  
  int  Open(const char *pName);
 
  int  Write(const char *format, ...);
  void SetWindowSize(int width, int heigth);
  void Msg(int type, int stamp, char *pSrc, char *format, char *pTxt);
  void Close(void); 
  void Clear(void);
  void ClearStd(void);

  int opened, wnd_setup;
  int time_stamp;
  SYSTEMTIME wtime;
  std::string str_stamp;
  
  protected:
  //----
  int wnd_width; int wnd_height;
  int buf_width; int buf_height;
  //----
  void GetTimeStamp(void);
  const char *pConsoleName;
  void *handle;
  FILE *pf;
};

#endif