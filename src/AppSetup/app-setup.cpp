//==============================================================================
//File name:   "app-setup.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "afxwin.h"
#include "app-setup.h"
#include "AppWinReg.h"

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
//Constructor
//------------------------------------------------------------------------------
CApplSetup::CApplSetup()
{					
  pSetup = NULL;
  hREG = NULL; 
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
CApplSetup::~CApplSetup()
{
  pSetup = NULL;
  hREG = NULL;
}

//------------------------------------------------------------------------------
//        
//------------------------------------------------------------------------------
void CApplSetup::FirstIni(void)
{
  if(pSetup==NULL) return;
  //----
  unsigned short length = pSetup->struct_length;
  memset(pSetup, 0x00, length);
  pSetup->struct_length = length;

  //----
  SetDefault();
  Save();
}

//------------------------------------------------------------------------------
//        
//------------------------------------------------------------------------------
void CApplSetup::SetDefault(void)
{	
  pSetup->log[0] = 1;
  pSetup->log[1] = 1;
  pSetup->log[2] = 1;
  pSetup->log[3] = 1;
  pSetup->log[4] = 1;
  pSetup->log[5] = 1;
  pSetup->log[6] = 1;
  pSetup->log[7] = 1;
  pSetup->log[8] = 1;
  pSetup->log[9] = 1;

  //---- graphic
  pSetup->line_color[0] = RGB(80,80,80);
  pSetup->line_color[1] = RGB(255,0,0);
  pSetup->line_color[2] = RGB(128,0,0);
  pSetup->line_color[3] = RGB(250,180,100);
  pSetup->line_color[4] = RGB(250,250,100);
  pSetup->line_color[5] = RGB(100,250,250);
  pSetup->line_color[6] = RGB(100,100,250);
  pSetup->line_color[7] = RGB(0,200,0);   //TPCB
  pSetup->line_color[8] = RGB(250,255,0); //PWM HTR
  pSetup->line_color[9] = RGB(0,0,255);   //PWM FAN
  
  //----
  pSetup->bgnd_color = RGB(255,255,255);
  pSetup->scale_color = RGB(0,0,0);
 
  //----
  pSetup->line_width[0] = 2;
  pSetup->line_width[1] = 1;
  pSetup->line_width[2] = 1;
  pSetup->line_width[3] = 1;
  pSetup->line_width[4] = 1;
  pSetup->line_width[5] = 1;
  pSetup->line_width[6] = 1;
  pSetup->line_width[7] = 2;
  pSetup->line_width[8] = 2;
  pSetup->line_width[9] = 2;
  	
  //----
  //int graph_max_value_x;
  //int graph_max_value_y;
  //int graph_line_width;
 
  //---- dsp
  pSetup->dsp_filter_LPF = 4;
  pSetup->dsp_filter_Bezier = 0;
  pSetup->dsp_normalize = 0;
  pSetup->dsp_img_dimension = 3;
  pSetup->dsp_img_heigth = 600;
  pSetup->dsp_img_width = -1;//AUTO
  pSetup->app_sync = 0;

  //----
  pSetup->dsp_chn_On[0] = 1;
  pSetup->dsp_chn_On[1] = 0;
  pSetup->dsp_chn_On[2] = 0;
  pSetup->dsp_chn_On[3] = 0;
  pSetup->dsp_chn_On[4] = 0;
  pSetup->dsp_chn_On[5] = 0;

  //---- open for user
  pSetup->show_img = 1;
  pSetup->show_txt = 0;
  pSetup->show_csv = 0;//excel;
  pSetup->show_cpp = 0;//for svg

  //---- save img
  pSetup->save_gif = 0;
  pSetup->save_png = 1;
  pSetup->save_jpg = 0;
  pSetup->save_bmp = 0; 
  
  //---- save file
  pSetup->save_txt = 0;
  pSetup->save_csv = 1;
  pSetup->save_svg = 0;
  pSetup->save_cpp = 0;
  
  //bt
  pSetup->bt_last_com = 1;
  pSetup->bt_enable = 0;

  //----
  memset(pSetup->bt_dev_address, 0, 8);
	    
}

//------------------------------------------------------------------------------
//        
//------------------------------------------------------------------------------
void CApplSetup::Save(void)
{
  if(hREG==NULL) return; 
  CAppWinReg *pREG = (CAppWinReg*)hREG;
  const char *pBuf = (const char*)pSetup;
  pREG->SetBinVar("AppSetup", pBuf, pSetup->struct_length);
}

//------------------------------------------------------------------------------
//        
//------------------------------------------------------------------------------
void CApplSetup::Read(void)
{
  if(hREG==NULL) return; 
  CAppWinReg *pREG = (CAppWinReg*)hREG;
  
  int result;
  result = pREG->GetBinVar("AppSetup", (char*)pSetup, pSetup->struct_length);

  if(result==0)
  {
  	FirstIni();
	pREG->SetWndColor();
  }
}
