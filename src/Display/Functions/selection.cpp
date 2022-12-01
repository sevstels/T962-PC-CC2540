//==============================================================================
//File name:    ".cpp"
//Purpose:      Source File, CPage2 property page
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "display.h"

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
int CDSPL::Object_Detector(int x, int y)
{
  int xgr, ygr, selected = 0;
  int x1, x2;
  float y1, y2, fl_y; 

  RecalcMousePosition(x, y, x, y);
  
  x += scroll_position;

  //получить значение из шкалы
  fl_y = Convert_ToScaleY(y);

  //ѕолучить длину профил€ 
  int length = profile.size();

  for(int i=0; i<length-1; i++)
  {
	//limites by x  
	int obj_x0 = profile[i].x;
	int obj_x1 = profile[i+1].x;
	float obj_y0 = profile[i].y;
	float obj_y1 = profile[i+1].y;

	//---- 
	//if(obj_y0!=obj_y1) object_type = 2;
	//else object_type = 1;

	//TRACE2("\nObj X:%d Y:%d", obj_x, obj_y);
    //смотрим какой обьект может находитс€ в этом интервале по x
	 /*
	//============================================================
	//Sensor detector
	//============================================================
	//проверка на превышение пороговой величины
	unsigned short sensor = (unsigned short) profile[i].y;
	if(sensor>0xF000) 
	{
	  //decode parameters
	  //if(obj_y1+4 > y && y > obj_y1-4)
	  //{
	    //TRACE2("\nFind X:%d Y:%d", x, y);
	    TRACE("\nFind Sensor, Index: %d", i);
		//object_index = i;
	    //selected = SELECT_SENSOR;
	    break;
	  //}
	}	 */

	//============================================================
	//Drag point detector
	//============================================================
	//проверка +/- по x
	if(obj_x1-4 < x && x < obj_x1+4) 
	{
	  //проверка +/- по y
	  if(obj_y1+4 > fl_y && fl_y > obj_y1-4)
	  {
	    //TRACE2("\nFind X:%d Y:%d", x, y);
	    TRACE1("\nFind DragPoint, Index: %d", i);
		object_index = i;
		object_type = SELECT_DRAGPOINT;
	    selected = 1;
	    goto OD_exit;
	  }
	}
	//============================================================
	//Ramp Line detector
	//============================================================
	//если у координаты не равны - лини€ наклонена
	if(obj_y0 != obj_y1) 
	{
	  //если объект находитс€ между х1 и х2 
	  if(obj_x0<x && x<obj_x1)
	  {
		//рассчитать предел отклонени€ 
		int step;
		if (obj_y1>obj_y0) step = obj_y1-obj_y0;
		else step = obj_y0-obj_y1;
		step /= obj_x1-obj_x0;
		TRACE1("\nRamp Step: %d", step);
		
		//считаем отклонение +/-y от наклонной линии ramp
		int graph_y = ProfileBuf[x];
		if(graph_y+4+step>fl_y && fl_y>graph_y-4-step)
		{
	      TRACE1("\nFind Ramp, Index: %d", i);
		  object_index = i;
		  object_type = SELECT_RAMP;
	      selected = 1;
	      goto OD_exit;
		}
	  }
	}	
	//============================================================
	//Horizontal Line detector
	//============================================================
	if(obj_x0 < x && x < obj_x1 && obj_y0 == obj_y1) 
	{
	  //наклон вверх || наклон вниз
	  if(obj_y0<fl_y+5 && fl_y-5<obj_y1 || 
		 obj_y1<fl_y+5 && fl_y-5<obj_y0)
	  {
	    TRACE1("\nFind Line, Index: %d", i);
		object_index = i;
		object_type = SELECT_HORIZLINE;
	    selected = 1;
	    goto OD_exit;
	  }
	}
  }

OD_exit:

  if(selected>0)
  {
    //запомнить последний выбранный обьект
    object_index_old = object_index;
	object_selected = 1;
  }
  else
  {
	TRACE("\nNo selection");
	object_selected = 0;
	object_type = -1;
	object_index = -1;
	index_selected.RemoveAll();
	rect_selection.SetRect(0,0,0,0);
  }
  
  return selected;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::Select_Focused(int x, int y)
{
  index_selected.RemoveAll();
  
  //просмотр всех обьектов по горизонтали на выделенном интервале
  int x1 = rect_selection.left;
  int x2 = rect_selection.right;
  if(x1==x2) return;

  //ѕолучить длину профил€ 
  int length = profile.size();

  for(int i=0; i<length; i++)
  {
	int x = profile[i].x;
  	int y = profile[i].y;

	if(x1<=x && x<=x2)
	{
	 TRACE1("\nFind Selected, Index: %d", i);
	 index_selected.Add(i);
	}
  }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::Add_Focused(void)
{
  if(object_selected!=1) return;
  if(object_type==SELECT_DRAGPOINT)return;

  TRACE1("\nAdd Selected, Index: %d", object_index);
  index_selected.Add(object_index);
}
/*
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::Draw_FocusRect(int x, int y)
{
  RecalcMousePosition(x, y, x, y); 
  int x0, y0;
  RecalcMousePosition(mouse_Lclick_x, mouse_Lclick_y, x0, y0);
  
  rect_selection.left =	x0;
  rect_selection.bottom = y0;
  rect_selection.top =	y;
  rect_selection.right = x;
  
  Draw_Scales();
  Draw_Profile();
  Draw_Frame(&IMG, 1, x0, y0, x, y, RGB(100,100,0));
  DisplayShow();
}*/
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::Draw_FocusRect(void)
{ /*
  RecalcMousePosition(x, y, x, y); 
  int x0, y0;
  RecalcMousePosition(mouse_Lclick_x, mouse_Lclick_y, x0, y0);
  
  rect_selection.left =	x0;
  rect_selection.bottom = y0;
  rect_selection.top =	y;
  rect_selection.right = x;
 
  Draw_Frame(&IMG, 1, x0, y0, x, y, RGB(100,100,0)); */
}
