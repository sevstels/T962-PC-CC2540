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
//изменить длину обьекта
//------------------------------------------------------------------------------
void CDSPL::ChangeDuration(int delta)
{
  //ограничение по сдвигу влево и превращению обьекта в отрицательный   
  if(profile[object_index].x >= 
	 profile[object_index+1].x-3 && delta<0) return;
  
  //------------------------------------------------
  //пересчитать горизонтальные координаты для
  //всех последующих обьектов
  int length = profile.size();
  for(int i=object_index+1; i<length; i++)
  {
     profile[i].x += delta;
  }
  //------------------------------------------------
  
  TRACE1("\nChange Duration: %d", delta);
  
  ev_Redraw.SetEvent();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::DragDuration(int x, int y)
{
  int delta, old_x, old_y, new_x, new_y;
  
  //старые координаты
  RecalcMousePosition(mouse_x, mouse_y, old_x, old_y);
  
  //новые координаты
  RecalcMousePosition(x, y, new_x, new_y);

  delta = new_x - old_x;
  
  if(delta!=0)
  {
  	TRACE1("\nDrag duration: %d", delta);
  	drag_mode = 1;
	ChangeDuration(delta);
  }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::Draw_Duration(int index)
{
  /*
  Draw_Profile();

  //coordinates for txt output 
  int y1 = profile[object_index].y;
  int x1 = profile[object_index].x;
  int x2 = profile[object_index+1].x;
  int x = (x1+x2)>>1;
  int duratiion = x2-x1;

  CString txt; txt.Format("%d Sec",duratiion);

  Draw_StringAt(&IMG, x-7, y1+20, txt, 14, RGB(200,0,0));
  */
}