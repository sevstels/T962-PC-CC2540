//==============================================================================
//File name:   "anls-graph.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:   (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "anls-graph.h"
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

#define GRPH_WIDTH_AUTO -11 

//------------------------------------------------------------------------------
//constructor
//------------------------------------------------------------------------------
CAnGraph::CAnGraph()
{   	
  last_file.empty();
  file_number = 0;
  normalize = 0;
  range_x_max = 0;
  range_y_max = 0;
  curve_height = 0;
  curve_length = 0;
  columns_x = 1; 
  rows_y = 1;
  scale_height_px_y = 0;
  scale_width_px_y = 0;
  resize_factor_x = 1;
  resize_factor_y = 1;
  hDC = NULL;
  pBuf = Buf;
  sample_count = sizeof(struct_analys)>>1;
}

//------------------------------------------------------------------------------
//destructor
//------------------------------------------------------------------------------
CAnGraph::~CAnGraph()
{

}

//------------------------------------------------------------------------------
//HDC -это идентификатор структуры данных,описывающих устройство вывода графики
//CDC -это контекст графического устройства
//------------------------------------------------------------------------------
void CAnGraph::SetHWnd(HANDLE hHWnd){hWnd = hHWnd;}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CAnGraph::SetRangeX(int from, int to, int columns)
{
  range_x0 = from;
  range_x_max = to;
  columns_x = columns; 
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CAnGraph::SetRangeY(int from, int to, int rows)
{
  range_y0 = from; 
  range_y_max = to;
  rows_y = rows;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CAnGraph::SetMulX(double factor_x)
{
  resize_factor_x = factor_x;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CAnGraph::SetMulY(double factor_y)
{
  resize_factor_y = factor_y;
}
//------------------------------------------------------------------------------
//For draw Line
//------------------------------------------------------------------------------
void CAnGraph::Calc_LinePoints(int x1, int y1, int x2, int y2)
{
 const int deltaX = abs(x2 - x1);
 const int deltaY = abs(y2 - y1);
 const int signX = x1 < x2 ? 1 : -1;
 const int signY = y1 < y2 ? 1 : -1;
 
 //----
 int error = deltaX - ((int)deltaY);
 //
 Buf[x2] = y2;
 while(x1 != x2 || y1 != y2) 
 {
   Buf[x1] = y1;
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
int  CAnGraph::Parser(CWordArray *pX, CWordArray *pY, int mode)
{ /*
  int count = pX->GetSize();
  buf_in_count = count;
  int last_x = (int) pX->GetAt(count-1);
  curve_length = last_x;
  //----
  CPoint p;
  PArray.RemoveAll();

  for(int i=0; i<count; i++)
  {
   	p.x = pX->GetAt(i);	
	p.y = pY->GetAt(i);
	PArray.Add(p);
  }

  //
  memset(pBuf, 0, sizeof(Buf));

  for(int i=0; i<count-1; i++)
  {
    //---- x1 first point
    int x1 = (int)(pX->GetAt(i)*resize_factor_x);
	
    double dbl_y = pY->GetAt(i);
	if(mode==GRAPH_LOG) 
	   dbl_y = log(dbl_y);
    int y1 = (int)(resize_factor_y*dbl_y);

    //---- x2
    int x2 = (int)(pX->GetAt(i+1)*resize_factor_x);
    dbl_y = pY->GetAt(i+1);
	if(mode==GRAPH_LOG) 
	   dbl_y = log(dbl_y);
    int y2 = (int)(resize_factor_y*dbl_y);

    if(y1 != y2)
     {
       //for ramp lines
       Calc_LinePoints(x1, y1, x2, y2);
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
   */
  return 1;
}

//------------------------------------------------------------------------------
//Распределение семплов по каналам
//------------------------------------------------------------------------------
int CAnGraph::Parser(CArray <struct struct_analys, struct struct_analys> *pArr)
{ 
 CPoint pCh;
 struct struct_analys sample;
 int channel_numbers = GRAPH_CHANNELS;

 for(int i=0; i<channel_numbers; i++) ArrCh[i].RemoveAll();
 
 //число семплов в массиве
 int sample_number = pArr->GetCount();
 for(int n=0; n<sample_number; n++)
 {
  sample = pArr->GetAt(n);
	  /*
  for(int i=0; i<channel_numbers; i++)
  {
	//---- point ch(x.y)
	pCh.x = sample.x;
	pCh.y = sample.ch[i]>>6;//to 10 bit
	ArrCh[i].Add(pCh);
  }  */
 }

 return sample_number;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CAnGraph::Normalization(void)
{
  int min_x, max_x, min_y, max_y;
  
  curve_height = range_y_max-range_y0;
  if(curve_length<=0 || curve_height<=0) return;

  //----
  int find_min = find_Min(pBuf, 0, curve_length, min_x, min_y);
  int find_max = find_Max(pBuf, 0, curve_length, max_x, max_y);
  
  if(find_min==1 && find_max==1)
  {
    //calc resize_factor X
    resize_factor_x = max_x-min_x;
    resize_factor_x /= curve_length;

    //calc resize_factor Y
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
void CAnGraph::Resize_Curve(short *pBufOut, 
	                        CArray<CPoint, CPoint> *pArray, 
	                        int width, int height, int flag_normalize)
{ 
  double y_normalize = 1;
  int count = pArray->GetCount();
  int y_max = 0;
  int y_min = 0;
  
  //find Y max
  for(int i=0; i<count; i++)
  {
  	int y = (int)pArray->ElementAt(i).y;
	//находим максимальное значение в массиве по Y
	if(y>y_max)
	  {y_max = y;}
  }
     
  //find Y min
  y_min = y_max;
  for(int i=0; i<count; i++)
  {
  	int y = (int)pArray->ElementAt(i).y;
	//находим минимальное значение в массиве по Y
	if(y<y_min)
	  {y_min = y;}
  }

  //коэффициент масштабирования по вертикали для нормирования к всей шкале
  if(y_max!=0)
  {
    if(y_max < range_y_max)
    {
      //если величина данных больше размера шкалы
	  y_normalize = (double)range_y_max / (double)(y_max-y_min);
    }
   else
    {
      //если величина данных меньше размера шкалы
	  y_normalize = (double)(y_max-y_min) /(double)range_y_max ;
    }
  }

  //если нормализация по y отключена
  if(flag_normalize!=1){y_normalize = 1;  y_min=0;}

  //считаем коэфф преобразования для данного размера картинки 
  double factor_y = height;
  factor_y /= range_y_max;
  factor_y *= y_normalize;

  //считаем коэфф преобразования для данного размера картинки 
  double factor_x = width;
  factor_x /= range_x_max;

  for(int i=0; i<count-1; i++)
  {
	//----
	int x1 = pArray->ElementAt(i).x;
  	int y1 = pArray->ElementAt(i).y;
  	x1 = (int)(factor_x * x1);
  	y1 = (int)(factor_y *(y1-y_min));

	//----
	int x2 = pArray->ElementAt(i+1).x;
  	int y2 = pArray->ElementAt(i+1).y;
  	x2 = (int)(factor_x * x2);
  	y2 = (int)(factor_y *(y2-y_min));

	Calc_LinePoints(x1, y1, x2, y2);
	//Calc_LinePointsT(x1, y1, x2, y2);
  }	

  memcpy(pBufOut, pBuf, width<<1);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CAnGraph::Polynom_Line(short *pBufOut, CArray<CPoint, CPoint> *pArray, 
	                        int width, int height, int flag_normalize)
{	   /*
  CPOLY POLYNOM;
  int length = pArray->GetCount();
  
  //переложить данные в формат для вычисления
  //POLYDATA *pData = (POLYDATA*) new POLYDATA [length]; 
  POLYDATA pData[50];

  for(int i=0; i<length;i++)
  {
	pData[i].r = pArray->ElementAt(i).y;
	pData[i].t = pArray->ElementAt(i).x;
  }

  //считаем коэффициенты полинома
  double a[4];
  POLYNOM.Calc(pData, length, 1);
  for(int i=0; i<4; i++) a[i] = *(double*)POLYNOM.erg[i];

  //считаем точки кривой по этим коэффициентам
  //double r = TempToResistance();
 */
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CAnGraph::Normalize_Y(CArray<CPoint, CPoint> *pArray, 
	                       int width, int height, 
						   int img_width, int img_height)
{ 
  int count = pArray->GetCount();
  
  //find Y max 
  double factor_y = img_height;
  factor_y /= range_y_max;
  
  double factor_x = img_width;
  factor_x /= range_x_max;

  for(int i=0; i<count-1; i++)
  {
	//----
	//int x = pArray->ElementAt(i).x;
  	int y = pArray->ElementAt(i).y;
  	//x = (int)(factor_x * x);
  	y = (int)(factor_y * y);
	
	//pArray->ElementAt(i).x = x;
	pArray->ElementAt(i).x = y;
  }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CAnGraph::Bezier_Resize(short *pBuf, CArray<CPoint, CPoint> *pArray, 
	                         int width, int height)
{ 
  //это не работает
  CINTP INTP;
  INTP.BezierResizeA(pBuf, pArray, width, height);
}
/*
//------------------------------------------------------------------------------
//приводит значения к ближайшим целым значениям
//берём значения и возвращаем результат в массив точек 
//------------------------------------------------------------------------------ 
void CINTP::BezierResizeA(short *pBufw, CArray<CPoint, CPoint> *pArray, 
	                       int width, int height, int *pOut)
{
  CPoint cp;
  Point2D pt;
  values.empty();
  double var;
  
  //---- Load data 
  for(int i=0; i<pArray->GetCount(); i++)
  {
	pt.x = pArray->GetAt(i).x;
	pt.y = pArray->GetAt(i).y;
	//----
	values.push_back(pt);
  }
   
  CalcSpline();
  CSegment sg;
  
  int count = values.size(); 
  for(int i=0; i<count-1; i++)
  {
	 sg.points[0] = bezier[i].points[0];
	 sg.points[1] = bezier[i].points[1];
	 sg.points[2] = bezier[i].points[2];
	 sg.points[3] = bezier[i].points[3];

	 int index = 0;
	 for(int j=0; j<width; j++)
	 {
	   var = (double)j;
	   var /=(double)width;
	   sg.Calc(var, p);

 	   //----
	   //TRACE1("Step: %d  ", j);
	   //TRACE2("X=%f  Y=%f  ", p.x, p.y);
	   //float y_db = (float)p.y;

	   //p.y = 200;
	   //делаем ресайз по Y
		  //TRACE2("Y[%d]=%f\n", index, y_db);
	   //находим ближайшую дискретную точку Y
	   short y_short = (short)p.y;//(unsigned short)floor(y_db+(float)0.5);
	     //TRACE2("Y[%d]=%d\n", index, y_short);
	   pBufw[index] = y_short;
	   pOut[index] = y_short;
	   index++;
	   //----
	   //TRACE2("Y[%d]=%d\n", j, y_short);
	 }
  }
}
*/
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CAnGraph::CreateGraph(int width, int height, const char *pFileName)
{
  int i, y;
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

  //====================================================================
  //Save signal to text *.h file + signal inversion
  //====================================================================
  //Bin_to_Txt(ADC_Buffer, size_x, 2);
  CBitmap bitmap;
  CDC PIC;

  PIC.CreateCompatibleDC(pDC);
  bitmap.CreateCompatibleBitmap(pDC, width+left_gap+right_gap, 
                                     height+top_gap+bottom_gap);
  PIC.SelectObject(&bitmap);

  //логические единицы отображаем, как физические
  //Set 1 pixel X = 1 pixel Y = MM_ISOTROPIC 
  PIC.SetMapMode(MM_ISOTROPIC);

  //Fill background
  PIC.FillSolidRect(0,0, width+left_gap+right_gap, 
                         height+top_gap+bottom_gap, RGB(0,0,0));// RGB(255,255,255)

  //Длина осей графика
  PIC.SetWindowExt(width, height); 
  
  //Определяем облась вывода, график растёт вверх по X
  PIC.SetViewportExt(width, -1*height);  
  
  //Установить начало координат	X,Y
  //Set zero point coordinate in the Graph space
  PIC.SetViewportOrg(left_gap, height+top_gap); 
      
  //Draw graph scale
  Draw_ScaleY(&PIC, 0,0, width, height, RGB(60,60,60));
  Draw_ScaleX(&PIC, 0,0, width, height, RGB(60,60,60));
  
  //Sin graph test
  //sin_wave((char*)pPic, 60, 60, 2000);
  //test_Func2((char*)pPic, 60, 60, 400);
    
  //Resize_Curve(pPic, &ArrR, width, height);
  //BZI.CalcBezier(pPic, &ArrR, 10, 640);
  //Draw_Curve(&PIC, pPic, 2, 0, 0, 0, width, RGB(255,0,0));  
  
 // Resize_Curve(pPic, &ArrG, width, height);
 // Draw_Curve(&PIC, pPic, 2, 0, 0, 0, width, RGB(0,255,0));  
  
  normalize	= 0;

  ///Polynom_Line(pPic, &ArrB, width, height, 0);

  //===========================================================
  //---- Draw curve Ch1
  Resize_Curve(pPic, &ArrCh[0], width, height, normalize); 
  //Bezier_Resize(nn, &ArrR, width, height);//	 pPic
  //Normalize_Y(&ArrR, 0, 0, width, height);
  Draw_Curve(&PIC, pPic, 2, 0, 0, 0, width, RGB(255,0,0)); //

  //---- Draw curve Ch2
  Resize_Curve(pPic, &ArrCh[1], width, height, normalize);
  Draw_Curve(&PIC, pPic, 2, 0, 0, 0, width, RGB(0,255,0)); // 
 
  //---- Draw curve Ch3
  Resize_Curve(pPic, &ArrCh[2], width, height, normalize);
  Draw_Curve(&PIC, pPic, 2, 0, 0, 0, width, RGB(0,0,255)); //RGB_BLUE 

  //---- Draw curve Ch4
  Resize_Curve(pPic, &ArrCh[3], width, height, normalize);
  Draw_Curve(&PIC, pPic, 2, 0, 0, 0, width, RGB(127,127,0));//RGB_YELLOW

  //---- Draw curve Ch5
  Resize_Curve(pPic, &ArrCh[4], width, height, normalize);
  Draw_Curve(&PIC, pPic, 2, 0, 0, 0, width, RGB(0,127,0));//RGB_YELLOW

  //---- Draw curve Ch6
  Resize_Curve(pPic, &ArrCh[5], width, height, normalize);
  Draw_Curve(&PIC, pPic, 2, 0, 0, 0, width, RGB(127,0,127));//RGB_YELLOW
  //============================================================

  //clear arrays
  for(int i=0; i<GRAPH_CHANNELS; i++){ArrCh[i].RemoveAll();}

  /*
  //============================================================
  //---- Limits
  y = 150; 
  Draw_Line(&PIC,1,0,y,width,y, RGB_BLUE);//RGB_YELLOW
  y = 136;
  Draw_Line(&PIC,2,0,y,width,y, RGB(255,0,0));//RGB_BLUE
  y = 125;
  Draw_Line(&PIC,1,0,y,width,y, RGB_BLUE);//RGB_BLUE
  //============================================================
 */
  //Release buffer
  if(pPic!=NULL){delete [] pPic; pPic = NULL;}
  
  //Release DC
  ReleaseDC((HWND)hWnd, hDC);

  file += "_";
  file += std::to_string((long long)file_number++);
  file += ".png";
  last_file = file;

  //Save image file
  CImage image;     
  image.Attach(bitmap);    
  image.Save(_T(file.c_str()), Gdiplus::ImageFormatPNG); 
  
  return 1; 
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CAnGraph::ShowGraph(const char *pFileName)
{
  HINSTANCE Status = 0;
  std::string msg, file;

  //если пробелы в пути - добавить двойные кавычки  
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
int CAnGraph::find_Max(signed short *pBuff, int from, int to, int &x, int &y)
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
int CAnGraph::find_Min(signed short *pBuff, int from, int to, int &x, int &y)
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
void CAnGraph::Draw_ScaleX(CDC *pCDC, int x0, int y0, int width, int height,
                            COLORREF color)
{ 
  CString txt; 
  char line_width = 1;
		
  //------------------------------------------
  //Set parameters for Text output
  //------------------------------------------
  //set font
  CFont font;
  font.CreateFont(11,0,0,0,FW_BOLD,0,0,0,RUSSIAN_CHARSET,
                OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
                PROOF_QUALITY,VARIABLE_PITCH | FF_DONTCARE,"Arial");
 
  pCDC->SelectObject(&font);
  pCDC->SetTextColor(RGB(180,180,180));

   //Calc vertical pixel steps 
  double range_x = range_x_max-range_x0; 
  float txt_step;
  float txt_digit = 0;
  
  int x=0, step_x;
  int txt_x_offset = 6;
  int txt_y_offset = -6;
  
  txt_step = (float)(range_x/columns_x);
  txt_digit = range_x0;
  step_x = (int)(width/columns_x);

  //----
  for (int n=0; n<columns_x+1; n++) 
  {
    Draw_Line(pCDC, line_width, x, y0, x, height+y0, color);

    //Print horizontal scale digit
    //txt.Format("%.1f", txt_digit);
	txt.Format("%ds", (int)txt_digit);
    txt_digit += txt_step;

    if(txt_digit>=10)txt_x_offset=4;
    if(txt_digit>=100)txt_x_offset=6;
    if(txt_digit>=1000)txt_x_offset=10;
    //Text out
    pCDC->TextOutA(x-txt_x_offset, y0+txt_y_offset, txt);
	x += step_x;
  }
 	
  //---- delete graphic objects
  font.DeleteObject();
}

//------------------------------------------------------------------------------
//Draw Vertical lines by X-direction
//------------------------------------------------------------------------------
void CAnGraph::Draw_ScaleY(CDC *pCDC, int x0, int y0, int width, int height, 
                            COLORREF color)
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
  double range_y = range_y_max-range_y0; 
  float txt_step;
  float txt_digit = 0;
  
  int y=0, step_y;
  int txt_x_offset = 24;
  int txt_y_offset = 6;
  
  txt_step = (float)(range_y/rows_y);
  txt_digit = range_y0;
  step_y = (int)(height/rows_y);

  for(int n=0; n<rows_y+1; n++)
  {
    Draw_Line(pCDC,(char)line_width, x0, y, width, y, color);

    //Print vertical scale digit
    //txt.Format("%.1f", txt_digit);
    txt.Format("%d", (int)txt_digit);
	txt_digit += txt_step;

    //Text out    
    pCDC->TextOutA(x0-txt_x_offset, y+txt_y_offset, txt);
    scale_height_px_y = y;
	y +=step_y;
  } 
    
  //---- delete graphic objects
  font.DeleteObject();
}

//------------------------------------------------------------------------------
//Draw line
//------------------------------------------------------------------------------
void CAnGraph::Draw_Line(CDC *pDC, char width, int x0, int y0, 
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
void CAnGraph::Draw_Curve(CDC *pDC, short *pBuf, char line_width, 
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
