 //==============================================================================
//File name:   "about_dlg.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include <afxwin.h>
#include <afxcmn.h>
#include <afxmsg_.h>

#include "tec-resource.h"  //for IDD

#ifndef _ABOUT_DLG_H_
#define _ABOUT_DLG_H_

//------------------------------------------------------------------------------
//Function:	CAboutDlg dialog used for App About
//------------------------------------------------------------------------------
class CAboutDlg : public CDialogEx
{
  public:
  CAboutDlg();

  // Dialog Data
  enum { IDD = IDD_ABOUTBOX };

  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  
  //Implementation
  protected:
  DECLARE_MESSAGE_MAP()
  
  public:
  CStatic m_txt;
  virtual BOOL OnInitDialog();
};

#endif
