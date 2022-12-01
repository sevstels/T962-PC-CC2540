//==============================================================================
//File name:   "graphs.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:   (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "graphs.h"
#include <math.h>
#include "cmd.h"

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
//constructor
//------------------------------------------------------------------------------
CGraphs::CGraphs()
{   	
  last_file.empty();
  file_number = 0;
  normalize = 0;
  curve_height = 0;
  curve_length = 0;
  curve_length = 0;
  resize_factor_x = 1;
  resize_factor_y = 1;
  hDC = NULL;
  pBuf = Buf;
  pBuf2 = Buf2;
  //curve_length1 = 0;
  curve_length2 = 0;
}

//------------------------------------------------------------------------------
//destructor
//------------------------------------------------------------------------------
CGraphs::~CGraphs()
{

}

//------------------------------------------------------------------------------
//HDC -это идентификатор структуры данных,описывающих устройство вывода графики
//CDC -это контекст графического устройства
//------------------------------------------------------------------------------
void CGraphs::SetHWnd(HANDLE hHWnd){hWnd = hHWnd;}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CGraphs::SetRangeX(int from, int to)
{
  range_x0 = from;
  range_xn = to;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CGraphs::SetRangeY(int from, int to)
{
  range_y0 = from; 
  range_yn = to;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CGraphs::SetMulX(double factor_x)
{
  resize_factor_x = factor_x;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CGraphs::SetMulY(double factor_y)
{
  resize_factor_y = factor_y;
}
//------------------------------------------------------------------------------
//For draw Line
//------------------------------------------------------------------------------
void CGraphs::Calc_LinePoints(short *pBuf, int x1, int y1, int x2, int y2)
{
 const int deltaX = abs(x2 - x1);
 const int deltaY = abs(y2 - y1);
 const int signX = x1 < x2 ? 1 : -1;
 const int signY = y1 < y2 ? 1 : -1;
 
 //----
 int error = deltaX - ((int)deltaY);
 //
 pBuf[x2] = y2;
 while(x1 != x2 || y1 != y2) 
 {
   pBuf[x1] = y1;
   const int error2 = error * 2;
   //
   if(error2 > -deltaY) 
   {
     error -= deltaY;
     x1 += signX;
   }
   if(error2 < deltaX) 
   {
     error += deltaX;
     y1 += signY;
   }
 }
}

extern unsigned short GetSampleSetup(float code);
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CGraphs::Parser(std::vector<Point2D> *pProfile)
{
  int count = pProfile->size();
  int last_x = (int) pProfile->at(count-1).x;
  curve_length = last_x;
  int y_old = 0;
  int y1, y2, led = 0;
  //
  memset(pBuf, 0, sizeof(Buf));
  
  TRACE("Processing PCR\n");

  for(int i=0; i<count-1; i++)
  {
    //---- x1 first point
    int x1 = (int)(pProfile->at(i).x*resize_factor_x);
    float fl_y = pProfile->at(i).y;
	//проверка LED    
    if(fl_y<2000){y1 = (int)(resize_factor_y*fl_y); y_old = y1;}
	else y1 = y_old; //{y1 = 1000+GetSampleSetup(fl_y); led = 1;}
	   
    //---- x2
    int x2 = (int)(pProfile->at(i+1).x*resize_factor_x);
    fl_y = pProfile->at(i+1).y;
    
	//проверка LED    
	if(fl_y<2000){y2 = (int)(resize_factor_y*fl_y); y_old = y2;}
	else  y2 = y_old; //{y2 = 1000+GetSampleSetup(fl_y); led = 1;}

	TRACE2("x1 %d y1 %d , ", x1, y1);
	TRACE2("x2 %d y2 %d\n", x2, y2);

    if(y1 != y2)
     {
       //for ramp lines
      Calc_LinePoints(Buf, x1, y1, x2, y2);
     }
    else
     {
       //for horisontal lines
       //заливаем значение функции
       for(int n=x1+1; n<x2; n++)
        {
           pBuf[n] = y1;
        }
     }
  }

  return 1;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CGraphs::ParserDots(std::vector<Point2D> *pData)
{
  int count = pData->size();
  if(count<1) return 0;
  int last_x = (int) pData->at(count-1).x;
  curve_length2 = last_x*resize_factor_x;
	
  //
  memset(pBuf2, 0, sizeof(Buf2));
  
  TRACE("Processing TEC\n");

  for(int i=0; i<count-1; i++)
  {
    //---- x1 first point
    int x1 = (int)(pData->at(i).x*resize_factor_x);
    double dbl_y = (float)pData->at(i).y;
    int y1 = (int)(resize_factor_y*dbl_y);

    //---- x2
    int x2 = (int)(pData->at(i+1).x*resize_factor_x);
    dbl_y = (float)pData->at(i+1).y;
    int y2 = (int)(resize_factor_y*dbl_y);

	TRACE2("x1 %d y1 %d , ", x1, y1);
	TRACE2("x2 %d y2 %d\n", x2, y2);

    if(y1 != y2)
     {
       //for ramp lines
       Calc_LinePoints(Buf2, x1, y1, x2, y2);
     }
    else
     {
       //for horisontal lines
       //заливаем значение функции
       for(int n=x1+1; n<x2; n++)
        {
           pBuf2[n] = y1;
        }
     }
  }

  return 1;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CGraphs::CreateGraph(int height, int width, const char *pFileName)
{
  int i,y;
  double data;
  short *pPic = NULL;
  std::string file = pFileName; 

  //Gaps
  int left_gap = 32, 
	  right_gap = 16, 
	  top_gap = 16, 
	  bottom_gap = 32;
  
  //If NO init hWnd
  ASSERT(hWnd!=NULL);
  //Open CDC
  HDC hDC = GetDC((HWND)hWnd);   
  CDC *pDC = CDC::FromHandle(hDC);

  //---
  if(width==GRPH_WIDTH_AUTO) width = (int)curve_length*resize_factor_x;
  
  //allocate memory for temp local buffer 
  pPic = new short[width];
  if(pPic==NULL) return -1;
  memcpy(pPic, pBuf, width<<1);

  //====================================================================
  //Save signal to text *.h file + signal inversion
  //====================================================================
  //Bin_to_Txt(ADC_Buffer, size_x, 2);
  CBitmap mem_BMP;
  CDC PIC;

  PIC.CreateCompatibleDC(pDC);
  mem_BMP.CreateCompatibleBitmap(pDC, width+left_gap+right_gap, 
                                     height+top_gap+bottom_gap);
  PIC.SelectObject(&mem_BMP);

  //Fill background
  PIC.FillSolidRect(0,0, width+left_gap+right_gap, 
                         height+top_gap+bottom_gap, 
						 RGB(255,255,255));// RGB(255,255,255)

  //PIC.SetBkColor(RGB(255,255,255));
  //PIC.SetDCBrushColor(RGB(255,255,255))

  //логические единицы отображаем, как физические
  //Set 1 pixel X = 1 pixel Y = MM_ISOTROPIC 
  int oldMapMode = PIC.SetMapMode(MM_ISOTROPIC); // MM_LOMETRIC
  //----
  int oldBkMode = PIC.SetBkMode(TRANSPARENT);
  //----  
  //int oldTextColor = PIC.SetTextColor(RGB(255,0,0));

  //Длина осей графика
  PIC.SetWindowExt(width, height); 
  
  //Определяем облась вывода, график растёт вверх по X
  PIC.SetViewportExt(width, -1*height);  
  
  //Установить начало координат	X,Y
  //Set zero point coordinate in the Graph space
  PIC.SetViewportOrg(left_gap, height+top_gap); 
  
  //Draw curve
  Draw_Curve(&PIC, pPic, 3, 0, 0, 0, width-1, RGB(120,160,120));	   
  
  //Draw graph scale
  Draw_ScaleY(&PIC, 0,0, width, height, 12, RGB(60,60,60));//RGB(180,180,180)
  Draw_ScaleX(&PIC, 0,0, width, height, 10, RGB(60,60,60));//RGB(180,180,180)
  
  //Sin graph test
  //sin_wave((char*)pPic, 60, 60, 2000);
  //test_Func2((char*)pPic, 60, 60, 400);
  
  //Draw Additional curves  
  //if(curve_length1 > width)curve_length1 = width;
  //if(curve_length2 > width)curve_length2 = width;
  
  Draw_Curve(&PIC, Buf2, 3, 0, 0, 0, curve_length2-1, RGB(255,0,0));
  
  //if(pCurveBuf2!=NULL)Draw_Curve(&PIC, pCurveBuf2, 2, 0, 0, 0, 
	//                  curve_length2-1, RGB(0,0,255));

 /* 
  //Draw Limits lines
  y = 70*h_scale; 
  IMG.DrawLine(&PIC,1,0,y,width,y, RGB_YELLOW);

  y = 50*h_scale;
  IMG.DrawLine(&PIC,1,0,y,width,y, RGB_BLUE);
 */

  //Restore old setup
  PIC.SetMapMode(oldMapMode);
  PIC.SetBkMode(oldBkMode);
  //PIC.SetTextColor(oldTextColor);

  //Release buffer
  if(pPic!=NULL){delete [] pPic; pPic = NULL;}
  
  //Release DC
  ::ReleaseDC((HWND)hWnd, hDC);

  file += "_";
  file += std::to_string((long long)file_number);
  file += ".png";
  last_file = file;

  //Save image file
  CImage image;     
  image.Attach(mem_BMP);    
  image.Save(_T(file.c_str()), Gdiplus::ImageFormatPNG); 
  
  //Release memory
  image.Detach();
  mem_BMP.DeleteObject();
  PIC.DeleteDC();
  
  file_number++;
  
  return 1; 
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CGraphs::ShowGraph(const char *pFileName)
{
  HINSTANCE Status = 0;
  std::string msg, file;
  //int length = 0;

  if(pFileName!=NULL){file = pFileName;}
  else file = last_file;
/*
  if(length==0)
  else file = pFileName;
*/	
  ///current_filepath = folder_path[0]; 
  //file_name = last_picture; //"Capture_x.png";
  ///current_filename = "controller_1.png";
  ///current_filepath += current_filename;

  //------------------------------------
  //launch default picture browser
  //------------------------------------
  ///if (current_filename.Find(_T(".png"))!=-1)
  ///{
  //launch picture browser exe
  Status = ShellExecuteA(NULL, "open", file.c_str(), 
                         NULL, NULL, SW_SHOW);
  //successful
  if((int)Status > 32) return 1;

  //error case
  switch((int)Status)
  {
    case ERROR_NOT_READY:
    msg = "The specified file: ../";
    msg += file;
    msg += " was not found";
    break;
        
    case ERROR_FILE_NOT_FOUND:
    msg = "The specified file: ../";
    msg += file;
    msg += " was not found";
    break;

    case ERROR_PATH_NOT_FOUND:
    msg = "The specified path: ../";
    msg += file;
    msg += " was not found";
    break;
        
    default:
    msg = "File type not supported";
  }

  //show error message
  MessageBoxA((HWND)hWnd, (LPCSTR)msg.c_str(),"Error", MB_OK);
  return -1;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CGraphs::Normalization(void)
{
  int min_x, max_x, min_y, max_y;

  if(curve_length==0) return;
  int find_min = find_Min(pBuf, 0, curve_length, min_x, min_y);
  int find_max = find_Max(pBuf, 0, curve_length, max_x, max_y);
  
  if(find_min==1 && find_max==1)
  {
    //calc resize_factor
    resize_factor_y = max_y-min_y;
    resize_factor_y /= curve_height;
    normalize = 1;
  }
  //====================================================================
  //Calc  
  //====================================================================
  /*
  if(resize_y==1)
  {
    //copy from Extern data Bufer to local buffer for normalisation
    for(i=0; i<width; i++)
     { 
       data = (double)pPic[i];
       data *= resize_factor_y;      
       pPic[i] = (short)data; 
     }
  }*/

  /*
  if(normalize>0)
  {
    //copy from Extern data Bufer to local buffer for normalisation
    for(i=0; i<width; i++)
     { 
       data = (double)pPic[i];
       data /= resize_factor_y;      
       pPic[i] = (short)data; 
     }
    normalize = 0;
  }*/ 
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CGraphs::find_Max(signed short *pBuff, int from, int to, int &x, int &y)
{
  int i;
  //int edge = 127+16;
  int result = -1;

  pBuff += from; 
  
  //short minmax = edge;
  signed short minmax = *pBuff;
  signed short first_val = minmax;

  pBuff++;

  //check array	
  for (i = from+1; i<to; i++)
  {
      if(*pBuff > minmax)
       {
           minmax = *pBuff;
           x = i;
           y = minmax;
           result = 1;
       }
      pBuff++;
  }	

  //for case if max not found in array, set as max first point
  if(result<0)
  {
    x = 0;
    y = first_val;
    result = 1;
  }

  return result;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CGraphs::find_Min(signed short *pBuff, int from, int to, int &x, int &y)
{
  int i;
  //int edge = 127-16;
  int result =-1;
  
  pBuff += from; 
  
  //short minmax = edge;
  signed short minmax = *pBuff;
  signed short first_val = minmax;
  pBuff++;
    
  for (i = from+1; i<to; i++)
  {
      if(*pBuff < minmax)
       {
           minmax = *pBuff;
           x = i;
           y = minmax;
           result = 1;
       }
      pBuff++;
  }	

  //for case if min not found in array, set as min first point
  if(result<0)
  {
    x = 0;
    y = first_val;
    result = 1;
  }

  return result;
}

//------------------------------------------------------------------------------
//Draw Horizontal lines by X-direction
//------------------------------------------------------------------------------
void CGraphs::Draw_ScaleX(CDC *pCDC, int x0, int y0, int width, int height,
                            int line_number, COLORREF color)
{ 
  CString txt; 
  char line_width = 1;
  int step_x;
  int txt_step;
  int txt_digit = 0;
  int txt_x_offset = 2;
  int txt_y_offset = -6;
  				
  //------------------------------------------
  //Set parameters for Text output
  //------------------------------------------
  //set font
  CFont font;
  font.CreateFont(11,0,0,0,FW_BOLD,0,0,0,RUSSIAN_CHARSET,
                OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
                PROOF_QUALITY,VARIABLE_PITCH | FF_DONTCARE,"Arial");

  pCDC->SelectObject(&font);
  pCDC->SetTextColor(RGB(0,0,0));

  if(range_xn==GRPH_WIDTH_AUTO)
  { 
	range_xn = curve_length;
  }

  //Calc horisontal pixel steps 
  int range_x = range_xn-range_x0; 
  int xb = (int)(range_x0*resize_factor_x);
  int xe = (int)(range_xn*resize_factor_x);
  step_x = (int)((range_x/line_number)*resize_factor_x);
  
  txt_step = (int)(range_x/line_number);
  txt_digit = range_x0;  
  

  //Draw vertical lines by X-step
  //step_x = (int)(width/50);
  
  step_x = 100;
  txt_digit = range_x0;
  txt_step = 10;

  for (int x=0; x <=xe-xb; x+=step_x) 
  {
    Draw_Line(pCDC, line_width, x, y0, x, height+y0, color);

    //Print horizontal scale digit
    txt.Format("%d", txt_digit);
    txt_digit += txt_step;

    if(txt_digit>=10)txt_x_offset=4;
    if(txt_digit>=100)txt_x_offset=6;
    if(txt_digit>=1000)txt_x_offset=10;
    //Text out
    pCDC->TextOutA(x-txt_x_offset, y0+txt_y_offset, txt);	
  }
 
  //---- delete graphic objects
  font.DeleteObject();	
}

//------------------------------------------------------------------------------
//Draw Vertical lines by X-direction
//------------------------------------------------------------------------------
void CGraphs::Draw_ScaleY(CDC *pCDC, int x0, int y0, int width, int height, 
                            int line_number, COLORREF color)
{ 
  CString txt;
  int line_width = 1;

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
  int range_y = range_yn-range_y0; 
  
  int step_y;
  int txt_step;
  int txt_digit = 0;
  int txt_x_offset = 18;
  int txt_y_offset = 6;
  
  int yb = (int)(range_y0*resize_factor_y);
  int ye = (int)(range_yn*resize_factor_y);
  step_y = (int)((range_y/line_number)*resize_factor_y);
  
  txt_step = (int)(range_y/line_number);
  txt_digit = range_y0;

  for(int y=0; y<=ye-yb; y+= step_y)
  {
    Draw_Line(pCDC, line_width, x0, y, width, y, color);

    //Print vertical scale digit
    txt.Format("%d", txt_digit);
    txt_digit += txt_step;

    //Text out    
    pCDC->TextOutA(x0-txt_x_offset, y+txt_y_offset, "ase");
  } 

  //---- delete graphic objects
  font.DeleteObject();
}

//------------------------------------------------------------------------------
//Draw line
//------------------------------------------------------------------------------
void CGraphs::Draw_Line(CDC *pDC, char width, int x0, int y0, 
                          int x, int y, COLORREF color)
{
 //---- Set parameters for Pancil
 CPen Pen; 
 //New pen
 Pen.CreatePen(PS_SOLID, width, color);   

 //Save first pen pointer, select new pancil
 CPen *pOldPen = pDC->SelectObject(&Pen); 
 
 //Jump to start point 
 pDC->MoveTo(x0, y0);

 //Draw curve line
 pDC->LineTo(x, y);
 
 //----- delete all Pen
 //Set old Pen to DC 
 pDC->SelectObject(pOldPen);
 Pen.DeleteObject();
}

//------------------------------------------------------------------------------
//Paint	curve for char Array
//------------------------------------------------------------------------------
void CGraphs::Draw_Curve(CDC *pDC, short *pBuf, char line_width, 
                           int x0, int y0, int x_from, int x_to, COLORREF color)
{
  //---- Set parameters for Pancil
  CPen pen;
  //New pen
  pen.CreatePen(PS_SOLID, line_width, color); 

  //Save first pen pointer
  //select pancil
  CPen *pOldPen = pDC->SelectObject(&pen); 
 
  //Start point
  int x, y, lim;
  //offset
  int offset_y = (int)(range_y0*resize_factor_y);

  //--------------------------------------------------------
  //Draw curve line
  //--------------------------------------------------------
  do 
   {								
     lim=0;

	 //Calculation current point position
     x = x0 + x_from; 
     y = pBuf[x_from];
	 if(y<offset_y)
	 { 
	   y = offset_y; 
	   lim=1;
	 }
	 y -= offset_y + y0;
	
     //if(value_overhead && value>=360) 
     if(lim==0) 
		pDC->MoveTo(x, y);

     //next point from data buffer
     x_from++;

     //Calculation next point position
     x = x0 + x_from; 
     y = pBuf[x_from];
	 if(y<offset_y)
	 { 
	   y = offset_y; 
	   lim=1;
	 }
	 y -= offset_y + y0;

	 //----
     if(lim==0) 
		pDC->LineTo(x, y);
   } 
  while (x_from < x_to);
 
 //----- delete all Pen
 //Set old Pen to DC 
 pDC->SelectObject(pOldPen);
 pen.DeleteObject();
}
