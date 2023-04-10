//==============================================================================
//File name:   "log-concole.cpp"
//Purpose:      Source File
//Version:      2.00
//Copyright:    (c) 2021, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include <windows.h>
#include "log-console.h"
//#include "Wincon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

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

#define FOREGROUND_BLUE      0x0001 // text color contains blue.
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define BACKGROUND_BLUE      0x0010 // background color contains blue.
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
#define   BLACK	     0
#define   BLUE		 FOREGROUND_BLUE
#define   GREEN	     FOREGROUND_GREEN
#define   RED	     FOREGROUND_RED
#define   YELLOW    (FOREGROUND_GREEN | FOREGROUND_RED)
#define   MAGENTA	(FOREGROUND_BLUE  | FOREGROUND_RED)
#define   CYAN		(FOREGROUND_GREEN | FOREGROUND_BLUE)
#define   WHITE	     7

static CLOGW *pCLOGW;
//BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);

//------------------------------------------------------------------------------
//constructor
//------------------------------------------------------------------------------
CLOGW::CLOGW()
{ 
  opened = 0;
  wnd_setup = 0;
  time_stamp = 0;
  handle = NULL;
  pf = NULL;
  
  //размер буфера экрана
  wnd_width = 400;
  buf_width = 400;
  wnd_height = 400;
  buf_height = 1000;
  pCLOGW = this;
}

//------------------------------------------------------------------------------
//destructor
//------------------------------------------------------------------------------
CLOGW::~CLOGW()
{
  //Close console
  Close();
}

