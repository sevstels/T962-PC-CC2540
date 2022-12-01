// PIDPC.h : main header file for the PROJECT_NAME application
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "tec-resource.h"		// main symbols

// CPIDPCApp:
// See PIDPC.cpp for the implementation of this class
class CPIDPCApp : public CWinAppEx
{
  public:
  CPIDPCApp();

  //Overrides
  public:
  virtual BOOL InitInstance();

  //Implementation
  DECLARE_MESSAGE_MAP()
  afx_msg void OnContextHelp();
  virtual void HtmlHelp(DWORD_PTR dwData, UINT nCmd = 0x000F);
};
extern CPIDPCApp theApp;
