//==============================================================================
//File name:   "dlg.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru
//==============================================================================
#include "stdafx.h"
#include "afxwin.h"
#include "tooltip.h"
#include "tec-resource.h"
#include "AppWinReg.h"
#include "dialog-pos.h"
#include "cstatic-color.h"
#include "app-setup.h"
#include "cc2540.h"

#ifndef _DLG_BT_SETUP_CLASS_H_
#define _DLG_BT_SETUP_CLASS_H_

//a custom Windows message
#define UWM_DIALOG_MESSAGES (WM_APP + 2) 

//----
#define DLG_CLOSE  0
#define DLG_SENSOR 2
#define DLG_WRITE_DEVSETUP 2

#define CMD_SET_COM         0x03
#define CMD_BT_SCAN         0x04
#define CMD_BT_CONNECT      0x05
#define CMD_BT_DISCONNECT	0x06	  

#pragma once                
//------------------------------------------------------------------------------
//Embedded dialog class 
//------------------------------------------------------------------------------
class CBTDlg : public CDialog
{
  DECLARE_DYNAMIC(CBTDlg)

  //Construction
  public:
  
  //---- Construction
  //standard constructor
  CBTDlg(); 
 ~CBTDlg();
  
  //Resource ID
  enum {IDD = IDD_DIALOG_BLE};

  //Implementation
  CAppWinReg *pREG;
  CToolTip ToolTip;
  CDPOS POS;
  HWND hWndParent;
  CC2540 *pBLE;

  //----
  CEdit m_edit_com;
  CEdit m_edit_addr;
  CListBox m_Monitor;
  //----
  CButton m_but_connect;
  CButton m_but_disconnect;
  CButton m_but_scan;

  //----func
  void CloseDlg(void);
  void SetTooltip(void);
  void Controls_Enable(BOOL on_off);
  void Controls_Update(void);
  void DeviceInfo(void);

  //----
  protected:
  DECLARE_MESSAGE_MAP()

  public:
  //DDX/DDV support 
  virtual BOOL OnInitDialog(void);
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual void DoDataExchange(CDataExchange* pDX);  
  virtual void OnOK();  
  virtual void OnCancel();
  afx_msg void OnBnClickedOk();
  afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
   
  afx_msg void OnButtonScan();  
  afx_msg void OnButtonConnect();
  afx_msg void OnButtonDisconnect();
  afx_msg void OnButtonCom();

};

#endif
