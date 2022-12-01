//==============================================================================
//File name:    "dsp.cpp"
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
//CPage1 constructor
//------------------------------------------------------------------------------
CDSPL::CDSPL()
{
  opened_bgnd = 0;
  opened_img = 0;
  scroll_index = 0;
  drag_mode = 0;
  pMonitor = NULL;

  //----
  display_offset_x = 0;
  display_offset_y = 0;
  display_width = 0; 
  display_height = 0;
  
  //----
  graph_x0 = 0; 
  graph_y0 = 0;
  
  //----
  scale_offset_x = 0; 
  scale_offset_y = 0;
  scale_width_x = 0; 
  scale_height_y = 0;
  ProfileLength = 0;

  //----
  pPrBuf = ProfileBuf;
  object_selected = 0;
  object_type = -1;
  object_index = -2;
  object_index_old = -1;
  object_multiselect = 0;
  pcr_profile = 0;
  profile.clear();
  index_selected.RemoveAll();

  //----
  mouse_Lclick_x = 0;
  mouse_Lclick_y = 0;
  mouse_Rclick_x = 0;
  mouse_Rclick_y = 0;
  map_mode = MM_ISOTROPIC;//MM_TEXT; //MM_ANISOTROPIC;

  int count = _countof(ProfileBuf);
  for(int i=0; i<count; i++){ProfileBuf[i] = 0;}
  count = _countof(ScreenBuf);
  for(int i=0; i<count; i++){ScreenBuf[i] = 0;}
}

//------------------------------------------------------------------------------
//CPage1 destructor
//------------------------------------------------------------------------------
CDSPL::~CDSPL()
{



}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
 void CDSPL::DisplayIni(HANDLE hHWnd, int x, int y, int height, int width)
{
  //----
  BOOL result;
  hWnd =(HWND) hHWnd;
  
  //---- Open CDC
  hDC = GetDC((HWND)hWnd);   
  pDC = CDC::FromHandle(hDC);
    
  //Set window size and position
  //left, top, right, bottom
  display_offset_x = x;   //offset left	 
  display_offset_y = y;   //offset top
  display_width = width;  //size X 
  display_height = height;//size Y

  //=====================================================
  //���������� ������������� ���� ��� ���������
  //=====================================================
  //�� ���� � � � ��������������� ���������� �����������
  int mm = pDC->GetMapMode();
  pDC->SetMapMode(map_mode);//MM_ISOTROPIC
  mm = pDC->GetMapMode();

  //������ ������� ���������� �������
  CSize we = pDC->GetWindowExt();
  pDC->SetWindowExt(width, height); 
  we = pDC->GetWindowExt(); 

  //������ ���������� ������� ������, ������ ����� ����� �� X
  CSize wp =pDC->GetViewportExt();
  pDC->SetViewportExt(width, -height);
  wp =pDC->GetViewportExt();

  //����� ������ ���������� ��������� ����
  CPoint cp = pDC->GetViewportOrg();
  pDC->SetViewportOrg(0, height);
  cp = pDC->GetViewportOrg();
                                    
  //====================================
  //Create BackGround
  //====================================
  opened_bgnd = 1;
  result = BGND.CreateCompatibleDC(pDC);
  ASSERT(result); if(result == false){opened_bgnd = 0; return;}
  result = bmp_BkGnd.CreateCompatibleBitmap(pDC,width,height);
  ASSERT(result); if(result == false){opened_bgnd= 0; return;}
  //Select object for draw
  BGND.SelectObject(&bmp_BkGnd);
  
  BGND.SetMapMode(map_mode);// MM_ISOTROPIC
  //����� ���� �������
  BGND.SetWindowExt(width, height); 
  //���������� ������ ������, ������ ����� ����� �� X
  BGND.SetViewportExt(width, -height);  
  //���������� ������ ���������	X,Y
  //Set zero point coordinate in the Graph space
  //����������� ������ ��������� � ����� ������ ���� ����
  BGND.SetViewportOrg(0, height);	

  //BGND.FillSolidRect(5, -5, 20, -30, RGB(0,0,255));
  //DSPL.Draw_SQR(&DSPL.BGND);
}

