//==============================================================================
//File name:   "dlg-.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include "stdafx.h"
#include "afxwin.h"
#include "Page4.h"
#include "afxcmn.h"
#include "tec-resource.h"  //for IDD
#include "dialog-pos.h"

#ifndef _DLG_ANLS_SETTINGS_CLASS_H_
#define _DLG_ANLS_SETTINGS_CLASS_H_

#pragma once
                
//------------------------------------------------------------------------------
//Embedded dialog class 
//------------------------------------------------------------------------------
class CAnGraphDlg : public CDialog
{
  DECLARE_DYNAMIC(CAnGraphDlg)
 
  //Construction
  public:
  
  //---- Construction
  //standard constructor
  ///CMonDlg(CWnd* pParent, HANDLE hPage); 
  CAnGraphDlg(); 
 ~CAnGraphDlg();
  
  //Resource ID
  enum {IDD = IDD_SENSOR_SETUP_DIALOG};

  //Implementation  
  CPage4 *pPage4;
  CDPOS POS;

  //----
  CProgressCtrl m_progress_temperature;
  CProgressCtrl m_progress_power;
  CStatic m_static_temperature;
  CStatic m_static_current;

  //----func
  void CloseDlg(void);
  void IniPointers(HANDLE hClass);

  //DDX/DDV support 
  virtual BOOL OnInitDialog(void);
  virtual void DoDataExchange(CDataExchange* pDX);  
  DECLARE_MESSAGE_MAP()

  public:
  virtual void OnOK();
  virtual void OnCancel();
  
  afx_msg void OnBnClickedOk();
  afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);

  protected:
  unsigned char enable;

  public:  
 
};

#endif
