//==============================================================================
//File name:   "led-blink.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _LED_BLINK_H_
#define _LED_BLINK_H_

#include <Windef.h>

//------------------------------------------------------------------------------
// CBTColor class
//------------------------------------------------------------------------------
class CLEDBlink
{
 //Construction
 public:
 CLEDBlink();
~CLEDBlink();

 void Ini(HWND hwnd, CStatic *pled,  CStatic *ptxt, 
		  HBITMAP hImage1, HBITMAP hImage2, HBITMAP hImage3);

 void Blink(int status, int count, int period, char *msg);
 void BlinkRed(int status, int count, int period, char *pMsg);
 void Tick(void);
 unsigned int hTimer;

 private:
 int triger;
 int led_status;
 int txt_status;
 int repetitions;
 
 CStatic *pLed;
 CStatic *pTxt;
 CString txt_last;
 
 HBITMAP hLED_OFF;
 HBITMAP hLED_ON;
 HBITMAP hLED_RED;
 HBITMAP hLED_Last;
 HWND hWND;

};

#endif
