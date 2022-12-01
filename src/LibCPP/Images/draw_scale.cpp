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

//------------------------------------------------------------------------------
//Draw Horizontal X-Scale
//------------------------------------------------------------------------------
void CImages::Draw_Scale_X(CDC *pCDC, int x0, int y0, int width, int height,
		                       int line_number, COLORREF color)
{ 
  CString txt; 
  char line_width = 1;
  int step_x;
  int txt_digit = 0;
  int txt_x_offset = 2;
  int txt_y_offset = -6;

  //------------------------------------------
  //Set parameters for Text output
  //------------------------------------------
  //set font
  CFont font;
  font.CreateFont(11,0,0,0,FW_BOLD,0,0,0,RUSSIAN_CHARSET,
				OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY,VARIABLE_PITCH | FF_DONTCARE,"Arial");
 
  pCDC->SelectObject(&font);
  pCDC->SetTextColor(RGB(180,180,180));

  //Draw vertical lines by step
  step_x = (int)(width/50);//2000 samples / 40 = 50 samples by cell
  for (int x = x0; x <=(width + x0); x += step_x) 
  {
    DrawLine(pCDC, line_width, x, y0, x, height+y0, color);

    //Print horizontal scale digit
	txt.Format("%d", txt_digit);
	txt_digit++;

    if(txt_digit>=10)txt_x_offset=4;
    if(txt_digit>=100)txt_x_offset=6;
    if(txt_digit>=1000)txt_x_offset=10;
	//Text out
    pCDC->TextOutA(x-txt_x_offset, y0+txt_y_offset, txt);	
  }

  //---- delete graphic objects
  font.DeleteObject();
}

//------------------------------------------------------------------------------
//Draw horisontal lines by X direction ----
//------------------------------------------------------------------------------
void CImages::Draw_Scale_Y(CDC *pCDC, int x0, int y0, int width, int height, 
	                       int line_number, COLORREF color)
{ 
  CString txt; 
  int step_y;
  int txt_step;
  int line_width = 1;

  //------------------------------------------
  //Set parameters for Text output
  //------------------------------------------
  //set font
  CFont font;
  font.CreateFont(11,0,0,0,FW_BOLD,0,0,0,RUSSIAN_CHARSET,
				OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY,VARIABLE_PITCH | FF_DONTCARE,"Arial");
 
  pCDC->SelectObject(&font);
  pCDC->SetTextColor(RGB(255,0,0));
  
  //-----------------------------------------------------------
  //Draw horisontal lines
  //-----------------------------------------------------------
  //Calc vertical pixel steps 
  step_y = (int)(height/line_number); 
  txt_step = step_y/3;
  int txt_digit = 0;
  int txt_x_offset = 18;
  int txt_y_offset = 6;

  for(int y = y0; y <= height; y += step_y)
  {
    DrawLine(pCDC, line_width, x0, y, width, y, color);

    //Print vertical scale digit
	txt.Format("%d", txt_digit);
	txt_digit += txt_step;

	//Text out    
	pCDC->TextOutA(x0-txt_x_offset, y+txt_y_offset, txt);
  } 

  //---- delete graphic objects
  font.DeleteObject();
}
