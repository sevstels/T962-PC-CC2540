//==============================================================================
//File name:   "led-blink.cpp"
//Purpose:      Source File
//Version:      2.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "afxwin.h"
#include "led-blink.h"

#define ID_TIMER_BLINK 1

CLEDBlink *pCBlnk;

//------------------------------------------------------------------------------
//Constructor
//------------------------------------------------------------------------------
CLEDBlink::CLEDBlink()
{
  hTimer = 0;
  triger = 0;
  led_status = 0;
  txt_status = 0;
  repetitions = 0;
  hLED_OFF = NULL;
  hLED_ON = NULL;
  hLED_Last = NULL;
  hLED_RED = NULL;
  txt_last.Empty();
  pCBlnk = this;
  pLed = NULL;
  pTxt = NULL;
  hWND = NULL;
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
CLEDBlink::~CLEDBlink()
{
  // Destroy timer
  if(hTimer > 0)	
  {
	::KillTimer(NULL, hTimer);
	hTimer = 0;
  }
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CLEDBlink::Ini(HWND hwnd, CStatic *pled, CStatic *ptxt,
	                HBITMAP hImage1, HBITMAP hImage2, HBITMAP hImage3)
{
  hLED_RED = hImage3;
  hLED_ON = hImage2;
  hLED_OFF = hImage1;
  pLed = pled;
  pTxt = ptxt;
  hWND = hwnd;
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
static void CALLBACK OnCbTimer(HWND, UINT, UINT idTimer, DWORD dwTime)
{
  if(idTimer == pCBlnk->hTimer)	
  {
	pCBlnk->Tick();
  }
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CLEDBlink::Tick(void)
{
  if(triger==0)
  {
    if(hLED_ON!=NULL) 
	   pLed->SetBitmap(hLED_ON);
    triger = 1;
  }
  else
  {
    if(hLED_OFF!=NULL) 
	   pLed->SetBitmap(hLED_OFF);
    triger = 0;
  }	
	
  repetitions--;
	
  if(repetitions<=0)
  {
	::KillTimer(NULL, hTimer);
	hTimer = 0;
	triger = 0;
	repetitions = 0;
	  
	if(txt_status==1)
    {
	  //----
	  pTxt->SetWindowTextA(txt_last);
	  txt_status = 0;
	}

    if(led_status==1)
	{
	   if(hLED_ON!=NULL) pLed->SetBitmap(hLED_ON);
	}
    else
	{
	   if(hLED_OFF!=NULL) pLed->SetBitmap(hLED_OFF);
	}
  }
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CLEDBlink::Blink(int status, int count, int period, char *pMsg)
{
  // Destroy any previous timer
  if(hTimer > 0)	
  {
	::KillTimer(NULL, hTimer);
	hTimer = 0;
  }

  repetitions = count<<1;
  led_status = status; 

  if(pMsg!=NULL)
  {
	pTxt->GetWindowTextA(txt_last);
	pTxt->SetWindowTextA(pMsg);
	txt_status = 1;
  }

  //hOldLED = pLed->GetBitmap();

  if(led_status==1){triger = 0; pLed->SetBitmap(hLED_OFF);}
  else{triger = 1; pLed->SetBitmap(hLED_ON);}  

  //Start timer	
  hTimer = ::SetTimer(hWND, ID_TIMER_BLINK, period, (TIMERPROC)OnCbTimer);
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CLEDBlink::BlinkRed(int status, int count, int period, char *pMsg)
{
  // Destroy any previous timer
  if(hTimer > 0)	
  {
	::KillTimer(NULL, hTimer);
	hTimer = 0;
  }

  repetitions = count<<1;
  led_status = status; 

  if(pMsg!=NULL)
  {
	pTxt->GetWindowTextA(txt_last);
	pTxt->SetWindowTextA(pMsg);
	txt_status = 1;
  }

  //hOldLED = pLed->GetBitmap();

  if(led_status==1){triger = 0; pLed->SetBitmap(hLED_RED);}
  else{triger = 1; pLed->SetBitmap(hLED_ON);}  

  //Start timer	
  hTimer = ::SetTimer(hWND, ID_TIMER_BLINK, period, (TIMERPROC)OnCbTimer);
}