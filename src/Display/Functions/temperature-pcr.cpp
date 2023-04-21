//==============================================================================
//File name:    "temperature.cpp"
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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//------------------------------------------------------------------------------
// Calculate new X position for neighbouring point on the left
//------------------------------------------------------------------------------
float CDSPL::CalcLeftX(float x0, float x1, float y0, float y1, float fl_delta)
{
    float A0 = y1 - y0;
    float B0 = x1 - x0;
    // Ratio of horizontal / vertical
    float ratio = ((float)B0) / (float)A0;
    // Find shift in X using the ratio
    float dx;
    // If heating/cooling is reversed, account for shift
    if ((fl_delta < y0 && y1 > y0) || (fl_delta > y0 && y1 < y0)) {
        dx = (2 * y0 - y1 - fl_delta) * ratio;
    }
    else {
        // Normal
        dx = ((fl_delta - y1) * ratio);
    }
    return dx;
}

//------------------------------------------------------------------------------
// Calculate new X position for neighbouring point on the right
//------------------------------------------------------------------------------
float CDSPL::CalcRightX(float x2, float x3, float y2, float y3, float fl_delta)
{
    float A0 = y3 - y2;
    float B0 = x3 - x2;
    // Ratio of horizontal / vertical
    float ratio = ((float)B0) / (float)A0;
    // Find shift in X using the ratio
    float dx;
    // If heating/cooling is reversed, reverse shift also
    if ((fl_delta < y3 && y2 > y3) || (fl_delta > y3 && y2 < y3)) {
        dx = (y2 + fl_delta - 2*y3) * ratio;
    }
    else {
        // Normal
        dx = ((y2 - fl_delta) * ratio);
    }
    return dx;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::ChangeTemperature(float fl_delta, int mode)
{ 
  float x0, x1, x2, x3;
  float y0, y1, y2, y3;

  x0 = profile[object_index-1].x; // Point left of selected line
  x1 = profile[object_index].x;  // 1st point of selected line
  x2 = profile[object_index + 1].x;  // 2nd point of selected line
  x3 = profile[object_index + 2].x;  // Point right of selected line

  y0 = profile[object_index-1].y;  // Point left of selected line
  y1 = profile[object_index].y;  // 1st point of selected line
  y2 = profile[object_index + 1].y; // 2nd point of selected line
  y3 = profile[object_index + 2].y; // Point right of selected line
  
  //float C0 = sqrt(A0*A0+B0*B0);
  //считаем старый угол
  //float angl0 = atan(tang0) * 180.0 / M_PI;

 if(mode==0)
  {
    //Step for mouse wheel scrolling mode 
	 if (fl_delta > 0) {
		 fl_delta = y1 + 0.1;
	 }
	 else {
		 fl_delta = y1 - 0.1;
	 }
	//Change
	//profile[object_index].y += fl_delta;
    //profile[object_index+1].y += fl_delta;
  }

	 // Define threshold for switching between heating/cooling
	 const float threshold = 0.5;
	 // Check if new position is too close to a straight line
	 if (fabs((double)fl_delta - y0) < threshold || fabs((double)fl_delta - y3) < threshold)
	 {
		 // Calculate new X-positions for left and right points
		 float leftx, rightx, newy;
		 // Check which line is closest
		 if (fabs((double)fl_delta - y0) < fabs((double)fl_delta - y3))
		 {
			 // If selection is closer to left line
			 if (y1 > y0) {
				 newy = y0 - threshold;
			 }
			 else {
				 newy = y0 + threshold;
			 }
		 }
		 else
		 {
			 // If selection is closer to right line
			 if (y2 > y3) {
				 newy = y3 - threshold;
			 }
			 else {
				 newy = y3 + threshold;
			 }
		 }
		 leftx = CalcLeftX(x0, x1, y0, y1, newy);  // Calc left point dX
		 rightx = CalcRightX(x2, x3, y2, y3, newy);  // Calc right point dX
		 profile[object_index].y = newy;
		 profile[object_index + 1].y = newy;
		 // Shift all affected points to the side
		 for (UINT i = object_index; i < profile.size(); i++) {
			 profile[i].x += leftx;
		 }
		 for (int i = object_index + 2; i < profile.size(); i++) {
			 profile[i].x += rightx;
		 }
	 }
	 else
	 {
		 // Calculate new X-positions for left and right points
		 float leftx = CalcLeftX(x0, x1, y0, y1, fl_delta);  // Calc left point dX
		 float rightx = CalcRightX(x2, x3, y2, y3, fl_delta);  // Calc right point dX
		 // Adjust vertical height of selected line
		 profile[object_index].y = fl_delta;
		 profile[object_index + 1].y = fl_delta;
		 // Shift all affected points to the side
		 for (int i = object_index; i < profile.size(); i++) {
			 profile[i].x += leftx;
		 }
		 for (int i = object_index + 2; i < profile.size(); i++) {
			 profile[i].x += rightx;
		 }
	 }
  	  /*
  //---- Hold Ramp ------------------------------
  int trg_x0 = profile[object_index-1].x;
  int trg_x1 = profile[object_index].x;
  float trg_y0 = profile[object_index-1].y;
  float trg_y1 = profile[object_index].y;

  //считаем размеры треугольника
  int a = trg_x1-trg_x0;
  float b = trg_y1 - trg_y0;
  */

  //считаем размеры нового треугольника
  //float A1 = profile[object_index].y-profile[object_index-1].y;//
  //float B1 = A1/tang;


  TRACE1("\nChange Temperature: %.2f", fl_delta);
  ev_Redraw.SetEvent(); 
}

//------------------------------------------------------------------------------
//Vertical drag
//------------------------------------------------------------------------------
void CDSPL::DragTemperature(int x, int y)
{ 
  int delta_x, delta_y, old_x, old_y;
  float fl_y;

  old_x = mouse_x;
  old_y = mouse_y;
  
  delta_x = x - old_x;
  delta_y = y - old_y;
    
  if(delta_y!=0)
  {
    //взять текущий Y дисплея
    //Covert_ToScalePos
    fl_y = RecalcMousePosition(x, y, old_x, old_y);

    TRACE1("\nDrag temperature: %.2f", fl_y);
    drag_mode = 1;
    ChangeTemperature(fl_y, 1);
  }
}

//------------------------------------------------------------------------------
//For horizontal lines
//------------------------------------------------------------------------------
void CDSPL::Draw_Temperature(int obj_index)
{  
  int x = 0;
  int y = 0;
  float fl_y = 0;
  int txt_height = 12;
  int txt_offset = 6;

  int x1 = profile[obj_index].x;
  int x2 = profile[obj_index+1].x;
  float y1 = profile[obj_index].y;
  float y2 = profile[obj_index+1].y;
  
  //----
  if(y1!=y2)
  {
    //Case for Ramp
    Draw_Ramp(obj_index);
    return;
  }
  else
  {
    //Case for horizontal line
    x = x2;
    y = (int)y2;
    fl_y = y2;
    txt_offset = 6;
  }

  //Txt msg  
  CString txt;
  txt.Format("%.1f°C",fl_y);
  Convert_ToDisplay_XY(y, x, y);

  if(object_selected && object_index == obj_index && 
    (object_type == SELECT_HORIZLINE || 
     object_type == SELECT_RAMP))
  {
    txt_height = 14;
    txt_offset -= 1;
    //Draw graph scale
    Draw_StringAt(&IMG, x+txt_offset-scroll_position, y+(txt_height>>1), 
                   txt, txt_height,0, RGB(255,0,0), RGB(255,255,100));
  }
  else
  {
    //Draw graph scale with bgnd TRANSPARENT
    Draw_StringAt(&IMG, x+txt_offset-scroll_position, y+(txt_height>>1), 
                   txt, txt_height,0, RGB(80,80,80), TRANSPARENT);
  }
}
