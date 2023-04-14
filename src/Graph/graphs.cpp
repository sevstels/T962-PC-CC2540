//==============================================================================
//File name:   "graphs.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:   (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "graphs.h"
#include <math.h>
#include "Arrows.h"

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
  //----
  scale_length_x = 0;
  scale_length_y = 0;

  //----	
  last_file.empty();
  file_number = 0;
  normalize = 0;
  curve_height = 0;
  curve_length = 0;
  curve_length = 0;
  resize_factor_x = 1;
  resize_factor_y = 1;
  hWnd = NULL;
  hDC = NULL;
  signal_detected = 0;
  //----
  scale_line_width = 1;
  //----
  save_bmp = 0;
  save_jpg = 0;
  save_gif = 0;
  save_png = 0;
  save_svg = 0;
  control_line_length = 0;
  test_line_length = 0;
  memset(graph_line_width, 0, sizeof(graph_line_width));
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
void CGraphs::SetScaleX(int from, int to, int step)
{
  scale_x_begin = from;
  scale_x_end = to;
  scale_x_cell = (to-from)/step;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CGraphs::SetScaleY(int from, int to, int step)
{
  scale_y_begin = from;
  scale_y_end = to;
  scale_y_cell = (to-from)/step;
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

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CGraphs::round(double value)
{
  return (int)floor(value+(double)0.5);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CGraphs::SetSize(int x, int y)
{
  picture_heigth = y;
  picture_width = 460;//x;
}

//------------------------------------------------------------------------------
//Vertical data resize
//------------------------------------------------------------------------------
int CGraphs::ResizeDataY(std::vector<PointXY> *pBufOut, 
	                     std::vector<Point2D> *pBufIn)
{
  TRACE("Resize Data by Y\n"); 
     
  //пересчитать максимальную точку в данных
  resize_factor_y = picture_heigth;
  resize_factor_y /= 225; //scale_y_end;
  resize_factor_y = 1;

  size_t count = pBufIn->size();
  if(count<1) return 0;
 
  //очистить буфер итоговой картинки
  pBufOut->empty();

  //Vertical resizer
  for(unsigned int x=0; x<count-1; x++)
  {
	Point2D p = pBufIn->at(x);
	double temp = p.y;
    
	//---- y point
	temp *= resize_factor_y;

	//величина для графика в пикселях
	int y = round(temp);
	PointXY pxy;
	
	pxy.y = y;
	pxy.x = (int)p.x;
	pBufOut->push_back(pxy);
	
	TRACE2("x %d y %d\n", pxy.x, pxy.y);
  }

  return 1;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CGraphs::ResizeDataX(std::vector<PointXY> *pBufOut, 
	                     std::vector<Point2D> *pBufIn)
{  /*
  size_t count = pBufIn->size();
  if(count<1) return 0;

 // int last_x = (int) pBuffer->at(count-1).x;
 // if(last_x>= curve_length) curve_length = last_x;
 
  TRACE("Resize Data by X\n");

  //очистить буфер итоговой картинки
 // x.RemoveAll();
 // y.RemoveAll();
 // double dbl_x;

  //Vertical resizer
  for(unsigned int x=0; x<count-1; x++)
  {
    //---- y point
    dbl_y = pBufIn->at(xx).x;
	dbl_y *= factor_y;

	//величина для графика в пикселях
	int yi = round(dbl_y);
	y.Add(yi);
	x.Add(xx);
	TRACE2("x %d y %d\n", xx, yi);
  }	  */
/*
  //Horizontal resizer
  double dk = (double)count;
  dk = 1;///= 240;
  
  unsigned int k = round(dk);
  unsigned int index_in  = 0;
  unsigned int index_out = 0;
    
  for(unsigned int x=0; x<count-2; x +=k)
  {
    dbl_y = 0;

    //считываем блок точек и находим среднее
    for(unsigned int i=0; i<k; i++)
    {
	  dbl_y += y.GetAt(index_in++);
    }
    
    //считаем среднее для группы
    dbl_y /= k;

    unsigned short out = 900;
    out -= round(dbl_y);
    
    //сохраняем точку
	y.SetAt(index_out++, out);

	TRACE2("x %d y %d\n", index_out, out);
  }

  //сколько удалить элементов
  count = count - index_out -1;

  //удалить не используемую память
  y.RemoveAt(index_out, count);
*/
  return 1;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CGraphs::CreateGraph(const char *pFileName)
{
  int i, y, width, height;
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

  int dots_count = (int) graph_line[0].size();
	
  //last poin
  width = graph_line[0].at(dots_count-1).x;
  width -= (int)graph_line[0].at(0).x;
  
  height = picture_heigth;

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
    
  //Draw graph scale
  Draw_ScaleY(&PIC, 0,0, width, height); // , 12, RGB(60,60,60)
  Draw_ScaleX(&PIC, 0,0);
  
  //Curve count
  int c_count = 1;
  for(int i=0; i<c_count; i++)
  {
	 for(int n=0; n<graph_line[i].size()-1; n++)
	 {
		int x0 = (int)graph_line[i].at(n).x;
		int y0 = (int)graph_line[i].at(n).y;
	    int x = (int)graph_line[i].at(n+1).x;
		int y = (int)graph_line[i].at(n+1).y;
		
		if(y0!=0)
		//Draw curve
	    Draw_Line(&PIC, graph_line_width[i],x0,y0,x,y,graph_line_color[i]);
	 }   
  }

  //Restore old setup
  PIC.SetMapMode(oldMapMode);
  PIC.SetBkMode(oldBkMode);

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
  HINSTANCE Status;
  std::string msg, file;

  if(pFileName!=NULL){file = pFileName;}
  else file = last_file;
   
  //------------------------------------
  //launch default picture browser
  //------------------------------------
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
//Draw Horizontal lines by X-direction
//------------------------------------------------------------------------------
void CGraphs::Draw_ScaleX(CDC *pCDC, int x0, int y0)
{ 
  CString txt;
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
  font.CreateFont(16,0,0,0,FW_HEAVY,0,0,0,DEFAULT_CHARSET,//RUSSIAN_CHARSET,
                OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
                PROOF_QUALITY,VARIABLE_PITCH | FF_DONTCARE,"Arial");
 
  pCDC->SelectObject(&font);
  pCDC->SetTextColor(RGB(0,0,0));

/*  if(range_xn==GRPH_WIDTH_AUTO)
  { 
	range_xn = curve_length;
  }	 */

  //Calc horisontal pixel steps
  int height = picture_heigth;
  int line_numbers = scale_x_cell;
  
  int range_x =  scale_x_end - scale_x_begin;  
  txt_step = (int)(range_x/line_numbers);
   
  //Draw vertical lines by X-step  
  step_x = range_x/scale_x_cell;
  txt_digit = scale_x_begin;

  for (int x=0; x <=range_x; x+=step_x) 
  {
    Draw_Line(pCDC, scale_line_width, x, y0, x, 
		      height+y0, scale_line_color);

    //Print horizontal scale digit
    txt.Format("%d", txt_digit);
    txt_digit += txt_step;

    if(txt_digit>=10)txt_x_offset=4;
    if(txt_digit>=100)txt_x_offset=10;
    if(txt_digit>=1000)txt_x_offset=14;
    //Text out
    pCDC->TextOutA(x-txt_x_offset, y0+txt_y_offset, txt);	
  }
 
  //---- delete graphic objects
  font.DeleteObject();	
}

//------------------------------------------------------------------------------
//Draw Vertical lines by X-direction
//------------------------------------------------------------------------------
void CGraphs::Draw_ScaleY(CDC *pCDC, int x0, int y0, int width, int height)
{ 
  CString txt;
  int line_width = 1;

  //------------------------------------------
  //Set parameters for Text output
  //------------------------------------------
  //set font
  CFont font;
  font.CreateFont(16,0,0,0,FW_HEAVY,0,0,0,DEFAULT_CHARSET,//RUSSIAN_CHARSET,
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
  int txt_x_offset = 26;
  int txt_y_offset = 6;

  width = scale_x_end; 
  height = picture_heigth;

  step_y = (int)(height/scale_y_cell);
  txt_step = (int)((scale_y_end-scale_y_begin)/scale_y_cell);

  txt_digit = scale_y_begin;


  for(int y=0; y<=height; y+= step_y)
  {
    //Print vertical scale digit
    txt.Format("%d", y/*txt_digit*/);

    Draw_Line(pCDC, scale_line_width, x0, y, width, y, 
		            scale_line_color);

	//Text out, vertical digits    
    pCDC->TextOutA(x0-txt_x_offset, y+txt_y_offset, txt);

	txt_digit += txt_step;
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
  int offset_y = 0;// (int)((range_yn-range_y0)*resize_factor_y);
  float y_fl;

  //--------------------------------------------------------
  //Draw curve line
  //--------------------------------------------------------
  do 
   {								
     lim=0;

	 //Calculation current point position
     x = x0 + x_from; 
     ///y = pBuf[x_from];
	 //Resize
	 y_fl = pBuf[x_from];
	 y_fl *= (float)resize_factor_y;
	 y = (int)y_fl; 
  if(y<0)y=0;
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
     ///y = pBuf[x_from];
	 //Resize
	 y_fl = pBuf[x_from];
	 y_fl *= (float)resize_factor_y;
	 y = (int)y_fl; 
	 
  if(y<0)y=0;
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

//------------------------------------------------------------------------------
//Draw
//------------------------------------------------------------------------------
void CGraphs::Draw_MsgBox(CDC *pCDC, int x0, int y0, int dx, int dy, CString txt, int yes_no)
{ 
  COLORREF bgn_color;
  COLORREF line_color;
  COLORREF txt_color;   
	
  if(yes_no==1)
  {
  	bgn_color = RGB(230,230,230);
  	line_color = RGB(255,0,0);
	txt_color = RGB(255,0,0);
  }
  else
  {
   	bgn_color = RGB(250,250,250);
  	line_color = RGB(0,180,0);
	txt_color = RGB(0,180,0);
  }

  //Fill BGnd
  pCDC->FillSolidRect(x0, y0, dx, dy+2, bgn_color);
  
  //Window edge line верхняя гориз
  Draw_Line(pCDC, 1, x0, y0, x0+dx, y0, line_color); 
  //Window edge line нижняя гориз
  Draw_Line(pCDC, 1, x0, y0+dy+2, x0+dx, y0+dy+2, line_color);
  //Window edge line вертикальная левая
  Draw_Line(pCDC, 1, x0, y0, x0, y0+dy+2, line_color); 
  //Window edge line вертикальная правая
  Draw_Line(pCDC, 1, x0+dx, y0, x0+dx, y0+dy+2, line_color);

  int x=x0+4, y=y0+26;
  
  SetFont(pCDC, 24, 1, 0);
  pCDC->SetTextColor(txt_color);
  pCDC->TextOutA(x, y, txt);
}

//------------------------------------------------------------------------------
//Draw
//------------------------------------------------------------------------------
void CGraphs::Draw_Point(CDC *pCDC, int buf_position)
{ 
  int x = 200;
  int y = 100;

  CPen pen_gray;
  CString txt;
  txt.Format("%d  %2.1f v", x, 0.1);

  //left/right arrows ini
  ARROWSTRUCT ARROW;
  ARROW.nWidth = 5;
  ARROW.fTheta = 0.4f;
  ARROW.bFill = true;
  ARROW.color = RGB(160,160,160);
  
  pen_gray.CreatePen(PS_SOLID, 1, RGB(120,120,120));
  pCDC->SelectObject(&pen_gray);	//select pancil

  //write arrow
  ARROW.color = RGB(255,0,0);
  pCDC->MoveTo(x+30-buf_position, y-10);
  pCDC->LineTo(x+16-buf_position, y-10);//draw
  ArrowTo(pCDC->m_hDC, x-buf_position, y,&ARROW);
  
  //pCDC->SetBkColor(RGB(255,255,111));//ETO_OPAQUE
  pCDC->SetBkColor(RGB(240,240,240));//ETO_OPAQUE
  pCDC->SetTextColor(RGB(40,40,40));
  pCDC->ExtTextOutA(x+34-buf_position, y-18, ETO_CLIPPED,NULL,"Lock Point:",NULL);
  pCDC->ExtTextOutA(x+34-buf_position, y, ETO_CLIPPED,NULL,txt,NULL);
  
  //----- delete all Pen
  pen_gray.DeleteObject();
}

//------------------------------------------------------------------------------
//Draw
//------------------------------------------------------------------------------
void CGraphs::Draw_PointBox(CDC *pCDC, int buf_position)
{
 //BOOL CDC::GetTextMetrics(LPTEXTMETRICS TextAtttrib) const;
}

//------------------------------------------------------------------------------
//Paint
//------------------------------------------------------------------------------
void CGraphs::SetFont(CDC *pCDC, // Гарнитура
                      int Size, // размер в пунктах
                      BOOL Bold,// Признак жирного начертания
                      BOOL Italic) // Признак наклонного начертания
{
 /*	
 //Получим контекст окна
 CWindowDC winDC = GetDC(this);
	
 //Узнаем, сколько пикселей в одном логическом дюйме
 int pixelsPerInch = winDC.GetDeviceCaps(LOGPIXELSY);
 	
 //Узнаем высоту в пикселях шрифта размером Size пунктов
 int fontHeight = -MulDiv(Size, pixelsPerInch, 72);
 	
 //Устанавливаем параметр жирности для функции CreateFont()
 int Weight = FW_NORMAL;
 if(Bold) Weight = FW_BOLD;
 	
 //Удаляем предыдущий экземпляр шрифта -- нельзя дважды 
 //инициализировать шрифт вызовом CreateFont().
 delete m_pFont;
 */

  //set font
  CFont font;
  font.CreateFont(Size,0,0,0,FW_HEAVY,0,0,0,DEFAULT_CHARSET,//RUSSIAN_CHARSET,
                OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
                PROOF_QUALITY,VARIABLE_PITCH | FF_DONTCARE,"Arial");
 
  pCDC->SelectObject(&font);
  font.DeleteObject();
 /*
 m_pFont = new CFont;
 
 //Создание шрифта. Большинство параметров не используются.
 m_pFont->CreateFont(fontHeight, 0, 0, 0, Weight, Italic, 0, 0, 
                                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
                                CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
                                DEFAULT_PITCH | FF_DONTCARE, Typeface);	 */
}
