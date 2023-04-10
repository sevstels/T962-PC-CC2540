//==============================================================================
//File name:    "dlg_sensor.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "bt-dlg.h"
#include "parse-value.h"
#include "MainDlg.h"

//---- Mem Leakage Debug
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//----

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---- Macro
IMPLEMENT_DYNAMIC(CBTDlg, CDialog)

//------------------------------------------------------------------------------
//Constructor
//------------------------------------------------------------------------------
CBTDlg::CBTDlg()
{ 

}

//------------------------------------------------------------------------------ 
//Destructor
//------------------------------------------------------------------------------
CBTDlg::~CBTDlg()
{
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CBTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBTDlg)

	//}}AFX_DATA_MAP

	//	DDX_Control(pDX, IDC_CHECK_LED_ON, m_check_led_on);
	DDX_Control(pDX, IDC_EDIT_COM, m_edit_com);
	DDX_Control(pDX, IDC_EDIT_ADDR, m_edit_addr);
	DDX_Control(pDX, IDC_LIST1, m_Monitor);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_but_connect);
	DDX_Control(pDX, IDC_BUTTON_DISCONNECT, m_but_disconnect);
	DDX_Control(pDX, IDC_BUTTON_SCAN, m_but_scan);
	DDX_Control(pDX, IDC_COMBO_RX_GAIN, m_combo_rx_gain);
	DDX_Control(pDX, IDC_COMBO_TX_POWER, m_combo_tx_power);
}														  

//------------------------------------------------------------------------------ 
//mesages
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CBTDlg, CDialog)
  //{{AFX_MSG_MAP(CBTDlg)
  ON_WM_PAINT() 
  ON_WM_DESTROY()
  ON_WM_SYSCOMMAND()
  //ON_WM_CONTEXTMENU()
  ON_WM_QUERYDRAGICON()
  ON_WM_WINDOWPOSCHANGED()

  ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CBTDlg::OnButtonConnect)
  ON_BN_CLICKED(IDC_BUTTON_SCAN, &CBTDlg::OnButtonScan)
  ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CBTDlg::OnButtonDisconnect)
  ON_BN_CLICKED(IDC_BUTTON_COM, &CBTDlg::OnButtonCom)
  ON_CBN_SELCHANGE(IDC_COMBO_TX_POWER, &CBTDlg::OnSelchangeComboTxPower)
  ON_CBN_SELCHANGE(IDC_COMBO_RX_GAIN, &CBTDlg::OnSelchangeComboRxGain)
END_MESSAGE_MAP()

