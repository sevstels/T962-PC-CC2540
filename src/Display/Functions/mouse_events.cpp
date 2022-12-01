//==============================================================================
//File name:    ".cpp"
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
//Handler
//------------------------------------------------------------------------------
float CDSPL::RecalcMousePosition(int ms_x, int ms_y, int &x, int &y)
{  /*
  x = ms_x - display_offset_x - scale_offset_x - graph_offset_x;
  y = display_height + display_offset_y - graph_offset_y - ms_y;
 */

  x = ms_x-14-28;
  y = 220-ms_y;

  TRACE2("\nMouse X:%d Y:%d, ", ms_x, ms_y);
  TRACE2("Display X:%d Y:%d, ", x, y);
  
  float fl_y = Convert_ToScaleY(y);
  TRACE2("Curve X:%d Y:%.1f", x, fl_y);
  return fl_y;
}

//------------------------------------------------------------------------------
//Function
//------------------------------------------------------------------------------
void CDSPL::MouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
 if(zDelta==0) return;

 int delta=0;
 if(zDelta>0) delta = 1;
 if(zDelta<0) delta = -1;
 TRACE1("\nMouse Wheel, delta: %d", delta);

 //изменение длительности
 if(object_type==SELECT_HORIZLINE && 
	object_selected==2)
   {ChangeDuration(delta);}

 //угловое перемещение, рамп
 if(object_type==SELECT_RAMP && 
	object_selected==1)
   {ChangeRamp(delta);}

 //перемещение по вертикали 
 if(object_type==SELECT_HORIZLINE && 
	object_selected==1)
   {ChangeTemperature(delta, 0);}

 //сдвиг картинки по горизонтали по циклам
 if(object_type==-1 &&  
	object_selected==0)
   {
	 int nSBCode;
	 if(delta>0) nSBCode = SB_LINELEFT;//SB_PAGELEFT;
	 else nSBCode = SB_LINERIGHT;//SB_PAGERIGHT;

	 Scroll_Handler(nSBCode, -1);
   }
}

//------------------------------------------------------------------------------
//Function
//------------------------------------------------------------------------------
void CDSPL::MouseMove(UINT nFlags, CPoint point)
{  
  //If the left mouse button is down
  if(nFlags==MK_LBUTTON && object_selected==1)
  {	 
	//перемещение по вертикали для горизонтальной линии
	if(object_type==SELECT_HORIZLINE)
	{
	   DragTemperature(point.x, point.y);
	   mouse_x = point.x;
       mouse_y = point.y;
	   return;
	}

    //перемещение по горизонтали для рамп, тянут за рамп
    if(object_type==SELECT_RAMP)
	{
       DragRamp(point.x, point.y);
	   mouse_x = point.x;
       mouse_y = point.y;
	   return;
	}

    //перемещение по горизонтали, тянут за DragPoint
    if(object_type==SELECT_DRAGPOINT)
	{
       DragDuration(point.x, point.y);
	   mouse_x = point.x;
       mouse_y = point.y;
	   return;
	}
  }

  //Рисуем фокус 
  if(nFlags==MK_LBUTTON && object_selected==0)
  {	
    Select_Focused(point.x, point.y);
	//Draw_FocusRect(point.x, point.y);
  } 

/*
  //Set if the SHIFT key is down
  if(nFlags==(MK_SHIFT && object_selected==1)
  {
  }	*/
  //Set if the right mouse button is down
  ///if(nFlags==MK_RBUTTON){}
  //Set if the CTRL key is down
  ///if(nFlags==MK_CONTROL){}

  mouse_x = point.x;
  mouse_y = point.y;
}

//------------------------------------------------------------------------------
//Function
//------------------------------------------------------------------------------
void CDSPL::RButtonDown(UINT nFlags, CPoint point)
{
  mouse_Rclick_x = point.x;
  mouse_Rclick_y = point.y;
}

//------------------------------------------------------------------------------
//Function
//------------------------------------------------------------------------------
void CDSPL::RButtonUp(UINT nFlags, CPoint point)
{  
 
}

//------------------------------------------------------------------------------
//Function
//------------------------------------------------------------------------------
void CDSPL::LButtonDown(UINT nFlags, CPoint point)
{ 
  mouse_Lclick_x = point.x;
  mouse_Lclick_y = point.y;

  int selected = Object_Detector(point.x, point.y);
  //Set if the control key is down
  if(nFlags==(MK_CONTROL|MK_LBUTTON) && object_selected==1)
  {
    Add_Focused();
  }
	  
  ev_Redraw.SetEvent(); 
}

//------------------------------------------------------------------------------
//Function
//------------------------------------------------------------------------------
void CDSPL::LButtonUp(UINT nFlags, CPoint point)
{
 
}
