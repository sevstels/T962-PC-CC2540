//==============================================================================
//File name:    "controls6.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page6.h"

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
//Function:
//------------------------------------------------------------------------------
void CPage6::Controls_Ini(void)
{
  //----
  m_progress.SetRange(0, 256); //from to	
  m_progress.SetPos(0);
  m_progress.SetStep(1); //Set the 1 step increment
  m_progress.SetBarColor(RGB(100, 100, 100)); //color
  m_progress.SetBkColor(RGB(0xff, 0xff, 0xff)); //color
  m_progress.ShowWindow(TRUE); //Show window
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage6::Progress_Ini(unsigned long bytes)
{
  unsigned int steps = bytes/45; // div 45 bytes by block
  //----
  m_progress.SetRange(0, steps); //from to	
  m_progress.SetPos(0);
  m_progress.SetStep(1); //Set the 1 step increment
  m_progress.SetBarColor(RGB(100, 100, 100)); //color
  m_progress.SetBkColor(RGB(0xff, 0xff, 0xff)); //color
  m_progress.ShowWindow(TRUE); //Show window
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage6::Progress_Error(void)
{
  m_progress.SetBarColor(RGB(255, 0, 0)); //color
  m_progress.SetBkColor(RGB(0xff, 0xff, 0xff)); //color
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage6::Progress_Step(void)
{
  //Do the 1 step
  m_progress.StepIt(); 
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage6::Controls_Enable(int on_off)
{
} 

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage6::Controls_Update(void)
{ 
}	
