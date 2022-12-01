//==============================================================================
//File name:   "Page6.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include <afxwin.h>
#include <afxcmn.h>
#include <afxmsg_.h>
#include "tec-resource.h"  //for IDD_Page6
#include "device_struct.h"
#include "log-console.h"
#include "AppWinReg.h"
#include "tooltip.h"
 
#include "app-setup.h"
#include "datatx.h"

#ifndef _PAGE_6_H_
#define _PAGE_6_H_

#pragma once

//------------------------------------------------------------------------------
//CPage6 dialog
//------------------------------------------------------------------------------
class CPage6 : public CPropertyPage
{
  DECLARE_DYNCREATE(CPage6)

  //---- Construction
  public:
  CPage6();
 ~CPage6();
  
  //---- Dialog Data
  enum {IDD = IDD_Page6};
  
  ///CMNT *pMonitoring;
  Device_Setup *pDevice;
  CLOGW	*pConsole;
  CC2540 *pBLE;
  CAppWinReg *pREG;
  ApplSettings *pAPP;
  CApplSetup *pCSETUP;
  CDataTX *pBT;
  CEdit m_edit_info;
  CString txt_info;
  CProgressCtrl m_progress;
  
  //----
  CWinThread *pThread;
  CEvent ev_Quit;	
  CEvent ev_WaitEnd;
  CEvent ev_ExitRequest;
  CEvent ev_DataAccepted;
  void ThreadLaunch(void);
  void ThreadStop(void);

  //----
  void Controls_Ini(void);
  void Controls_Enable(BOOL on_off);
  void Controls_Update(int number);
  void PrintInfo(std::string *pTxt);
  void Clear(void);
  
  //bootloader
  CEvent ev_BootDataRx;
  void ParseBootMsg(char *pBuf, int length);
  std::string boot_txt;

  //---- Overrides
  protected:
  //DDX/DDV support
  virtual void DoDataExchange(CDataExchange* pDX);
  //Generated message map functions
  DECLARE_MESSAGE_MAP()
  
  //----
  public:				 
  virtual BOOL OnInitDialog();
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  //---- 
  afx_msg void OnButtonAbout();
  afx_msg void OnButtonFWInfo();
  afx_msg void OnButtonHelp();
  afx_msg void OnButtonProfileList();
  afx_msg void OnButtonSensorsList();
  afx_msg void OnButtonShowEeprom();
  afx_msg void OnShowSettings();
  afx_msg void OnButtonEepromTest();
  afx_msg void OnButtonEepromErase();
  afx_msg void OnButtonSetupDefault();
  afx_msg void OnButtonCurrProfile();
  afx_msg void OnButtonEraseCustomProfile();
  afx_msg void OnButtonBtInfo();
  afx_msg void OnButtonFWUpdate();

};

#endif
