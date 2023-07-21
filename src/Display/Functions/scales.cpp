//==============================================================================
//File name:    "scales.cpp"
//Purpose:      Source File, CPage2 property page
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "display.h"
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
//Scale	setup
//------------------------------------------------------------------------------   
void CDSPL::ScaleRange(int x0, int x, int cells_x, int y0, int y, int cells_y)
{
  //размер по горизонтали
  scale_range_x0 = x0; 
  scale_range_x = x;
  
  //размер по вертикали
  scale_range_y0 = y0; 
  scale_range_y = y;
  
  //число ячеек 
  scale_cell_x = cells_x; 
  scale_cell_y = cells_y;
  
  //начальная точка графика в координатах дисплея
  graph_x0 = x0;
  graph_y0 = y0;
  
  //для расчета экранных координат относительно температуры и положения
  ///graph_factor_x = 1; 
  graph_factor_x = scale_width_x;
  graph_factor_x /= x-x0;

  //загружаем высоту графика в пикселях дисплея
  graph_factor_y = scale_height_y;
  //делим на температуру в градусах
  graph_factor_y /=	y-y0;
}

//------------------------------------------------------------------------------
//Scale	setup
//------------------------------------------------------------------------------   
void CDSPL::ScaleRangeX(int x0, int x, int cells_x)
{
  //размер по горизонтали
  scale_range_x0 = x0; 
  scale_range_x = x;
  
  //число ячеек по горизонтали
  scale_cell_x = cells_x;
  
  //начальная точка графика в координатах дисплея
  graph_x0 = x0;
  
  //для расчета экранных координат относительно температуры и положения
  graph_factor_x = 1; 
}

//------------------------------------------------------------------------------
//Scale	setup
//------------------------------------------------------------------------------
void CDSPL::ScaleOffset(int x0, int y0)
{ 
  scale_offset_x = x0;
  scale_offset_y = y0;
}

//------------------------------------------------------------------------------
//Scale	setup
//------------------------------------------------------------------------------
void CDSPL::ScaleSize(int width, int height)
{
  scale_width_x = width;
  scale_height_y = height;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::Draw_Scales(void)
{
  //Draw graph scale
  IMG.FillSolidRect(0, 0, graph_width, graph_height, RGB(255,255,255));
 
  //---- Clear
  //SCL.FloodFill(0, 0, RGB(255,255,255));

  //----
  Draw_ScaleX(&SCL, scale_offset_x, scale_offset_y, 
	          scale_width_x, scale_height_y, 
			  scale_cell_y, RGB(200,200,200));

  Draw_ScaleY(&SCL, scale_offset_x, scale_offset_y, 
	          scale_width_x, scale_height_y, 
			  scale_cell_x, RGB(200,200,200));
}

//------------------------------------------------------------------------------
//Draw Vertical Y-Scale lines
//------------------------------------------------------------------------------
void CDSPL::Draw_ScaleY(CDC *pCDC, int x0, int y0, int width, int height,
                               int cell_number, COLORREF color)
{
  CString txt;
  char line_width = 1;
  int line_length; 
  int step_x;
  int current_step = scale_range_x0;
  int txt_x_offset = 2;
  int txt_y_offset = -3;
  float f_temp;
  int time_min;
  int time_sec;
  int offset = 0;
  int cycle = 0;
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
  line_length = height;
  step_x = (int)(width/cell_number);
  
  //TRACE1("Step X: %f\n", (float)((float)width/(float)cell_number));
  
  //---- Vertical lines
  for(int x=x0; x <=width+x0; x += step_x) 
  {
    Draw_Line(pCDC, line_width, x, y0, x, line_length+y0, color);

	int time = current_step+(cycle*step_x);
	if(time<=60){ txt.Format("%ds", time); txt_x_offset = 6;}
	else
	{
	  //находим число минут
	  int min = (time/60);
	  int sec = time-(60*min);
	  //min
	  txt_x_offset=6; 
	  txt.Format("%d:%d", min, sec);
	}
     //Print horizontal scale digit
   
    //current_step++;
	cycle++;

/*  if(txt_digit>=10)txt_x_offset=2;
    if(txt_digit>=100)txt_x_offset=4;
    if(txt_digit>=1000)txt_x_offset=8;	*/
	//---
	//Clean place
	pCDC->TextOutA(x-txt_x_offset, y0+txt_y_offset,"         ");//	
    //Text out
    pCDC->TextOutA(x-txt_x_offset, y0+txt_y_offset, txt);//	
  }

  //---- delete graphic objects
  font.DeleteObject();
}

//------------------------------------------------------------------------------
//Draw horisontal lines by X direction	+
//------------------------------------------------------------------------------
void CDSPL::Draw_ScaleX(CDC *pCDC, int x0, int y0, int width, int height, 
                        int cell_number, COLORREF color)
{ 
  CString txt;
  int y, step_y;
  float txt_step;
  int line_width = 1;
  int line_length;

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
  step_y = (int)(height/cell_number);
  //Calc txt value steps размер по вертикали
  txt_step = scale_range_y - scale_range_y0;
  txt_step /= scale_cell_y; //число ячеек

  float txt_digit = scale_range_y0;
  int txt_x_offset = 24;
  int txt_y_offset = 6;

  //TRACE1("Step Y: %f\n", (float)((float)height/(float)cell_number));

  //Limit for draw
  line_length = height+y0;
  y = y0;

  //---- Draw horisontal lines
  for(int i=0; i<cell_number+1; i++)
  {
	Draw_Line(pCDC, line_width, x0, y, width+x0, y, color);
	
    //Print vertical scale digit
    //txt.Format("%.1f", txt_digit);
	txt.Format("%.0f", txt_digit);
    txt_digit += txt_step;

    //Text out    
    pCDC->TextOutA(x0-txt_x_offset, y+txt_y_offset, txt);
	y += step_y;
  } 

  //---- delete graphic objects
  font.DeleteObject();
}
