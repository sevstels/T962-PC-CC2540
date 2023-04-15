//==============================================================================
//File name:   "Page4.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include "afxcmn.h"
#include "afxwin.h"
#include "tec-resource.h"  //for IDD_Page4
#include "device_struct.h"
#include "anls-graph.h"
#include "display.h"
#include "graphs.h"
#include "profile-file.h"
#include "log-file.h"
#include "log-console.h"
#include "AppWinReg.h"
#include "tooltip.h"
#include "Page2.h"
#include "Page3.h" 
#include "app-setup.h"
#include "dlg_reflow.h"

#ifndef _PAGE4_H_
#define _PAGE4_H_

#pragma once

//------------------------------------------------------------------------------
//CPage4 dialog
//------------------------------------------------------------------------------
class CPage4 : public CPropertyPage
{
  DECLARE_DYNCREATE(CPage4)

  //---- Construction
  public:
  CPage4();
 ~CPage4();
  
  //---- Dialog Data
  enum {IDD = IDD_Page4};
  unsigned char CAN_device_ID;
  int device_online;
  int load_profile;
  int PID_log;
  int EVNT_log;
  int block_writed;

  Device_Setup2 *pDevice;
  CReflowSetupDlg *pDlg;

  //----
  CDataTX *pBT;
  CPage2 *pPage2;
  CPage3 *pPage3;
  CC2540 *pBLE;
  CLOGW	*pConsole;
  
  ApplSettings *pAPP;
  CApplSetup *pCSETUP;
  CAppWinReg *pREG;  
  CDSPL	DSPL;
  CTPRF PCRF;
  CAnGraph AGRPH;
  CEdit m_monitor;
  CToolTip ToolTip;
  CGraphs GRPH;
  CLOGFILE LOG_EEP;
  CLOGFILE LOG_RFLW;
  CEvent ev_Quit;
  
  //----
  CButton m_but_FileOpen;
  CButton m_but_FileSave;
  CButton m_but_FileShow;
  CButton m_but_ShowGraph;
  CButton m_but_ProfileRead;
  CButton m_but_ProfileWrite;
  CButton m_but_Settings;
  CButton m_but_ReflowRun;
  CButton m_but_ReflowPause;
  CButton m_but_FromBegin;
  CButton m_but_Analysis;
  CComboBox m_combo_SelectProfile;
  
  //----
  CButton m_check_show_log_console;
  CButton m_check_write_log_file;

  //----
  CProgressCtrl *pProgress;
  CProgressCtrl m_Progress_Write;
  CScrollBar m_Scroll;
   
  //----
  void ThreadLaunch(void);
  void ThreadStop(void);
  void TabOpen(void);
  void Controls_Ini(void);
  void Controls_Enable(BOOL on_off);
  void Controls_Update(void);
  void ControlsGrop(int on_off);
  int  DeviceProfileWrite(void);
  int  DeviceProfileRead(void);
  int  AnalysisRead(void);
  int  AnalysisGraph(HANDLE pArray);
  int  WriteDevSetup(void);
  void Parser(char *pBuf);
  void ButtonSoldering(int state);
  void ParseLED(unsigned char leds, CString *pTxt);
  void ParseProfile(char *pBuf, int length);
  void ParseReflowStep(char *pBuf, int length);
  void ParseReflowLog(char *pBuf, int length);
  void LoadProfile(void);

  void LogFile_DevicePCR(HANDLE pArray);
  void LogFile_Reflow(HANDLE pArray);
  
  //----
  void PID_LogGraphIni(void);
  void PID_LogGraph(int point, int adc_code);
  void PID_LogFile(int point, int adc_code);
  void PID_LogFileCreate(void);
  //----
  void PCR_CheckLogFile(void);
  void PCR_LogFileCreate(void);
  void PCR_LogFile(int intensity, char leds);
  //----
  //void SMPL_CheckLogFile(void);
  //void SMPL_LogFileCreate(CString file_name);
  //void SMPL_LogFile(int intensity, char leds);
  unsigned char CRC(char *pBuffer, int length);
  unsigned char CRC8(char *pBuf, unsigned short length);
  void OpenSetupDlg(void);
  void CheckProfileCRC(char *pBuffer);

  //---- Overrides
  protected:
  //DDX/DDV support
  virtual void DoDataExchange(CDataExchange* pDX);
  //Generated message map functions
  DECLARE_MESSAGE_MAP()

  int file_selected;
  int file_parsed;
  int file_length;
  int file_saved;
  int from_begin;
  int write_log;
  int m_PCR_number;
  int m_Reflow_enabled;
  int m_Reflow_pause;
  float PID_1sec_old;
  std::vector<Point2D> PidLog;
  std::vector<PointXY> ReflowLog[10];

  CString file_name;
  CString file_path; 

  CWinThread *pThread;

  public:
  afx_msg void OnButtonTest();
  afx_msg void OnButtonProfileWrite();
  afx_msg void OnButtonProfileRead();

  afx_msg void OnButtonFileOpen();
  afx_msg void OnButtonFileSave();
  afx_msg void OnButtonShowGraph();
  afx_msg void OnButtonReflowRun();
  afx_msg void OnButtonReflowReset();
  afx_msg void OnButtonShowFile();
  afx_msg void OnButtonAnalysis(); 

  afx_msg void OnPaint();
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
				 
  afx_msg void OnPage5_AddVertex();
  afx_msg void OnPage5_DeleteVertex();
  afx_msg void OnPage5_DeleteSegment();
  afx_msg void OnPage5_AddSegment();
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnPage5_SetpProfileDefault();
  //afx_msg void OnPage5_WriteProfileToDevice();
  //afx_msg void OnPage5_SaveProfileToFile();

  virtual BOOL OnInitDialog();
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

  afx_msg void OnCheckConsoleLogEnable();
  afx_msg void OnCheckPCRdataLog();
  afx_msg void OnButtonSettings();
  afx_msg void OnButtonReflowPause();
  afx_msg void OnSelchangeProfile();
};

#endif