//------------------------------------------------------------------------------
//HDC -��� ������������� ��������� ������,����������� ���������� ������ �������
//CDC -��� �������� ������������ ����������
//------------------------------------------------------------------------------
void CDSPL::DisplayShow(void)
{ 
  //--------------------------------------------------------
  //Copy BackGround image to output buffer
  //--------------------------------------------------------
  if(opened_img==1)
  BGND.BitBlt(10, 10, graph_width+1, graph_height+1,  
                     &SCL,0,0, SRCCOPY);

  //if(opened_img==1)
  //-------------------------------------------------------
  //Copy Curves Graph image to output buffer
  //-------------------------------------------------------  
  BGND.BitBlt(10, 10, graph_width+1, graph_height+1,  
                     &IMG, 0, 0, SRCAND);// MERGECOPY

  //Copy final output BMP image buffer to Display
  //����������� �� �����
  if(opened_bgnd==1)
  pDC->BitBlt(display_offset_x, display_offset_y, 
              display_width, display_height, &BGND,0,0,SRCCOPY);			 
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
 void CDSPL::GraphIni(int offset_x, int offset_y, int width, int height)
{
  BOOL result;
  
  graph_offset_x = offset_x; 
  graph_offset_y = offset_y;
  graph_width = width;
  graph_height = height;

  //====================================
  //Create Curve surface 
  //====================================
  opened_img = 1;
  result = SCL.CreateCompatibleDC(pDC);
  ASSERT(result); if(result == false){opened_img = 0; return;}
  result = bmp_Scale.CreateCompatibleBitmap(pDC, width+1, height+2);
  ASSERT(result); if(result == false){opened_img = 0; return;}
  //Select object for draw
  SCL.SelectObject(&bmp_Scale);
    
  SCL.SetMapMode(map_mode); //MM_ISOTROPIC
  //����� ���� �������
  SCL.SetWindowExt(width+1, height+1); 
  //���������� ������ ������, ������ ����� ����� �� X
  SCL.SetViewportExt(width, -height);  
  //���������� ������ ���������	X,Y
  //Set zero point coordinate in the Graph space
  //����������� ������ ��������� � ����� ������ ���� ����
  SCL.SetViewportOrg(0, height);
      
  //Fill graph background
  SCL.FillSolidRect(0, -1, width+1, height+1, RGB(255,255,255));	
  /*
  //----
  Draw_Line(&IMG,1, 0,0, width, 0, RGB(255,0,0));
  Draw_Line(&IMG,1, 0,0, 0, height, RGB(255,0,0));  
  Draw_Line(&IMG,1, 0, height, width, height, RGB(255,0,0));
  Draw_Line(&IMG,1, width, 0, width, height, RGB(255,0,0));	  
  //----
  IMG.Rectangle(0,-1, width-2, height-2);
  */

  //====================================
  //Create Curve surface 
  //====================================
  opened_img = 1;
  result = IMG.CreateCompatibleDC(pDC);
  ASSERT(result); if(result == false){opened_img = 0; return;}
  result = bmp_Img.CreateCompatibleBitmap(pDC, width, height);
  ASSERT(result); if(result == false){opened_img = 0; return;}
  //Select object for draw
  IMG.SelectObject(&bmp_Img);
    
  IMG.SetMapMode(map_mode);//MM_ISOTROPIC
  //����� ���� �������
  IMG.SetWindowExt(width, height); 
  //���������� ������ ������, ������ ����� ����� �� X
  IMG.SetViewportExt(width, -height);  
  //���������� ������ ���������	X,Y
  //Set zero point coordinate in the Graph space
  //����������� ������ ��������� � ����� ������ ���� ����
  IMG.SetViewportOrg(0, height);
  IMG.FillSolidRect(0, 0, width, height, RGB(255,255,255));
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CDSPL::round(double value)
{
  return (int)floor(value+(double)0.5);
}

//------------------------------------------------------------------------------
//������������� � ���������� �� �������
//------------------------------------------------------------------------------
void CDSPL::Convert_ToDisplay_XY(int &y, int &x, float fl_y)
{
  int dspl_y;
  double value;

  //---- Y
  fl_y -= graph_y0;
  value = fl_y;
  value *= graph_factor_y;
	   
  dspl_y = (int)round(value);
  dspl_y += scale_offset_y;

  y = dspl_y;

  //---- X
  x += scale_offset_x;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CDSPL::Convert_ToDisplay_Y(float fl_y)
{
  //������������� � ���������� �� �������
  int dspl_y;
  double value;

  //---- Y
  fl_y -= graph_y0;
  value = graph_factor_y;
  value *= fl_y;
	   
  dspl_y = (int)round(value);
  if(dspl_y < graph_y0) dspl_y = graph_y0;
  dspl_y += scale_offset_y;
  return dspl_y;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
float CDSPL::Convert_ToScaleY(int y)
{
  double value;	
  
  //---- Y
  value = y;
  value /= graph_factor_y;
  value += graph_y0;
  
  return (float)value;
}

//------------------------------------------------------------------------------
//Set Y-Limit for curves
//------------------------------------------------------------------------------
void CDSPL::Overflow_Correction(int &x, float &y, int length)
{
  //---- Y Bottom  scale_offset_y
  if(y<scale_range_y0)
  {
    //�������� �������� �������
	for(int i=x+scroll_position; i<length; i++)
	{
	    if(ProfileBuf[i]>= scale_range_y0)
		{
		  x = i;
		  y = ProfileBuf[i];
		  return;
		}
	}
  }

  //---- Y Top
  if(y>(scale_height_y+scale_offset_y)) 
	 y = (float)(scale_height_y+scale_offset_y);
}

//------------------------------------------------------------------------------
//Set 
//------------------------------------------------------------------------------
int InterpolatorY(int delta_x, int delta_y)
{


  return 1;
}

//------------------------------------------------------------------------------
//Set 
//------------------------------------------------------------------------------ 
int CDSPL::CheckOutsideScale(int &x0, int &y0, int &x1, int &y1)
{
  //TRACE2("Draw x0: %d  x1: %d", x0, x1);
  //TRACE2("  y0: %d  y1: %d\n", y0, y1);  

  if(y0<scale_offset_y) y0 = scale_offset_y;
  if(y1<scale_offset_y) y1 = scale_offset_y;

  if(x0<scale_offset_x)
  {
	 x0 = scale_offset_x;
     y0 = y1;
	   /*
	 if(x1==x0)
	 {
	 
	 
	 
	 }
	 else
	 {
	   float step_y = (float)(y1-y0)/(float)(x1-x0);

	  // y0 = (int)(y1-(step_y*scroll_position));//(x1-x0)));
	   
	   //int delta = x1 - scroll_position; 
	   TRACE2("x0: %d  x1: %d  ", x0, x1);
	   TRACE2("y0: %d  step: %f\n", y0, step_y);
	   //TRACE2("x1: %d  delt: %d\n", x1, delta);
	 }
	 
	 //float step_y = (float)(y1-y0)/(float)(x1-x0);
	 //y0 = (y1-y0)/4 ;
	  */
  }
  
  if(x1<scale_offset_x)
  {
	 x1 = scale_offset_x;
  }

  //������ ������� �������
  if(x1>(scale_width_x+scale_offset_x))
  {
	x1 = scale_width_x+scale_offset_x;
	y1 = y0;
  }

  //���� ����� ����� �� ������� �������
  if(x0>=(scale_width_x+scale_offset_x))
  {
  	 return 0;
  }

  return 1;
}

  /*
  //---- graph
  int graph_offset_x, graph_offset_y;
  int graph_width, graph_height;
  int graph_x0, graph_y0;
  double graph_factor_x, graph_factor_y;

  //---- display 
  int display_offset_x, display_offset_y;
  int display_width, display_height;

  //---- scale
  int scale_offset_x, scale_offset_y;
  int scale_width_x, scale_height_y;
  int scale_range_x0, scale_range_x;
  int scale_range_y0, scale_range_y;
  int scale_cell_x, scale_cell_y;
  */