//==============================================================================
//File name:    "scroll.cpp"
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
//void CDSPL::Scroll_Handler(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
void CDSPL::Scroll_Handler(UINT nSBCode, UINT nPos)
{  
 //Conversion to CSliderCtrl*
 //CSliderCtrl *pSlider = (CSliderCtrl*)pScrollBar;
 //Get current hWnd
 //HWND hWnd = pSlider->m_hWnd;
 //if(hWnd == m_Scroll.m_hWnd)

 //========================================= 
 //check by slider hWnd, Slider Move Graph
 //=========================================

  //Scrollbar Current Position
  int x_min, x_max;
  scroll_step = 0;
  int direction = 0;
  int position;

  pScroll->GetScrollRange(&x_min, &x_max); 

  //Select the Mode of Scrolling and set the Increment position
  //variable accordingly
  switch (nSBCode)
   {
    case SB_LEFT:      
       //scroll_position = x_min; 
       break;
    
    case SB_RIGHT:     
       //scroll_position = x_max; 
       break;
    
    case SB_LINELEFT:  
       scroll_step = -1; 
       direction = -1;
       break; 								
        
    case SB_LINERIGHT: 
       scroll_step = 1;
       direction = 1;
       break;

    case SB_PAGELEFT:  
       scroll_step = -20;
       direction = -1;
       break;

    case SB_PAGERIGHT: 
       scroll_step = 20; 			
       direction = 1;
       break; 
        
    case SB_THUMBTRACK:
       scroll_step = nPos - scroll_position;
       if(scroll_step>0){direction = 1;}
       if(scroll_step<0){direction =-1;}
       break;

    //release handle
	//Генерируется при перемещении ползунка на новую позицию
    case SB_THUMBPOSITION:
       break;

    case SB_RESET:
	  //Reset all 
	  scroll_position = 0;
      scroll_step = 0;
	  direction = 1;
	  break;

    default:  
	  //Reset all 
	  //scroll_position = 0;
      scroll_step = 0;
    }
  	
  if(direction!=0)
  {
    if(nSBCode!=SB_THUMBTRACK) scroll_position += scroll_step;
    else scroll_position = nPos;
	
	//check track limits
    if(scroll_position>x_max){scroll_position = x_max; scroll_step=0;}
    if(scroll_position<x_min){scroll_position = x_min; scroll_step=0;}
    
    pScroll->SetScrollPos(scroll_position, TRUE);

    if(direction==-1)
	   ev_ScrollLeft.SetEvent();

    if(direction==1) 
	   ev_ScrollRight.SetEvent();

    //TRACE2("\nPos %d   Step %d", scroll_position, step);
  }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CDSPL::Scroll_Setup(void)
{  
  int length = profile.size();
  int last_x = profile[length-1].x;
  int half_display = scale_width_x>>3;

  //---- Scroll cotrol setup
  pScroll->SetScrollRange(0, last_x+half_display, 1); 
  //m_ScrollMux = 1;
  
  unsigned int current_pos = pScroll->GetScrollPos();
  if(current_pos>(length-1))
  {
    current_pos = 0;
	pScroll->SetScrollPos(current_pos, 1);
  }

  scroll_position = current_pos;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------  
void CDSPL::Scroll_Left(void)
{
  //scroll_index = scroll_position;
  ScaleRangeX(scroll_position, scroll_position+340, SCALE_X_CELLS_NUM);
  ///TRACE2("\nScroll Left, Pos %d  Step %d", scroll_position, scroll_step);
  ev_Redraw.SetEvent();
}

//------------------------------------------------------------------------------
//scrool screen buffer
//------------------------------------------------------------------------------  
void CDSPL::Scroll_Right(void)
{
  //shift screen buffer				
  //memcpy(ProfileBuf,(void*)&(ProfileBuf[scroll_position]), 300);
  
  //scroll_index = scroll_position;
  ScaleRangeX(scroll_position, scroll_position+340, SCALE_X_CELLS_NUM);
  ///TRACE2("\nScroll Right, Pos %d  Step %d", scroll_position, scroll_step);
  ev_Redraw.SetEvent();
}
