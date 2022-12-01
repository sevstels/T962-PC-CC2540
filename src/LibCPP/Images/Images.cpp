//==============================================================================
//File name:   "Images.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Images.h"
#include "../Arrows/Arrows.h" //Draw Arrow lines
#include <math.h>
//#include "Brent.h"

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

//-----------------------------------------------------------------------------
//constructor
//-----------------------------------------------------------------------------
CImages::CImages(void)
{   	
  //Set Graph window size and position
  //left, top, right, bottom
  WindGraph_Rect.left = 32;						//offset left
  WindGraph_Rect.top = 12;						//offset top
  WindGraph_Rect.right = WindGraph_Rect.left + 400;	//size X
  WindGraph_Rect.bottom = WindGraph_Rect.top + 256;	//size Y
    
  Window_Rect.SetRect(0,0,460,310);

  xy0.x = 0;   //graph x0 point 
  xy0.y = 256; //graph y0 point
}

//-----------------------------------------------------------------------------
//destructor
//-----------------------------------------------------------------------------
CImages::~CImages(void)
{
  //WINDOW.ReleaseOutputDC();
  //result = WINDOW.Detach();
  //ASSERT(result);
  //BGND.ReleaseOutputDC();
  //CURVE.ReleaseOutputDC();

  bmp_Window.DeleteObject();
  bmp_Curve.DeleteObject();
  bmp_BackGround.DeleteObject();

  //DeleteObject(&bmp_BackGround);
  //DeleteObject(&bmp_Curve);
  //DeleteObject(&bmp_Window);
}
//------------------------------------------------------------------------------
//Create CDC
//------------------------------------------------------------------------------
int CImages::Create_CDC(CDC *pDC)
{	
  BOOL result;
  int width = Window_Rect.Width();
  int height = Window_Rect.Height();
  
  //====================================
  //Create Output Window
  //====================================
  result = WINDOW.CreateCompatibleDC(pDC);
  ASSERT(result); if (result == false) return -1;
  result = bmp_Window.CreateCompatibleBitmap(pDC,width,height);
  ASSERT(result); if (result == false) return -1;
  //Select object for draw
  WINDOW.SelectObject(&bmp_Window);

  //====================================
  //Create BackGround
  //====================================
  result = BGND.CreateCompatibleDC(pDC);
  ASSERT(result); if (result == false) return -1;
  result = bmp_BackGround.CreateCompatibleBitmap(pDC,width,height);
  ASSERT(result); if (result == false) return -1;
  //Select object for draw
  BGND.SelectObject(&bmp_BackGround);
  
  //====================================
  //Create Curve 
  //====================================
  width = WindGraph_Rect.Width();
  height = WindGraph_Rect.Height();
  
  result = CURVE.CreateCompatibleDC(pDC);
  ASSERT(result); if (result == false) return -1;
  result = bmp_Curve.CreateCompatibleBitmap(pDC,width,height);
  ASSERT(result); if (result == false) return -1;
  //Select object for draw
  CURVE.SelectObject(&bmp_Curve);
  
  //Set 1 pixel = 1point  
  CURVE.SetMapMode(MM_TEXT);  //MM_ANISOTROPIC 
  //установить начало координат в физической области вывода
    
  //CURVE.SetViewportExt(300,-250); 
  //CURVE.SetViewportOrg(40,-250);
  
  //Set zero point coordinate in the Graph space
  //CURVE.SetWindowExt(20, 40);
  
  //Set zero point coordinate in the Graph space

  return 1;
}

