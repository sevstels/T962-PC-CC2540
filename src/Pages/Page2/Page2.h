//==============================================================================
//File name:   "Page2.h"
//Purpose:      Header File
//Version:      2.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include "afxwin.h"
#include "afxcmn.h"
#include "tec-resource.h"  //for IDD_Page2
#include "device_struct.h"
#include "log-console.h"
#include "AppWinReg.h"
#include "tooltip.h"
#include "app-setup.h"
#include "protocol-ex.h"
 
#include "datatx.h"

#ifndef _PAGE_2_H_
#define _PAGE_2_H_

#pragma once

//------------------------------------------------------------------------------
// CPage2 dialog
//------------------------------------------------------------------------------
class CPage2 : public CPropertyPage
{
  DECLARE_DYNCREATE(CPage2)

  //---- Construction
  public:
  CPage2();
 ~CPage2();

  enum {IDD = IDD_Page2};
    
  CDataTX *pBT;
  CLOGW	*pConsole;
  Device_Setup *pDevice;

  CC2540 *pBLE;
  CAppWinReg *pREG;
  CToolTip ToolTip;
  ApplSettings *pAPP;

  //Buttons
  CButton m_but_test_step;
  CButton m_but_test_linerity;
  CButton m_but_get_setup;
  CButton m_but_set_default;
  CButton m_but_PID_test;
  CButton m_but_PID_Manual;
  CButton m_but_PID_AutoTune;

  //Check Box`s
  CButton m_check_LogFix;
  CButton m_radio_tuning;
  CButton m_radio_normal;

  //Static controls
  CStatic m_txt_CH1_KP;
  CStatic m_txt_CH1_KI;
  CStatic m_txt_CH1_KD;
  CStatic m_txt_chn;
  CStatic m_txt_ref;

  //Edit controls	
  CEdit m_KP, m_KI, m_KD, m_REF;
  CEdit m_FAN_Gain;
  CEdit m_PID_Period;

  void Controls_Ini(void);
  void Controls_Enable(BOOL on_off);
  void Controls_Update(void);
  void Controls_PIDDisable(void);
  void Controls_PIDEnable(bool on_off);
  void Controls_PIDWind(bool on_off);
  
  //----
  int  ParseErrValue(CEdit *pEdit, float &value);
  void ShowMode(char number);
  int  EditBoxs(MSG* pMsg);
  int  ParseDetectPeriod(void);

  //---- 
  void PID_TestFileTxt(void);
  void PID_SetMode(int mode);
  //----
  void PID_TuneOpenFile(void); 
  void PID_TuneSaveSample(int adc_value, int actuator_value, int period_ms);
  
  //var  
  int PID_log;
  signed short  DAC_temp;

  protected:
  unsigned int PID_Period;
  int monitoring;
  int PID_sample_counter;
  int PID_test_log;
  int PID_tune_log;
  unsigned long PID_lock_interval;
  short *pSample;

  //---- functions
  void prfloat(unsigned char *pBuf, float data);
  
  //==== Time / Freq calculation ================
  LARGE_INTEGER Frequency, tLast, tPeriod; 
  void Calc_Freq(void);
  unsigned long Calc_Delay(LARGE_INTEGER *pTimeStart, LARGE_INTEGER *pTimeEnd);
  unsigned long Calc_Time(LARGE_INTEGER *pTime);

  //---- Implementation
  virtual void DoDataExchange(CDataExchange* pDX);
  DECLARE_MESSAGE_MAP()

  public:
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedCheckPage2UseKp();
  afx_msg void OnBnClickedCheckPage2UseKi();
  afx_msg void OnBnClickedCheckPage2UseKd();
  //----
  afx_msg void OnModePidEnable();
  afx_msg void OnModePidDisable();
  afx_msg void OnModeRemoteDAC();
  //----
  afx_msg void OnButtonSetDefault();
  afx_msg void OnButtonPidTest();  
  afx_msg void OnButtonTestStep();
  afx_msg void OnButtonTestLinerity();
  afx_msg void OnCheckFixLog();
  //----
  afx_msg void OnRadioPidModeManual();
  afx_msg void OnRadioPidModeAuto();
  afx_msg void OnButtonPidManualRun();
  //----
  afx_msg void OnCheckChannelEnable();
  afx_msg void OnCheckReflowLogFix();
 
};

#endif


