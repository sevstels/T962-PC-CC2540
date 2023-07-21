//==============================================================================
//File name:    "display.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
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
  //Подготовка существующего окна под рисование
  //=====================================================
  //по осям х и у устанавливается одинаковая размерность
  int mm = pDC->GetMapMode();
  pDC->SetMapMode(map_mode);//MM_ISOTROPIC
  mm = pDC->GetMapMode();

  //задаем размеры логической области
  CSize we = pDC->GetWindowExt();
  pDC->SetWindowExt(width, height); 
  we = pDC->GetWindowExt(); 

  //размер физической области вывода, график растёт вверх по X
  CSize wp =pDC->GetViewportExt();
  pDC->SetViewportExt(width, -height);
  wp =pDC->GetViewportExt();

  //задаём начало физических координат окна
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
  //Длина осей графика
  BGND.SetWindowExt(width, height); 
  //Определяем облась вывода, график растёт вверх по X
  BGND.SetViewportExt(width, -height);  
  //Установить начало координат	X,Y
  //Set zero point coordinate in the Graph space
  //расположить начало координат в левом нижнем углу окна
  BGND.SetViewportOrg(0, height);	

  //BGND.FillSolidRect(5, -5, 20, -30, RGB(0,0,255));
  //DSPL.Draw_SQR(&DSPL.BGND);
}

//------------------------------------------------------------------------------
//HDC -это идентификатор структуры данных,описывающих устройство вывода графики
//CDC -это контекст графического устройства
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
  //скопировать на экран
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
  //Длина осей графика
  SCL.SetWindowExt(width+1, height+1); 
  //Определяем облась вывода, график растёт вверх по X
  SCL.SetViewportExt(width, -height);  
  //Установить начало координат	X,Y
  //Set zero point coordinate in the Graph space
  //расположить начало координат в левом нижнем углу окна
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
  //Длина осей графика
  IMG.SetWindowExt(width, height); 
  //Определяем облась вывода, график растёт вверх по X
  IMG.SetViewportExt(width, -height);  
  //Установить начало координат	X,Y
  //Set zero point coordinate in the Graph space
  //расположить начало координат в левом нижнем углу окна
  IMG.SetViewportOrg(0, height);
  IMG.FillSolidRect(0, 0, width, height, RGB(255,255,255));
  //отрезать всё лишнее за пределами шкалы
  IMG.IntersectClipRect(offset_x, offset_y, 
	                    offset_x + scale_width_x, 
						offset_y + scale_height_y);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CDSPL::round(double value)
{
  return (int)floor(value+(double)0.5);
}

//------------------------------------------------------------------------------
//Преобразовать в координаты на дисплее
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
  //Преобразовать в координаты на дисплее
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
//Set 
//------------------------------------------------------------------------------
int InterpolatorY(int delta_x, int delta_y)
{
  return 1;
}
