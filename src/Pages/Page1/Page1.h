//==============================================================================
//File name:   "Page1.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#include "afxcmn.h"
#include "afxwin.h"
#include "tec-resource.h"
#include "device_struct.h"

#include "AppWinReg.h"
#include "tooltip.h"
#include "datatx.h"

#ifndef _PAGE_1_H_
#define _PAGE_1_H_

#pragma once

//------------------------------------------------------------------------------
//CPage1 dialog
//------------------------------------------------------------------------------
class CPage1 : public CPropertyPage
{
  DECLARE_DYNCREATE(CPage1)

  //---- Construction
  public:
  CPage1();
 ~CPage1();

  //---- Dialog Data
  enum {IDD = IDD_Page1};

  protected:
  //DDX/DDV support
  virtual void DoDataExchange(CDataExchange* pDX);    
  
  DECLARE_MESSAGE_MAP()
    
  public:
  CStatic m_txt;
  CEdit m_offset[2];
  CEdit m_dgain[2]; 
  CStatic m_temperature[2];

  //----
  CDataTX *pBT;
  CToolTip ToolTip;
  Device_Setup2 *pDevice;
  Device_Params *pParams;
  CAppWinReg *pREG;
  UINT MM_TimerID;

  //----
  void Controls_Ini(void);
  void Controls_Enable(BOOL on_off);
  void Controls_Update(void);
  void HeaterTemperature(char *pBuf); 
  void Monitoring(int on);

  //----
  virtual BOOL OnInitDialog();
  virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif