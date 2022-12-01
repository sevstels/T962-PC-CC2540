//==============================================================================
//File name:    "ramp.cpp"
//Purpose:      Source File, CPage2 property page
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
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
//
//------------------------------------------------------------------------------
void CDSPL::ChangeRamp(int delta)
{
  //ограничение по сдвигу влево
  if(profile[object_index].x == 
	 profile[object_index+1].x-3 && delta==-1) return;
 
  int length = profile.size();
  //------------------------------------------------
  //пересчитать горизонтальные координаты для 
  for(int i=object_index+1; i<length; i++)
  {
     profile[i].x += delta;
  }
  //------------------------------------------------

  TRACE1("\nChange Ramp: %d", delta);
  ev_Redraw.SetEvent();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::DragRamp(int x, int y)
{
  int delta_x, delta_y, old_x, old_y;
  
  old_x = mouse_x;
  old_y = mouse_y;
  
  delta_x = x - old_x;
  delta_y = y - old_y;

  if(delta_x!=0)
  {
  	TRACE1("\nDrag ramp: %d", delta_x);
  	drag_mode = 1;
	ChangeRamp(delta_x);
  }
}

//------------------------------------------------------------------------------
//For horizontal lines
//------------------------------------------------------------------------------
void CDSPL::Hold_Ramp(void)
{  
  int x = object_index;

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::Draw_Ramp(int obj_index)
{
  int txt_height = 13;
  int txt_offset = 6;

  int x1 = profile[obj_index].x;
  int x2 = profile[obj_index+1].x; 
  float y1 = profile[obj_index].y;
  float y2 = profile[obj_index+1].y;

  int x = (x1+x2)>>1;
  int y;
  
  if(y2>y1){y =(int)(((y2-y1)/2)+y1);} 
  else {y =(int)(((y1-y2)/2)+y2);}
 
  //calc ramp
  float ramp = y2 - y1;
  ramp /= (x2-x1);

  //---- Txt msg  
  CString txt;
  if(ramp>0) txt.Format("+%.2f°/s",ramp);
  else txt.Format("%.2f°/s",ramp);
  Convert_ToDisplay_XY(y, x, y);

  if(object_selected && 
	 object_index == obj_index && 
	 object_type == SELECT_RAMP)
  {
	txt_height = 14;
    txt_offset -= 1;
    //Draw graph scale  ETO_OPAQUE
    Draw_StringAt(&IMG, x+txt_offset-scroll_position, y+(txt_height>>1), 
	             txt, txt_height,0, RGB(255,0,0), RGB(255,255,100));
  }
  else
  {
    //Draw graph scale with bgnd TRANSPARENT   +txt_offset
    Draw_StringAt(&IMG, x-scroll_position-16, y+(txt_height>>1), 
	             txt,txt_height,0, RGB(80,80,80), RGB(255,255,255));//  TRANSPARENT
  }

}