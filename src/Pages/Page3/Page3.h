//==============================================================================
//File name:   "Page3.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include "afxcmn.h"
#include "afxwin.h"
#include "integrator.h"
#include "tec-resource.h"
#include "device_struct.h"
#include "ColorStaticST.h"
#include "AppWinReg.h"
#include "tooltip.h"
#include "app-setup.h"
#include "datatx.h"

#ifndef _PAGE3_H_
#define _PAGE3_H_

#define ID_TIMER_WARNINGS  3

#pragma once
//------------------------------------------------------------------------------
//CPage3 dialog
//------------------------------------------------------------------------------
class CPage3 : public CPropertyPage
{
  DECLARE_DYNCREATE(CPage3)

  //---- Construction
  public:
  CPage3();
 ~CPage3();
  
  //---- Dialog Data
  enum {IDD = IDD_Page3};
  
  //----
  CDataTX *pBT;
  CINTG INTG_TEMP;
  CINTG INTG_HTRPWR;
  CINTG INTG_FANPWM;
  CINTG INTG_HTRPWM;

  //----
  CAppWinReg *pREG;
  CToolTip ToolTip;
  Device_Setup2 *pDevice;
  Device_Params *pParams;
  ApplSettings *pAPP;

  void TabOpen(void);

  //----
  virtual BOOL OnInitDialog();
  void Controls_Ini(void);
  void Controls_Enable(int on_off);
  void Controls_Update(void);
  void Manual_Enable(int on_off);
  void MonitoringParser(char *pBuf);
  void Monitoring(int on);
  void DeletePanel(void);

  //----
  void Show_Temperature(void);  
  void Show_HeaterPower(void);
  void Show_FanSpeed(void);
  void ShowCurrent(CEdit *pEdit, unsigned short current);
  int  Calc_HeaterPowerLimit(void);

  //----
  void ShowWarningMsg(CString *pTxt);
  void ShowOkMsg(CString *pTxt);
  int  CheckTemperatureSensor(int adc_code);
  unsigned short CalcCurrentLimiter(int limit_ma);

  //----
  CSliderCtrl m_slider_fan;
  CSliderCtrl m_slider_heater;

  //----
  CProgressCtrl m_progress_temperature;
  CProgressCtrl m_progress_heater;
  //----
  CProgressCtrl *pPanel_temperature;
  CProgressCtrl *pPanel_heater;
  CProgressCtrl *pPanel_fan;

  //----
  CStatic m_txt_power;
  CStatic m_txt_temperature;
  CStatic *pPanel_Txt_temperature;
  CStatic *pPanel_Txt_heater;
  CStatic *pPanel_Txt_fan;
  HANDLE hPanelDlg;

  //----
  CColorStaticST m_static_warnings;
  
  //----
  CEdit m_edit_fan;
  CEdit m_edit_power_limit;
  CEdit m_edit_heater_power;
  CEdit m_edit_temperature_limit;
  
  //----
  CButton m_check_manual;

  //---- Overrides
  protected:
  //DDX/DDV support
  virtual void DoDataExchange(CDataExchange* pDX);
  //Generated message map functions
  DECLARE_MESSAGE_MAP()
  int hTimer;
  int slider_old_pwr;
  int slider_old_fan;
  UINT MM_TimerID;

  public:
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnCheckManual();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnButtonPanel();
};

#endif