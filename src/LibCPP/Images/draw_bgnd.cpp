#include "stdafx.h"
#include "Images.h"
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

//HDC -это идентификатор структуры данных,описывающих устройство вывода графики
//CDC -это контекст графического устройства

//------------------------------------------------------------------------------
//Draw BackGround Image
//------------------------------------------------------------------------------
void CImages::Draw_BackGround(void)
{	 
 int w_width = Window_Rect.Width();
 int w_height = Window_Rect.Height();
 int Scale = 8;
 
 //Fill background, залить белым 
 BGND.FillSolidRect(0,0,w_width,w_height,RGB(255,255,255));
 
 CURVE.FillSolidRect(0,0, WindGraph_Rect.Width(), WindGraph_Rect.Height(),RGB(255,255,255));
 
 //1 pixel = 1point 
 BGND.SetMapMode(MM_TEXT);	
 
 //установить начало координат в физической области вывода
 //Set zero point coordinate in the Graph space
 //memBGD.SetWindowOrg(4,-20);
 
 //memBGD.SetViewportOrg(4,200); //ограничивает показ окна

 //memBGD.SetWindowOrg(30,30); //сдвиг относительно окна
 //dc.SetWindowExt(80,80);
 
 //------------------------------------------
 //параметры вывода фонтов и карандашей
 //Set parameters for Text 
 //------------------------------------------
 //set font
 CFont font;
 font.CreateFont(11,0,0,0,FW_BOLD,0,0,0,RUSSIAN_CHARSET,
				OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
				PROOF_QUALITY,VARIABLE_PITCH | FF_DONTCARE,"Arial");
 CFont *pFont = BGND.SelectObject(&font);
 CString txt;

 //------------------------------------------
 //параметры вывода фонтов и карандашей
 //Set parameters for Pancil
 //------------------------------------------
 CPen pen_cell;

 //инициализация карандаша для рисования линии координат
 pen_cell.CreatePen(PS_SOLID, 1, RGB(200,200,200));
 
 //Paint Window border, рисуем рамку окна
 BGND.DrawEdge(&Window_Rect, EDGE_ETCHED, BF_RECT);	//EDGE_BUMP

 //-----------------------------------------
 //Paint Horisontal lines (coordinate cell)
 //-----------------------------------------
 //Table cell Graph Image size and position
 int width    = WindGraph_Rect.Width();
 int height   = WindGraph_Rect.Height();
 int offset_x = WindGraph_Rect.left;
 int offset_y = WindGraph_Rect.top;

 //select coordinate pancil  (для рисования линии координат)
 BGND.SelectObject(&pen_cell);

 BGND.SetTextColor(RGB(255,0,0));
 int   cellsize = height/16;
 char  end = (char)(height/cellsize);
 float scale_digit = 1;
 
 if(Scale==8)  scale_digit = 16;  //256
 if(Scale==10) scale_digit = 64;  //1024
 if(Scale==17) scale_digit = 4096; //65536

 float temp = 0;
 char loop = 0;
 int y=0;
 
 do 
 {
	//move to point (x,y)
	BGND.MoveTo(WindGraph_Rect.left,WindGraph_Rect.bottom-y);
	//draw line
	BGND.LineTo(WindGraph_Rect.right,WindGraph_Rect.bottom-y);
	
	//put digit verticall
	//txt.Format("%5.0f", temp);
	//BGND.TextOut(WindGraph_Rect.left-26, WindGraph_Rect.bottom-y-6, txt);
	
	//шаг клетки графика по вертикали
	temp = temp + scale_digit;
	y = y + cellsize;

	loop ++;
			
} while (loop <= end);
 
 //Parameter name
 //txt = _T("Y: Scale 0..1024 nA");
 //BGND.TextOut(8, Window_Rect.bottom-20,txt);

 //-----------------------------------------
 //рисуем вертикальные линии 
 //Paint Vertical lines (coordinate network)
 //-----------------------------------------
 BGND.SetTextColor(RGB(0,0,255));
 cellsize = (int)(width/20);
 end = (char)(width/cellsize); 
 loop = 0;
 int x = 0;

 do 
 {
	//move to point (x,y)
	BGND.MoveTo(WindGraph_Rect.left+x,WindGraph_Rect.bottom);
	//draw line
	BGND.LineTo(WindGraph_Rect.left+x,WindGraph_Rect.top);
	
	//put digit horizontally
	//txt.Format("%d", x);
	//BGND.TextOutA(x, 270, txt);	
	
	//шаг клетки по горизонтали
	x = x + cellsize;	
	loop++;

 } while (loop <= end);//число клеток по горизонтали

 //Parameter name 
 //txt = _T("X: Time (x100 msec)");
 //BGND.TextOut(300, Window_Rect.bottom-20,txt);

 //---- delete all graphic objects
 BGND.SelectObject(&pen_cell);
 pen_cell.DeleteObject();
 
 BGND.SelectObject(&font);
 font.DeleteObject();
 
 //pDC->Detach();
 //pDC = NULL;
}
