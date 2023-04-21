//==============================================================================
//File name:   "Page6.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
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
  CToolTip ToolTip;
  CEdit m_edit_info;
  CString txt_info;
  CProgressCtrl m_progress;
  CTabCtrl *pTab;

  //---- 
  CButton m_but_eep_show;
  CButton m_but_eep_erase;
  CButton m_but_eep_test;
  CButton m_but_fw_update;
  CButton m_but_bt_info;
  CButton m_but_fw_info;
  CButton m_but_help;
  CButton m_but_profiles;
  CButton m_but_profile;
  CButton m_but_ers_profile;
  CButton m_but_sensors;
  CButton m_but_settings;
  CButton m_but_resetsetup;

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
  void Controls_Update(void);
  void PrintInfo(std::string *pTxt);
  void Progress_Ini(unsigned long bytes);
  void Progress_Step(void);
  void Progress_Error(void);
  void Clear(void);
  void Test(void);
  void I2C_Read(unsigned char dev_addr, unsigned short mem_addr, short len);
  void I2C_Write(unsigned char dev_addr, unsigned short addr, char *pBuf, short len); 
  
  //bootloader
  CEvent ev_BootDataRx;
  void ParseBootMsg(char *pBuf, int length);
  std::string bootloader_msg;
  int program_run;
  int dfu_length;
  char *pDfuBuf;

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

  afx_msg void OnButtonGetSerial();
};

#endif
