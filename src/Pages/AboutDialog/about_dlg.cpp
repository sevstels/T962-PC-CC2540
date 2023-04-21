//==============================================================================
//File name:    "about_dlg.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "afxdialogex.h"
#include "about_dlg.h"
#include "firmware_build.h"
#include "tec-resource.h"  //for IDD

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{

}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_STATIC_TXT_BUILD, m_txt);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
BOOL CAboutDlg::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  //TODO:  Add extra initialization here
  CString txt, data;  
  //-----
  txt  = "Build: ";
  txt += FIRMWARE_BUILD_DATA_STR;
  /*
  #ifdef _WIN64
  txt += ", x64, Win64, Bluetooth";
  #else
  txt += ", x86, Win32, No BT";
  #endif
  txt += "\r\n"; 
  */
  //txt += "Software tool for bioscanners\r\n";
  //txt += "Driver: silabs.com -> CP210x USB VCP Driver\r\n\r\n";
  //txt += "NanoBio Lab, Singapore";
  m_txt.SetWindowText(txt);
  
  //return TRUE unless you set the focus to a control
  //EXCEPTION: OCX Property Pages should return FALSE  
  return TRUE;
}

//------------------------------------------------------------------------------
// CMainDlg dialog
//------------------------------------------------------------------------------
  BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
  END_MESSAGE_MAP()
