//==============================================================================
//File name:   "MainDlg.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================

//---- Pages
#include "Page1.h"
#include "Page2.h"
#include "Page3.h"
#include "Page4.h"
#include "Page5.h"
#include "Page6.h"

//----
#include "cc2540.h"
#include "device_struct.h"
#include "dialog-pos.h"
 
#include "app-setup.h"
#include "protocol-ex.h"
#include "datatx.h"
#include "led-blink.h"
#include "bt-dlg.h"

//---- Using multimedia timer
#include "Mmsystem.h"				//MM_Timer
#include "afxwin.h"
#pragma comment(lib,"Winmm.lib")	//MM_Timer

#define ID_TIMER_GET_DATA	2
#define ID_TIMER_GET_DEVICE	3
#define ID_TIMER_MONITORING	4

#pragma once
//------------------------------------------------------------------------------
//CMainDlg dialog
//------------------------------------------------------------------------------
class CMainDlg : public CDialog
{  
  public:
  //Construction standard constructor
  CMainDlg(CWnd* pParent = NULL);
 ~CMainDlg();

  //Dialog Data
  enum {IDD = IDD_MAIN_DIALOG};
  
  //Remote device internal structure
  Device_Setup  Device;
  Device_Setup *pDevice;
  Device_Params Params;
    
  Device_Setup2 DEV;

  //---- Implementation
  CPage1 *pPage1;
  CPage2 *pPage2;
  CPage3 *pPage3;
  CPage4 *pPage4;
  CPage5 *pPage5;
  CPage6 *pPage6;
  CBTDlg *pBTDlg;

  CDataTX BT;
  CProtocolEx PROTOCOL;  
  CC2540 BLE;

  CLOGW	Console;
  CDPOS POS;
  CLEDBlink LED;
  CTabCtrl m_Tab;
  CToolTip ToolTip;
  CAppWinReg REG;
  CApplSetup CSETUP;
  ApplSettings APP;

  HICON   m_hIcon;
  HBITMAP hLED_red;
  HBITMAP hLED_grey;
  HBITMAP hLED_green;
  CStatic m_led, m_led_txt;

  //----
  CButton m_but_BT;
  CButton m_but_LogClear;
  CButton m_but_DevReset;
  CButton m_but_GetSetup;

  CWinThread *pThread;
  //----
  CEvent ev_Quit;	
  CEvent ev_WaitEnd;
  CEvent ev_ExitRequest;
  CEvent ev_DataAccepted;
  
  int hTimer; int hTimer1;
  int monitoring_ack;
  int exit_request;
  int tab_opened;
  int first_launch;
  
  //----
  int  BLE_ini(void);
  int  BLE_Open(void);
  int  BLE_Close(void);
  void BLE_UpdateDeviceList(void);
  void BLE_SelchangeDevice(int index);
  void BLE_SelchangeAdapters(int index);
  static void BLE_DataRx(char *pBuf, int length);
  static void BLE_Event(int event_id, char *pBuf, int length);
  
  //----
  void DataRx(char *pBuf, int length);
  void DeviceMsgHandler(char cmd, char *pBuf, int length);
  int  ReadDeviceSetup(char *pBuf, int length);
  void Print_MemBuffer(char *pBuf, int length);
  void ParseDeviceEvents(char *pBuf, int length);
  
  //----
  void DeviceList(int param);
  int  GetDeviceSetup(void);
  int  OpenCommunication(void);
  void Controls_Enable(BOOL on_off);
  void LED_Control(int online);
  void TimerControl(int period);
  void Parse_DeviceAnswer(unsigned char *pBuf);
  void Parse_Monitoring(unsigned char *pBuf);
  void Parse_Device(unsigned char *pBuf);
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
  //virtual void HtmlHelp(DWORD_PTR dwData, UINT nCmd = 0x000F);
  //void WinHelp( DWORD dwData, UINT nCmd );

  afx_msg BOOL OnDeviceChange(UINT EventType, DWORD_PTR dwData);
  afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
  afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnTcnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnButtonCanSetup();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnButtonGetSetup();
  afx_msg void OnButtonOvenReset();
  afx_msg void OnButtonConsoleCls();
  afx_msg LRESULT OnDlgMessges(WPARAM wParam, LPARAM lParam);
  afx_msg void OnButtonBT();

  //Implementation
  protected:
  //DDX/DDV support
  virtual void DoDataExchange(CDataExchange* pDX);
  virtual BOOL OnInitDialog();	  
  DECLARE_MESSAGE_MAP()
  
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg void OnPaint();
};
