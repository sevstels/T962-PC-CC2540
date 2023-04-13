//T962.h : main header file for the PROJECT_NAME application
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "tec-resource.h"		// main symbols

// T962App:
// See T962.cpp for the implementation of this class
class T962App : public CWinAppEx
{
  public:
  T962App();

  //Overrides
  public:
  virtual BOOL InitInstance();

  //Implementation
  DECLARE_MESSAGE_MAP()
  afx_msg void OnContextHelp();
  virtual void HtmlHelp(DWORD_PTR dwData, UINT nCmd = 0x000F);
};
extern T962App theApp;
