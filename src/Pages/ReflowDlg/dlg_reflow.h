//==============================================================================
//File name:   "dlg_reflow.h"
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
#include "device_struct.h"

#ifndef _DLG_REFLOW_SETUP_CLASS_H_
#define _DLG_REFLOW_SETUP_CLASS_H_

//a custom Windows message
#define UWM_DIALOG_MESSAGES (WM_APP + 2)

//----
#define DEV_DIALG_CLOSE     1
#define DEV_SAVE_SETUP	    2
#define DEV_SET_SENSOR_TYPE 3
#define DEV_SET_FILTER      4
#define DEV_SET_MSR_RES	    5
#define DEV_SET_AMB_RES     6
#define DEV_SET_BURST	    7
#define DEV_SET_MODE        8 
#define DEV_SET_STP_DEFAULT	9
#define DEV_SET_CALIBR	    10

#pragma once                
//------------------------------------------------------------------------------
//Embedded dialog class 
//------------------------------------------------------------------------------
class CReflowSetupDlg : public CDialog
{
  DECLARE_DYNAMIC(CReflowSetupDlg)

  //Construction
  public:
  
  //---- Construction
  //standard constructor
  CReflowSetupDlg(); 
 ~CReflowSetupDlg();
  
  //Resource ID
  enum {IDD = IDD_REFLOW_SETUP_DIALOG};

  //Implementation
  CDPOS POS;
  CToolTip ToolTip;
  CAppWinReg *pREG;
  Device_Setup2 *pDev;
  CWnd *pCWnd;

  //----
  HBITMAP hLED_red;
  HBITMAP hLED_grey;
  HBITMAP hLED_green;
  HBITMAP hLED_yellow;

  //----
  int chn;
  int enabled;
  unsigned char status_old;

  //----
  CComboBox m_combo_sns_types;
  CComboBox m_combo_filter;
  CComboBox m_combo_measure_res;
  CComboBox m_combo_ambient_res;
  CComboBox m_combo_mode;
  CComboBox m_combo_burst;
  CComboBox m_combo_align_mode;

  //----
  CEdit m_edit_pause;
  CEdit m_edit_prehit_time;
  CEdit m_edit_prehit_temper;

  CButton m_but_default;
  CStatic m_status[7];
  CStatic m_status_txt[7];

  //----func
  void CloseDlg(void);
  void SetTooltip(void);
  void Controls_Ini(void);
  void Controls_Enable(BOOL on_off);
  void Controls_Update(void);
  int  ParseValue(CEdit *pEdit, short min, int max);
  int  ParseValueFl(CEdit *pEdit, float min, float max, float &ret);
  void LED_Control(unsigned char status);

  protected:
  DECLARE_MESSAGE_MAP()

  public:
  //DDX/DDV support 
  virtual void OnOK();  
  virtual void OnCancel();  
  virtual BOOL OnInitDialog(void);
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual void DoDataExchange(CDataExchange* pDX);
  afx_msg void OnComboMode();
  afx_msg void OnComboBurst();
  afx_msg void OnComboTypes();
  afx_msg void OnComboFilter();
  afx_msg void OnBnClickedOk();
  afx_msg void OnButtonSetDefault();
  afx_msg void OnComboMeasureResolution();
  afx_msg void OnComboAmbientResolution();
  afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
  
};

#endif
