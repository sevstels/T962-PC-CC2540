#include "stdafx.h"
#include "Images.h"
#include <math.h>

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

//HDC -это идентификатор структуры данных,описывающих устройство вывода графики
//CDC -это контекст графического устройства
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CImages::Draw_Window(CDC *pDC, HWND hWnd)
{    
  DWORD line_number;
  CRect window_position;
  LPRECT pRc =&window_position;
  LPDWORD pLine = &line_number;
//  unsigned short line;
//  unsigned short wind_bott;
//  unsigned short wind_top;
					
  //--------------------------------------------------------
  //Copy BackGround image to output buffer
  //--------------------------------------------------------
  WINDOW.BitBlt( 0,0, Window_Rect.Width(),Window_Rect.Height(),
				 &BGND, 0, 0, SRCCOPY);

  //-------------------------------------------------------
  //Copy Curve Graph image to output buffer 
  //-------------------------------------------------------  
  WINDOW.BitBlt( WindGraph_Rect.left, WindGraph_Rect.top,
				 WindGraph_Rect.Width(),WindGraph_Rect.Height(),
				 &CURVE, 0, 0, SRCAND);// MERGECOPY
   /*	
 //sync, do not delete
 if(DirectX_ini)
 { 
  
  //Wait while window refreshed
  do 
   {
	  ::GetWindowRect(hWnd, pRc);
      
	  //Get current line number for Display refresh
	  lpDD->GetScanLine(pLine);	//Direct Draw Function
	  wind_top  = (unsigned short)window_position.top; 
	  wind_bott = (unsigned short)window_position.bottom;
	  line = (unsigned short)line_number;
	  
	  //if (line_number < window_position.top) break;
	  if (line > wind_bott || line+200 < wind_top) break; 

    } while(1);
 }
	 */
 //Copy output BMP image buffer to Display, скопировать на экран
 pDC->BitBlt(6,12, Window_Rect.Width(),Window_Rect.Height(),
				&WINDOW,0,0,SRCCOPY);  
 
  

 //memCUR.Detach();
 //memBMP.Detach();

 /*
 memCUR.ReleaseOutputDC();
 memBMP.ReleaseOutputDC();
 memCUR.DeleteDC();
 memBMP.DeleteDC();
 bmp_Curves.DeleteObject();
 bmp_Graph.DeleteObject();

 pDC->ReleaseOutputDC();
 pDC->DeleteDC();
 */
 //pDC->Detach();
 //pDC = NULL;
}

