//==============================================================================
//File name:   "dlg-monitor.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include "stdafx.h"
#include "afxwin.h"
#include "Page3.h"
#include "afxcmn.h"
#include "dialog-pos.h"

#ifndef _DLG_MONITOR_CLASS_H_
#define _DLG_MONITOR_CLASS_H_

#pragma once
                
//------------------------------------------------------------------------------
//Embedded dialog class 
//------------------------------------------------------------------------------
class CMonDlg : public CDialog
{
  DECLARE_DYNAMIC(CMonDlg)

  //Construction
  public:
  
  //---- Construction
  //standard constructor
  ///CMonDlg(CWnd* pParent, HANDLE hPage); 
  CMonDlg(); 
 ~CMonDlg();
  
  //Resource ID
  enum {IDD = IDD_MONITOR_TEMPR};

  //Implementation  
  CPage3 *pPage3;
  CDPOS POS;

  CDC *pDC;

  //----
  CProgressCtrl m_progress_temperature;
  CProgressCtrl m_progress_heater;
  CProgressCtrl m_progress_fan;
  
  //----
  CStatic m_static_temperature;
  CStatic m_static_heater;
  CStatic m_static_fan;

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

};

#endif