//------------------------------------------------------------------------------
//https://docs.microsoft.com/en-us/windows/console/setconsolectrlhandler?redirectedfrom=MSDN
//https://docs.microsoft.com/en-us/windows/console/registering-a-control-handler-function
//https://docs.microsoft.com/en-us/windows/console/handlerroutine?redirectedfrom=MSDN
//https://docs.microsoft.com/en-us/windows/console/console-functions
//------------------------------------------------------------------------------
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
 switch (fdwCtrlType)
 {
    //Handle the CTRL-C signal.
    case CTRL_C_EVENT:
         TRACE("Ctrl-C event\n\n");
         pCLOGW->Close();
		 return TRUE;

    // CTRL-CLOSE: confirm that the user wants to exit.
    case CTRL_CLOSE_EVENT:
         TRACE("Ctrl-Close event\n\n");
         //pCLOGW->Close();
		 FreeConsole();
		 return FALSE;

    // Pass other signals to the next handler.
    case CTRL_BREAK_EVENT:
         TRACE("Ctrl-Break event\n\n");
        
		 return TRUE;//FALSE;

    case CTRL_LOGOFF_EVENT:
         TRACE("Ctrl-Logoff event\n\n");
         return TRUE;//FALSE;

    case CTRL_SHUTDOWN_EVENT:
         TRACE("Ctrl-Shutdown event\n\n");

         return TRUE;//FALSE;

    default:
        return TRUE;//FALSE;
  }

}
  //GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT,0); закрыть 
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int CLOGW::Open(const char *pName)
{
 if(opened==1) return 1;
 
 setlocale(LC_ALL, "en_GB");
 
 //устанавливает кодировку ввода из консоли и из редактора кода;
 SetConsoleCP(1252);
 
 //устанавливает кодировку вывода на консоль;	 
 SetConsoleOutputCP(1252); 

 //----
 AllocConsole();
 SetConsoleTitleA((LPCSTR)pName);
 pConsoleName = pName;
 
 //читаем дескриптор стандартного вывода
 handle = GetStdHandle(STD_OUTPUT_HANDLE);
  
 if(handle!=NULL) opened = 1;
 else return 0;

 //—начала задаем минимальные значени€
 SMALL_RECT zeroWindow = {0, 0, 0, 0};
 COORD zBuffer = {1, 1};
 SetConsoleWindowInfo(handle, TRUE, &zeroWindow); 
 SetConsoleScreenBufferSize(handle, zBuffer);
 
 //ј уже потом измен€ем значени€ на нужные нам
 COORD bufferSize = {buf_width, buf_height};
 SMALL_RECT windowSize = {0, 0, wnd_width - 1, wnd_height - 1};
 SetConsoleScreenBufferSize(handle, bufferSize);
 SetConsoleWindowInfo(handle, TRUE, &windowSize);

 //устанавливаем новый размер буфера экрана
 //BOOL ret = SetConsoleScreenBufferSize(handle, coord);

 //open file "read write" mode "rb+"
 //fopen_s(&pf,"Device-events.txt","wb+");

 //-------------------------------------
 //установить размер окна
 //-------------------------------------
 RECT rect;
 HWND hWindConsole = GetConsoleWindow();
    
 //stores the console's current dimensions
 GetWindowRect(hWindConsole, &rect);
 MoveWindow(hWindConsole, rect.left, rect.top, wnd_width, wnd_height, TRUE);

 //добавить обработчик событий окна конслои
 BOOL ret = SetConsoleCtrlHandler(CtrlHandler, TRUE);

 return opened;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CLOGW::Close(void)
{
  if(opened==0) return;
  
  opened = 0;

  //Close console
  CloseHandle(handle);
  handle = NULL;
}

//------------------------------------------------------------------------------
//Function:	 https://www.cyberforum.ru/win-api/thread520249.html
//------------------------------------------------------------------------------
void CLOGW::SetWindowSize(int width, int heigth)
{
  wnd_width = width; 
  wnd_height = heigth;
     
  buf_width = width;
  buf_height = heigth*60;
  
  //если окно консоли было открыто
  if(opened==1)
  {
    HWND hWindConsole = GetConsoleWindow();
    RECT rect;
    //stores the console's current dimensions
    GetWindowRect(hWindConsole, &rect);
    MoveWindow(hWindConsole, rect.left, rect.top, wnd_width, wnd_height, TRUE);
  }
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int CLOGW::Write(const char *format, ...)
{
  if(opened!=1){Open("Device log");}
  if(opened!=1) return -1;
	
  char buf[1024];
  memset(buf, 0, sizeof(buf));

  va_list vaList;
  va_start(vaList, format);
  vsprintf_s(buf, format, vaList);
  va_end(vaList);
  
  //fwrite(buf, 1, strlen(buf), pf);
  //fflush(pf);

  if(handle==NULL) return -1;
  if(WriteConsoleA(handle, buf, strlen(buf), 0, 0)== 0){return 1;}
  return -1;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------ 
void CLOGW::Msg(int type, int stamp, char *pSrc, char *format, char *pTxt)
{
  std::string str;
  WORD atributes;

  if(stamp)
  {
	GetTimeStamp();
	str = str_stamp;
	str += ": ";
  }
  else
  {
     str = "";
  }
  //Device source 
  str += pSrc;
  str +="> ";
  //color setup 
  SetConsoleTextAttribute(handle, WHITE);
  
  //Trailer
  Write("%s", str.c_str());
  
  //Msg color
  if(type==LOG_ERR){atributes = RED | FOREGROUND_INTENSITY;}
  else if(type==LOG_WARN){atributes = YELLOW | FOREGROUND_INTENSITY;}
  else if(type==LOG_OK){atributes = GREEN;}
  else{atributes = WHITE;} 	

  //color setup 
  SetConsoleTextAttribute(handle, atributes);
  
  //Tail
  Write(format, pTxt);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
/*
template <typename ... Args>
std::string string_format(char* fmt, Args... args)
{
    size_t size = snprintf(nullptr, 0, fmt, args...);
    std::string buf;
    buf.reserve(size + 1);
    buf.resize(size);
    snprintf(&buf[0], size + 1, fmt, args...);
    return buf;
} */  /*
#include <stdlib.h>  
#include <stdio.h>  
#include <stdarg.h> 
#include <string.h>

       int vasprintf(char** strp, const char* fmt, va_list ap) {
            va_list ap2;
            va_copy(ap2, ap);
            char tmp[1];
            int size = vsnprintf(tmp, 1, fmt, ap2);
            if (size <= 0) return size;
            va_end(ap2);
            size += 1;
            *strp = (char*)malloc(size * sizeof(char));
            return vsnprintf(*strp, size, fmt, ap);
        }	  */
//https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf/2342176
//http://qaru.site/questions/8459/stdstring-formatting-like-sprintf
std::string string_format(const char *fmt, ...)
{
  char textString[128*5];

  // -- Empty the buffer properly to ensure no leaks.
  memset(textString, 0, sizeof(textString));

  va_list args;
    
  va_start (args, fmt );
  vsnprintf_s (textString, 128*5, fmt, args );
    
  va_end (args);
  std::string retStr = textString;

  return retStr;
 /*
  char *buffer = NULL;
  va_list ap;

  va_start(ap, fmt);
  (void)vasprintf(&buffer, fmt, ap);
  va_end(ap);

  std::string result = buffer;
  free(buffer);

  return result;   */
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------ 
void CLOGW::GetTimeStamp(void)
{
  //Get time
  GetLocalTime(&wtime);

  str_stamp =  string_format("%02d", wtime.wHour);
  str_stamp += "h ";
  str_stamp += string_format("%02d", wtime.wMinute);
  str_stamp += "m ";
  str_stamp += string_format("%02d", wtime.wSecond);
  str_stamp += "s ";
  str_stamp += string_format("%03d", wtime.wMilliseconds);
  str_stamp += "mS";
  /*
  //---- create 
  str_stamp =  std::to_string((long long)wtime.wHour);
  str_stamp += "h";
  str_stamp += std::to_string((long long)wtime.wMinute);
  str_stamp += ":";
  str_stamp += std::to_string((long long)wtime.wSecond);
  str_stamp += ".";
  str_stamp += std::to_string((long long)wtime.wMilliseconds);
  str_stamp += "uS";
  return; */
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CLOGW::Clear(void)
{
  COORD topLeft  = { 0, 0 };
  HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO screen;
  DWORD written;

  GetConsoleScreenBufferInfo(console, &screen);
  
  //----
  FillConsoleOutputCharacterA(console, ' ', 
	                          screen.dwSize.X * screen.dwSize.Y, 
							  topLeft, &written);
  //----  
  FillConsoleOutputAttribute(console, FOREGROUND_GREEN | 
	                         FOREGROUND_RED | FOREGROUND_BLUE,
                             screen.dwSize.X * screen.dwSize.Y, 
							 topLeft, &written);
  //----
  SetConsoleCursorPosition(console, topLeft);
}

#include <iostream>
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CLOGW::ClearStd(void) 
{
  //CSI[2J clears screen, CSI[H moves the cursor to top-left corner
  std::cout << "\x1B[2J\x1B[H";
}