//------------------------------------------------------------------------------
//Paint Power Vector Pointer
//------------------------------------------------------------------------------
void CImages::Paint_Power_Vector(CDC *pDC, CPoint center, int angle)
{

 ARROWSTRUCT arrow;

 arrow.nWidth = 25;
 arrow.fTheta = 0.7f;
 arrow.bFill = true;
 arrow.color = RGB(255,0,0);

 //---- Calc coordinate
//int x, y;
 CRect internal_ellips;
 CRect external_ellips;
 CPoint coord;

 //ini
 internal_ellips.top = 118;	//ellipse size b (x)
 internal_ellips.right = 60;//ellipse size a (z)

 external_ellips.top = internal_ellips.top + 30;
 external_ellips.right = internal_ellips.right + 20;

 //Start point
 coord = Ellipse_xy(external_ellips, angle);
 pDC->MoveTo(coord.x + center.x, coord.y + center.y);	

 //End Point, 3D-Z
 coord = Ellipse_xy(internal_ellips, angle);
 ArrowTo(pDC->m_hDC, coord.x + center.x, coord.y + center.y, &arrow);

 pDC->LineTo(center.x,center.y);

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CPoint CImages::Ellipse_xy(CRect ellipse, int angle)
{
 CPoint coord;
 float Pi = (float)3.1415926535;
 angle = 180 - angle;
 //ellipse equation
 //int x, y, a, b;
 //x = (int)((a/b)*sqrt(float(b*b-y*y)));
 //b = ellipse.top;
 //a = ellipse.right;

 coord.x = (long)(ellipse.right * sin(float(angle*Pi/180))); 
 coord.y = (long)(ellipse.top * cos(float(angle*Pi/180)));

 return coord;
}

//-----------------------------------------------------------------------------
//Paint
//-----------------------------------------------------------------------------
void CImages::Display_XYZ(CDC *pDC, int x,int y, int z)
{ 
    
 CPoint position; 
 position.x = 30; 
 position.y = 296;
 
 CString txt;
 /*
 //---- Set parameters for Pancil
 CPen pen_red, pen_red_sl, 
      pen_blue, pen_blue_sl,	  
      pen_green, pen_green_sl;
 
 char width = 10;
 char l_wd  =  5;
 char r_wd  =  6;
 char txt_x = (char)position.x -(width>>1);

 pen_red.CreatePen(PS_SOLID,width,	RGB(255,0,0));		//X красный
 pen_red_sl.CreatePen(PS_SOLID, 1,	RGB(255,0,0));		//X красный
 pen_green.CreatePen(PS_SOLID,width,RGB(0,128,0));		//Y зелёный
 pen_green_sl.CreatePen(PS_SOLID,1,RGB(0,128,0));		//Y зелёный
 pen_blue.CreatePen(PS_SOLID,width,	RGB(0,0,255));		//Z синий
 pen_blue_sl.CreatePen(PS_SOLID,1,RGB(0,0,255));		//Y зелёный

 unsigned char bias = 127;
 x = (abs(x-bias)); 
 y = (abs(y-bias));
 z = (abs(z-bias)); 

 //--------------------------------------------------------
 //Draw Level line
 //--------------------------------------------------------
 //---- X 
 //Save first pan pointer
 CPen *pOldPen = pDC->SelectObject(&pen_red);		//select pancil
 pDC->MoveTo(position.x, position.y);				//Start point	 
 pDC->LineTo(position.x,position.y-x);				//draw line X
 pDC->SelectObject(&pen_red_sl);					//select pancil
 pDC->MoveTo(position.x-l_wd,position.y-(SENSE.x>>1));//Start point
 pDC->LineTo(position.x+r_wd,position.y-(SENSE.x>>1));//draw Sens line X
 //Print text
 txt.Format("Sensitivity X: %1.1f G", SENSE.x/42.667);
 pDC->SetTextColor(RGB(255,0,0));
 pDC->TextOutA(txt_x, position.y-170, txt);
 position.x = position.x + width + 4;

 //---- Y 
 pDC->SelectObject(&pen_green);						//select pancil
 pDC->MoveTo(position.x, position.y);				//Start point	 
 pDC->LineTo(position.x, position.y-y);				//draw line Y
 pDC->SelectObject(&pen_green_sl);					//select pancil
 pDC->MoveTo(position.x-l_wd,position.y-(SENSE.y>>1));//Start point
 pDC->LineTo(position.x+r_wd,position.y-(SENSE.y>>1));//draw Sens line X
 //Print text
 txt.Format("Sensitivity Y: %1.1f G", SENSE.y/42.667);
 pDC->SetTextColor(RGB(0,127,0));
 pDC->TextOutA(txt_x, position.y-158, txt);
 position.x = position.x + width + 4;

 //---- Z 
 pDC->SelectObject(&pen_blue);						//select pancil
 pDC->MoveTo(position.x, position.y);				//Start point	 
 pDC->LineTo(position.x, position.y-z);				//draw line Z
 pDC->SelectObject(&pen_blue_sl);					//select pancil
 pDC->MoveTo(position.x-l_wd,position.y-(SENSE.z>>1));//Start point
 pDC->LineTo(position.x+r_wd,position.y-(SENSE.z>>1));//draw Sens line X
 //Print text
 txt.Format("Sensitivity Z: %1.1f G", SENSE.z/42.667);
 pDC->SetTextColor(RGB(0,0,255));
 pDC->TextOutA(txt_x, position.y-146, txt);

 //----- delete all Pen
 //Выбираем старый Pen в DC (освобождаем свой Pen из DC)
 pDC->SelectObject(pOldPen);
 pen_red.DeleteObject();
 pen_red_sl.DeleteObject();
 pen_blue.DeleteObject();
 pen_blue_sl.DeleteObject();
 pen_green.DeleteObject();
 pen_green_sl.DeleteObject();	*/
}

//-----------------------------------------------------------------------------
//Paint
//-----------------------------------------------------------------------------
void CImages::Display_G(CDC *pDC, unsigned char G, unsigned char alarm_level)
{

 CPoint position;
 position.x = 100; 
 position.y = 296;

 //---- Set parameters for Pancil
 CPen pen_red, pen_red_sl;//, pen_blue, pen_grey;
 char width = 10;
 char l_wd =  5;
 char r_wd =  6;

 pen_red.CreatePen(PS_SOLID,width,	RGB(255,100,100));	//X красный
 pen_red_sl.CreatePen(PS_SOLID, 1,	RGB(255,100,100));	//X красный
 //pen_grey.CreatePen(PS_SOLID, 1,	RGB(255,0,0));		//Y зелёный
 //pen_blue.CreatePen(PS_SOLID, width,RGB(0,0,255));	//Z синий

 //---- Fill background 
 //pDC->FillSolidRect(position.x-l_wd, 100, 11, 100,RGB(30,30,30));

 //--------------------------------------------------------
 //Draw Level line
 //--------------------------------------------------------
 //---- G 
 //Save first pen pointer
 CPen *pOldPen = pDC->SelectObject(&pen_red);			//select pancil
 pDC->MoveTo(position.x, position.y);					//Start point	 
 pDC->LineTo(position.x, position.y-G);					//draw line X
 
 //--------------------------------------------------------
 //Draw Alarm threshold line
 //--------------------------------------------------------
 alarm_level >>= 1; 
 pDC->SelectObject(&pen_red_sl);						//select pancil
 pDC->MoveTo(position.x-l_wd, position.y-alarm_level);	//Start point	 
 pDC->LineTo(position.x+r_wd, position.y-alarm_level);	//draw line X

 //----- delete all Pen
 //Выбираем старый Pen в DC (освобождаем свой Pen из DC)
 pDC->SelectObject(pOldPen);
 pen_red.DeleteObject();
 pen_red_sl.DeleteObject();
 //pen_blue.DeleteObject();
 //pen_grey.DeleteObject();

}

//-----------------------------------------------------------------------------
//Paint Module G
//-----------------------------------------------------------------------------
void CImages::Curve_ModuleG(CDC *pDC, unsigned char *pBuffer)
{

 CPoint position;
 position.x = 0; 
 position.y = 255; //127 - Midle level line
 int pos = 0;
 int width = 400;
 //Pen Styles 
 //#define PS_DASH             1        -------  
 CPen pen_red, pen_red_dash;
 pen_red.CreatePen(PS_SOLID, 1,	RGB(255,0,0));		//X красный
 pen_red_dash.CreatePen(PS_DASH, 1,	RGB(255,0,0));	//X красный
 
 //Save first pen pointer
 CPen *pOldPen = pDC->SelectObject(&pen_red);		//select pancil

 //---- Write G-curve line
 pDC->MoveTo(position.x,position.y-pBuffer[0]);		//Start point	 
 do {								
        //draw line from this point to poin with new coordinate (x,y)
        pDC->LineTo(position.x+pos,position.y-pBuffer[pos]);
        pos++;
      
    } while (pos < width);
    
 //---- Write Sensing level line
 pDC->SelectObject(&pen_red_dash);					//select pancil
 pDC->MoveTo(position.x,position.y-SENSE.g);		//Start point
 pDC->LineTo(position.x+width,position.y-SENSE.g);	//draw

 //----- delete all Pen
 //Выбираем старый Pen в DC (освобождаем свой Pen из DC)
 pDC->SelectObject(pOldPen);
 pen_red.DeleteObject();
 pen_red_dash.DeleteObject();

}
/*
//-----------------------------------------------------------------------------
//Paint Searh Maximum Point
//-----------------------------------------------------------------------------
void CImages::MaxPoint(CDC *pDC, unsigned char *pBuffer, int size)
{
 CPoint position;
 position.x = 0; 
 position.y = 255; //127 - Midle level line
 
 int x_left = 20*8;
 int x_right = 20*8 + size;

 CPen pen_gray;
 pen_gray.CreatePen(PS_SOLID, 1, RGB(120,120,120));

 //---- Write Obserwation area line
 CPen *pOldPen = pDC->SelectObject(&pen_gray);		//select pancil
 pDC->MoveTo(position.x+x_left-1, position.y);		//Start point
 pDC->LineTo(position.x+x_left-1, position.y-200);	//draw
 pDC->MoveTo(position.x+x_right+1, position.y);		//Start point
 pDC->LineTo(position.x+x_right+1, position.y-200);	//draw
 
 //left/right arrows
 ARROWSTRUCT ARROW;
 ARROW.nWidth = 5;
 ARROW.fTheta = 0.4f;
 ARROW.bFill = true;
 ARROW.color = RGB(160,160,160);
 
 pDC->MoveTo(position.x+x_left-20, position.y-190);
 ArrowTo(pDC->m_hDC, x_left, position.y-190,&ARROW);
 pDC->MoveTo(position.x+x_right+20, position.y-190);
 ArrowTo(pDC->m_hDC, x_right, position.y-190,&ARROW);

 //---- Search Max Point
 CBrent BR;
 double x;
 int max_y = 255-(int)BR.brentoptimize(pBuffer, x_left, x_right, 0.5, x, SEARCH_MAX);
 int max_x = (int)x;
 
 //check G overhead 
 if(max_y > SENSE.g)
  {
    //msg1 = "Y max  = ";
    //double tmp = -1*yy/255.0;	
    //msg2.Format("%.3f",tmp);
    //memCUR.TextOutA(120,-45, msg1+msg2);	

    //msg1 = "X point = ";
    //tmp = xx * 0.1875;
    //msg2.Format("%.2f",tmp);
    //memCUR.TextOut(120,-30, msg1+msg2);

    //write text message
    CString txt = "max";
    pDC->SetTextColor(RGB(0,0,0));
    pDC->SetBkColor(RGB(255,255,111));//ETO_OPAQUE
    pDC->ExtTextOutA(position.x+max_x+4,position.y-max_y-40,ETO_CLIPPED,NULL,txt,NULL);
    //write arrow
    ARROW.color = RGB(255,0,0);
    pDC->MoveTo(position.x+max_x+8, position.y-(max_y+16));
    ArrowTo(pDC->m_hDC, position.x+max_x, position.y-max_y,&ARROW);
  }

 //----- delete all Pen
 //Выбираем старый Pen в DC (освобождаем свой Pen из DC)
 pDC->SelectObject(pOldPen);
 pen_gray.DeleteObject();

}
 // MyTimerProc - callback-функция, которая обрабатывает
 // сообщения WM_TIMER.
  */
