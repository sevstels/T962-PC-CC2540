//==============================================================================
//File name:   "Page5.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include "afxcmn.h"
#include "afxwin.h"
#include "tec-resource.h"  //for IDD_Page5
#include "device_struct.h"
#include "log-console.h"
#include "AppWinReg.h"
#include "tooltip.h"
#include "datatx.h"
#include "app-setup.h"
#include "dlg_sensor.h"

#ifndef _PAGE5_H_
#define _PAGE5_H_

#pragma once

//------------------------------------------------------------------------------
//CPage6 dialog
//------------------------------------------------------------------------------
class CPage5 : public CPropertyPage
{
  DECLARE_DYNCREATE(CPage5)

  //---- Construction
  public:
  CPage5();
 ~CPage5();
  
  //---- Dialog Data
  enum {IDD = IDD_Page5};
 
  //----
  CDataTX *pBT;
  CAppWinReg *pREG;
  ApplSettings *pAPP;
  Device_Setup2 *pDevice;
  CSensorSetupDlg *pDlg[4];

  //----   
  CComboBox m_combo_ts_calc;
  CComboBox m_combo_htr_calc;
  CComboBox m_combo_ts[4];
  CButton m_check_pcbs[4];
  CButton m_check_htrs[2];

  //----
  void Controls_Ini(void);
  void Controls_Enable(BOOL on_off);
  void Controls_Update(int number);
  
  //----
  void Sensors_Enable(void);
  void Sensors_CalcMode(void);
  void Sensors_SetSetupDefault(void *pSetup);
  void OpenSensorDlg(int chn);

  //---- Overrides
  protected:
  //DDX/DDV support
  virtual void DoDataExchange(CDataExchange* pDX);
  //Generated message map functions
  DECLARE_MESSAGE_MAP()
  
  //----
  public:
  virtual BOOL OnInitDialog();
  //virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
  afx_msg void OnCheckTsTopLeft();
  afx_msg void OnCheckTsTopRight();
  afx_msg void OnButtonCfgTS1();
  afx_msg void OnButtonCfgTS2();
  afx_msg void OnButtonCfgTS3();
  afx_msg void OnButtonCfgTS4();

  afx_msg void OnCheckTS1();
  afx_msg void OnCheckTS2();
  afx_msg void OnCheckTS3();
  afx_msg void OnCheckTS4();
  
  afx_msg void OnComboCalcHTR();
  afx_msg void OnComboCalcTS();
  
};

#endif
