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
//
//------------------------------------------------------------------------------
void CDSPL::AddVertex(void)
{
  int x, y, find=0;
  float fl_y;

  //координаты на графике
  fl_y = RecalcMousePosition(mouse_x, mouse_y, x, y);
  int count = profile.size();

  //ищем индекс, куда вставлять новую точку  
  for(int index=0; index<count-1; index++)
  {
  	int x0 = (int)profile[index].x;
  	int x1 = (int)profile[index+1].x;
	
	//Compare coordinate X
	if(x0<=x && x<=x1)
	{
	  PointFLT p; 
	  find = 1;

	  //new point X
	  //сдвиг если точки совпадают
	  if(x==x0) p.x = x+3;
	  else if(x==x1) p.x = x-3;
	  else p.x = x;
	  
	  //new point Y
	  if(object_type==SELECT_HORIZLINE)
	   { 
		 //for case: horizontal line
		 p.y = profile[index].y;
	   }
	  else
	   { 
		 //for case: pamp line 
		 p.y = ProfileBuf[x];
	   }

	  //----------------------------------
	  //---- Add new point to array
	  //----------------------------------
	  std::vector<PointFLT>::iterator pos;
	  pos = profile.begin();
	  profile.insert(pos+index+1, p);

	  TRACE2("\nAdd Vertex At: X:%d Y:%.1f", p.x, p.y);
	  
	  //higliht vertex
	  object_selected = 1;
      object_index = index;	//тут походу надо +1
	  object_type = SELECT_DRAGPOINT;
	  break;
	}
  }
 
  if(find) ev_Redraw.SetEvent();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::DelVertex(void)
{
  if(object_selected!=1) return;
  if(object_type!=SELECT_DRAGPOINT) return;
  
  //----
  int index = object_index;
  int count = profile.size();
  if(index>count) return;

  std::vector<PointFLT>::iterator pos;
  pos = profile.begin()+index+1;
  int x	= profile[index].x;
  int y = profile[index].y;
  //удаляем
  profile.erase(pos);
  //чистим память
  std::vector<PointFLT>(profile).swap(profile);

  TRACE2("\nDelete Vertex At: X:%d Y:%d", x,y);
  object_selected = 0;
  object_type = -1;

  Draw_Profile();
  ev_Redraw.SetEvent();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::DelSegment(void)
{
  if(object_selected!=1) return;
  if(object_type!=SELECT_RAMP)
  {
    if(object_type!=SELECT_HORIZLINE) return;
  }

  //----
  int index = object_index;
  int count = profile.size();
  if(index>count) return;

  std::vector<PointFLT>::iterator pos;
  pos = profile.begin()+index+1;
  int x	= profile[index].x;
  int y = profile[index].y;
  //удаляем
  profile.erase(pos);
  //чистим память
  std::vector<PointFLT>(profile).swap(profile);

  TRACE2("\nDelete Segment At: X:%d Y:%d", x,y);
  object_selected = 0;
  object_type = -1;

  Draw_Profile();
  ev_Redraw.SetEvent();
}
