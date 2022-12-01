//==============================================================================
//File name:   "dlg_pid-test.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include "stdafx.h"
#include "afxwin.h"
#include "Page2.h"

#ifndef _PID_TEST_DLG_CLASS_H_
#define _PID_TEST_DLG_CLASS_H_

#pragma once
                
//------------------------------------------------------------------------------
//Embedded dialog class 
//------------------------------------------------------------------------------
class CPidTestDlg : public CDialog
{
  DECLARE_DYNAMIC(CPidTestDlg)

  //Construction
  public:
  
  //---- Construction
  //standard constructor
  CPidTestDlg(CWnd* pParent, HANDLE hPage); 
  virtual ~CPidTestDlg();
  
  //Resource ID
  enum {IDD = IDD_PID_TEST_DIALOG};

  //Implementation  
  CPage2 *pPage2;
  
  //----
  CComboBox m_combo_function;
  CEdit m_edit_max;
  CEdit m_edit_min;
  CEdit m_edit_period;
  CEdit m_edit_cycles;
  CButton m_butt_testrun;

  //----func
  void CloseDlg(void);
  int ParseValue(CEdit *pEdit, short min, short max);

  //DDX/DDV support 
  virtual BOOL OnInitDialog(void);
  virtual void DoDataExchange(CDataExchange* pDX);  
  DECLARE_MESSAGE_MAP()

  public:
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual void OnCancel();
  afx_msg void OnBnClickedOk();
  virtual void OnOK();

  protected:
  unsigned char enable;

  public:
  afx_msg void OnButtonTestOnoff();
  afx_msg void OnComboTestFunction();
};

#endif
