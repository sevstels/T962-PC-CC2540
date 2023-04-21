//==============================================================================
//File name:    "profile.cpp"
//Purpose:      Source File, CPage2 property page
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "cmd.h"
#include "profile-file.h"
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
// Function
//------------------------------------------------------------------------------
void CDSPL::Profile_DefaultIni(void)
{ /*
  //Main profile buffer clear 
  profile.clear();
  index_cycles.RemoveAll();
  PointFLT p, draw;
  int i, n, begin, add, index = 0, last_x = 0;
  float last_y;
  TRACE("\nAdd ");

  //=====================================
  //Add Initial keypoints 
  //=====================================
  int count = _countof(initial);
  for(i=0; i<count; i++)
  {
	p.x = initial[i].time ; 
	p.y = initial[i].param;
	profile.push_back(p);
	index++;
  }

  //last X,Y position
  last_x = p.x;
  last_y = p.y;

  //=====================================
  //Add Cycles keypoints
  //=====================================
  count = _countof(cycle);
  for(int n=0; n<7; n++)
  {
    //save keypoints 
    for(i=0; i<count; i++)
    {
	  p.x = cycle[i].time; 
	  p.y = cycle[i].param;

 	  //match point detector
	  if(p.x==0) 
	  {
		index_cycles.Add(index-1);
		TRACE1(" s%d", index-1);

		index_cycles.Add(index-1+count-1);
		TRACE1(" e%d", index-1+count-1);
	  }

	  //match point detector
	  if(p.y==last_y && p.x==0) 
	  {
		add = 0;
		index--;
	  }
	  else
	  {
		add = 1;
	  }
	  p.x += last_x;
	 
	  if(add)
	   { 
		   profile.push_back(p);
	   }
	  last_x = p.x;
	  last_y = p.y;

	  index++;
    }
  }
  
  //pMonitor->SetWindowTextA("Load default profile");
  TRACE("\nLoad default PCR profile");	*/
}

#define DOT_TEMPERATURE  0  
#define DOT_LED_SAMPLE   1
#define DOT_NO_EXIST    -1
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CDSPL::get_profile_point(int index, int &x, float &y)
{
  int count = profile.size();
  if(index>count) return DOT_NO_EXIST;

  //Считать точку
  int x_value = (int)profile[index].x;
  float y_value = (float)profile[index].y;
  
  //проверка типа точки
  int type;
  //проверка это температура 
  if(y_value<2000) type = DOT_TEMPERATURE; 
  //найден LED 
  else type = DOT_LED_SAMPLE; 

  x	= x_value;
  y	= y_value;
  
  //конец поиска 
  return type;
}