//------------------------------------------------------------------------------ 
//Dialog initialization
//------------------------------------------------------------------------------
BOOL CBTDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  POS.SetWindowPositon(this->m_hWnd);
  
  Controls_Ini();

  CString txt;
  txt.Format("%d", pBLE->COM.port_number);
  m_edit_com.SetWindowText(txt);

  if(pBLE->dev_connected==1)
  {
	m_Monitor.AddString(pBLE->dev_name_str.c_str());
	m_edit_com.EnableWindow(FALSE);
    m_edit_addr.EnableWindow(FALSE);

    //Buttons
    m_but_connect.EnableWindow(FALSE);
    m_but_disconnect.EnableWindow(TRUE);
    m_but_scan.EnableWindow(FALSE);
  }
			 /*
  //----
  m_edit_addr.SetWindowText(pBLE->dev_bd_addr_str.c_str());
  			   */

  DeviceInfo();

  Controls_Enable(TRUE);

  return TRUE;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CBTDlg::Controls_Ini(void)
{
  int pos;
  m_combo_rx_gain.ResetContent();
  m_combo_rx_gain.InsertString(0, "Normal");//
  m_combo_rx_gain.InsertString(1, "High");//
  pREG->GetIntVar("rx_gain", pos);
  m_combo_rx_gain.SetCurSel(pos);
  
  m_combo_tx_power.ResetContent();
  m_combo_tx_power.InsertString(0, " 4  dBm");//3
  m_combo_tx_power.InsertString(1, " 0  dBm");//2
  m_combo_tx_power.InsertString(2, "-6  dBm");//4
  m_combo_tx_power.InsertString(3, "-23 dBm");//0
  pREG->GetIntVar("tx_power", pos);
  m_combo_tx_power.SetCurSel(pos);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CBTDlg::Controls_Enable(BOOL on_off)
{
  //Edit controls
  m_edit_com.EnableWindow(on_off);
  //m_edit_addr.EnableWindow(on_off);

  //Buttons
  m_but_connect.EnableWindow(on_off);
  m_but_disconnect.EnableWindow(on_off);
  m_but_scan.EnableWindow(on_off);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CBTDlg::Controls_Update(void)
{  
  CString txt;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CBTDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
  CDialog::OnWindowPosChanged(lpwndpos);
  POS.WindowPosChanged(this->m_hWnd, lpwndpos);
}

//------------------------------------------------------------------------------ 
//Press OK
//------------------------------------------------------------------------------
BOOL CBTDlg::PreTranslateMessage(MSG* pMsg)
{
  //Check message from press ENTER 
  if((WM_KEYDOWN == pMsg->message)&&(VK_RETURN == pMsg->wParam)) 
  {
   //----    
   if(pMsg->hwnd==m_edit_com.m_hWnd)
   {     
	 //----
	 OnButtonCom();
	  	  
	 //Send cmd to main dlg
	 //::PostMessageA(hWndParent, UWM_DIALOG_MESSAGES, CMD_SET_COM, new_com);
     return TRUE;
   }
  }
  
  ToolTip.PreTranslateMessage(pMsg);      
  return CDialog::PreTranslateMessage(pMsg);
}  

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CBTDlg::OnButtonCom()
{
  m_Monitor.ResetContent();  
  char com;

  //----
  int result = ParseValueCh(&m_edit_com, com, 1, 100);
  if(!result) return; 

  if(pBLE->COM.port_number==com && pBLE->COM.opened==1)
  {
	m_Monitor.AddString("Port already open");
	return;
  }
  
  m_Monitor.ResetContent();
  m_Monitor.AddString("Detach BT Adapter");
  pBLE->Close(); 
  
  Sleep(500);
  
  m_Monitor.ResetContent();
  CString txt = "Wait Open BT Addapter..";
  m_Monitor.AddString(txt);
  m_Monitor.UpdateWindow();

  int ret = pBLE->Open((char)com);

  if(ret==1)
  { 
	 pREG->SetIntVar("last_com", com); 
     m_Monitor.ResetContent(); 
     m_Monitor.AddString("BT Addapter OK");
	 m_edit_addr.SetWindowText(pBLE->dev_bd_addr_str.c_str());
  }
  else 
  {
     m_Monitor.ResetContent(); 
     m_Monitor.AddString("Opening Error");
  }
}

//------------------------------------------------------------------------------ 
//Press Exit
//------------------------------------------------------------------------------
void CBTDlg::OnBnClickedOk()
{
  OnCancel();
  //TODO: Add your control notification handler code here
  //CDialog::OnOK();
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CBTDlg::OnOK()
{
  //CDialog::OnOK();
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CBTDlg::OnCancel()
{
  CloseDlg(); 
}                 

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CBTDlg::CloseDlg(void)
{
  ::PostMessageA(hWndParent, UWM_DIALOG_MESSAGES, DLG_CLOSE, 0);
  CDialog::OnCancel();
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CBTDlg::OnButtonScan()
{
  if(pBLE->dev_connected==1) 
  {
    m_Monitor.ResetContent(); 
    m_Monitor.AddString("Device already connected");
	return;
  }  
   	
  m_Monitor.ResetContent();   
  m_Monitor.AddString("Wait, Scan devices...");
  m_Monitor.UpdateWindow();
  m_edit_addr.SetWindowTextA("");
  m_edit_addr.UpdateWindow();

  pBLE->CMD_Scan();
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CBTDlg::DeviceInfo(void)
{
  if(pBLE->dev_detected==0 && pBLE->dev_connected == 0) 
  {
    m_Monitor.ResetContent(); 
    m_Monitor.AddString("Device not found");
	return;
  }  
   	
  m_Monitor.ResetContent(); 

  CString txt, dig;
  txt = pBLE->dev_name_str.c_str();
  txt += ", RSSI: ";
  dig.Format("%d dBm", pBLE->dev_rssi);
  txt += dig;

  m_Monitor.AddString(txt);
  m_Monitor.UpdateWindow();
  
  std::string addr_txt;
  pBLE->BD_AddrToString((char*)pBLE->dev_address, &addr_txt);

  m_edit_addr.SetWindowTextA(addr_txt.c_str());
  
  pBLE->CMD_ScanCancel();
}
  
extern CMainDlg *pCMainDlg;

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CBTDlg::OnButtonConnect()
{
  m_Monitor.ResetContent(); 

  if(pBLE->dev_connected==1) 
  {
    m_Monitor.ResetContent(); 
    m_Monitor.AddString("Device connected");
	return;
  }

  m_Monitor.ResetContent(); 
  m_Monitor.AddString("Connect");

  CString txt;
  m_edit_addr.GetWindowTextA(txt);
  std::string adr(txt.GetBuffer());
  unsigned char bd_address[6];

 /* bd_address[0] = 0xBD;
  bd_address[1] = 0x9A;
  bd_address[2] = 0x27;
  bd_address[3] = 0x9C;
  bd_address[4] = 0x04;
  bd_address[5] = 0xC6;	*/

  int ret = pBLE->BD_StringToAddr(&adr, (char*)bd_address);
  if(ret!=1)
  {
  	m_Monitor.ResetContent(); 
    m_Monitor.AddString("Address Error!");
    return;  
  }

  ret = pBLE->CMD_Connect((char*)bd_address);
  if(ret!=1)
  {
  	m_Monitor.ResetContent(); 
    m_Monitor.AddString("Can`t Connect");

	pBLE->CMD_ConnectCancel();
    return;
  }
   
  m_edit_addr.EnableWindow(FALSE);

  Sleep(400);
  ret = pBLE->CMD_NotifyEnable();
  if(ret==1) pCMainDlg->GetDeviceSetup();

  DeviceInfo();

  //Send cmd to main dlg
  //::PostMessageA(hWndParent, UWM_DIALOG_MESSAGES, CMD_BT_CONNECT, 0);
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CBTDlg::OnButtonDisconnect()
{
  if(pBLE->dev_connected==0) 
  {
    m_Monitor.ResetContent(); 
    m_Monitor.AddString("Device disconnected");
	return;
  } 
    
  m_Monitor.ResetContent(); 
  m_Monitor.AddString("Disconnect");

  pBLE->CMD_Disconnect();
  
  m_edit_addr.EnableWindow(TRUE);

  //Buttons
  m_but_connect.EnableWindow(TRUE);
  m_but_disconnect.EnableWindow(TRUE);

  //Send cmd to main dlg
  //::PostMessageA(hWndParent, UWM_DIALOG_MESSAGES, CMD_BT_DISCONNECT, 0);
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CBTDlg::OnSelchangeComboTxPower()
{
  int index;
  char pwr_code = 0;
  index = m_combo_tx_power.GetCurSel();
	
  if(index==0) pwr_code = 3;
  if(index==1) pwr_code = 2;
  if(index==2) pwr_code = 4;
  if(index==3) pwr_code = 0;

  pBLE->CMD_SetTxPower(pwr_code);
  pREG->SetIntVar("tx_power", index);
}

//------------------------------------------------------------------------------ 
//
//------------------------------------------------------------------------------
void CBTDlg::OnSelchangeComboRxGain()
{
  int index;
  index = m_combo_rx_gain.GetCurSel();

  pBLE->CMD_SetRxGain((char)index);
  pREG->SetIntVar("rx_gain", index);
}