//------------------------------------------------------------------------------
//Draw Profile
//------------------------------------------------------------------------------
void CDSPL::Draw_Profile(void)
{ 
  int find_x[2], find_LED, dot_index[2], i;
  int x[2], y[2], zero=1, step, result;
  float fl_y[2], y_temp, fl_LED;
  int line_width;
  double value;
  int x_temp;

  //для обслуживания точек семплов
  CDWordArray xLED;
  CDWordArray yLED;
  CDWordArray indexLED;

  CRect rect;
  CPoint center;
  COLORREF color;
    
  int count = profile.size();  
  int countbuf = _countof(ProfileBuf);

  if(count<1) return;

  //clear display points curve buffer
  for(i=0; i<countbuf; i++){ProfileBuf[i] = 0;}
  
  //-----------------------------------------
  //нарисовать линии
  //-----------------------------------------
  int index = 0;
  for(;;)
  {	
	find_x[0] = 0; 
	find_x[1] = 0;
	find_LED = 0;
	
	int counter=0;
	step = 0;

	for(;;)
	{  
	  //----------------------------------
	  //проверка точки профиля
	  //----------------------------------
	  result = get_profile_point(index+step, x_temp, y_temp);
	  //TRACE1("Step: %d, ", index+step);
	  //TRACE2("%d, %.1f\n", x_temp, y_temp);

	  //если это точка температуры
	  if(result==DOT_TEMPERATURE)
	  {
	    //запоминаем точки сегмента температуры
	    x[counter] = x_temp, 
	    fl_y[counter] = y_temp;
		//запоминаем index точки
		dot_index[counter] = index+step;
	    //флаг - первая точка сегмента найдена
	    find_x[counter++] = 1;  
	  }
		
	  //если это точка семпла LED
	  if(result==DOT_LED_SAMPLE)
	  {
	    //прочитать значение установок из float семпла
	    unsigned short LED_RGBY = GetSampleSetup(y_temp); 
	    //запоминаем точку как int
		xLED.Add(x_temp);
        yLED.Add(LED_RGBY);
		//запоминаем index точки
		indexLED.Add(index+step);
	    //флаг - первая точка сегмента найдена
	    find_LED = 1;  
	  }	  
	  
	  if(find_x[0]==1 && find_x[1]==1)
	    { break;}
	  if(result==DOT_NO_EXIST) 
	    { break;}
	  
	  step++;
	}

    //==== обработчик сегмента ============================
	//проверка наличия координат сегмента
	if(find_x[0]==1 && find_x[1]==1)
	{
	  //------------------------------
	  //найдена линия сегмента, рисуем
	  //------------------------------
	  //для наклонных линий 
	  if(fl_y[0] != fl_y[1])
	   {
	     //Calc_LinePoints(x1, fl_y1, x2, fl_y2);
		 Calc_LinePointsT(x[0], fl_y[0], x[1], fl_y[1]);
		 ProfileLength = x[1];
	   }
	  //для горизонтальных линий
	  else
	   {
	     //заливаем значение функции
	     for(int i=x[0]; i<x[1]; i++)
	     {
	       ProfileBuf[i] = fl_y[0]; 
	       //TRACE2("   fl_y0: %.1f y1: %d", fl_y1, y1);
		 }
		 ProfileLength = x[1];
	   }

	   //-----
	   if(object_selected && object_index == dot_index[0] && 
		 (object_type == SELECT_HORIZLINE || object_type == SELECT_RAMP))
	   {
		  color = RGB(255,255,0);
	   }
	   else
	   {
		  color = RGB(150,150,150);
	   }
	   
	   int focused = index_selected.GetCount(); 
	   if(focused>0)
		{
		   for(int i=0; i<focused; i++)
		   {
			  int value = index_selected.GetAt(i);
			  if(value==index)
			  { 
				 color = RGB(255,255,0);
				 break;
			  }
		   }
	    } 

	   /*
	   TRACE2("\nDraw X1: %d  X2: %d", X1, X2);
	   TRACE2("   fl_y1: %.1f y1: %d", fl_y1, y1);
	   */

///	   Overflow_Correction(x[0], fl_y[0], countbuf);
///	   Overflow_Correction(x[1], fl_y[1], countbuf);

	   //Преобразовать в координаты на дисплее
	   Convert_ToDisplay_XY(y[0], x[0], fl_y[0]);
 	   Convert_ToDisplay_XY(y[1], x[1], fl_y[1]);

	   x[0] -= scroll_position; 
	   x[1] -= scroll_position; 
	   
	   //TRACE2("\nDraw x1: %d  X2: %d", x1, 0);
	   //TRACE2("\nDraw y1: %d  float: %.1f\n", y[1], fl_y[1]);

	   //Check line outside display scale
	   int outside = CheckOutsideScale(x[0], y[0], x[1], y[1]);
	   if(outside)
	   {
	     //---- Draw profile curves --------------
	     //Рисуем график на экране
	     Draw_Line(&IMG, 3, x[0], y[0], x[1], y[1], color);
	   }

	   Draw_DragPoints(dot_index[1]);

	   //Additional Line
	   //Draw_Line(&IMG, 1, x1, y1+5, x2, y2+4, RGB(255,0,0));
	   
	   //---- Draw Txt msg
	///   Draw_Temperature(index);
	/* Draw_Duration();
	   Draw_FocusRect();  */
	   //Draw_MaxPoint(&IMG, ProfileBuf, countbuf);
	   //Draw_SampleLED(indexLED, sample_LEDs);
	   
	   //---------------------------------------------------------------
	   //добавить в буфер точки LEDs если она были найдены на сегменте
	   //---------------------------------------------------------------
	   if(find_LED)
	   {
		 int led_count = xLED.GetCount();
		 for(int i=0; i<led_count; i++)
		 {
		   //Получить index и значение
		   int xpos = xLED.GetAt(i);
		   short leds = yLED.GetAt(i);
		   int indx = indexLED.GetAt(i);
		   //TRACE2("LEDs: x %d code: %d\n", xpos, code);
		   
		   //сначала нарисовать на графике!
		   //Draw_DragPoints(indx);
		   //float fp = Calc_LinePoint(x[0], fl_y1, x[1], fl_y2);
		   Draw_SampleLED(indx, leds, fl_y[0]);
		   
		   //физическое значение координаты X 
		   //сохраняем обратно в буфер как float
		   //это испортит у-координату кодом
		   ProfileBuf[xpos] = SetSampleSetup(leds);
		 }
	   } 
	   //-------------------------------------------------

	}
	//=== конец обработчика сегмента ============================

	index += step;
	
	//exit
	if(index>=count-1 && count>2)
	{ 
	  //if(controller_file_ready) 
	  break;
	}
 }
 
 //Draw_CycleArrow(&IMG);
}

//------------------------------------------------------------------------------
// Function
//------------------------------------------------------------------------------
void CDSPL::Load_Profile(int number)
{
 if(number==PCR_STANDARD) Profile_DefaultIni();
  //TRACE2("\nX:%d Y:%d", x, y);
}

//------------------------------------------------------------------------------
// Function
//------------------------------------------------------------------------------
void CDSPL::Save_Profile(const char *pFileName)
{
 //if(number==0)ProfileStandardIni();
 //TRACE2("\nX:%d Y:%d", x, y);
}
