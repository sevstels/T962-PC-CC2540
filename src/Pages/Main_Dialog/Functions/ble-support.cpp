//==============================================================================
//File name:    "MainDlg.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "PIDPC.h"
#include "MainDlg.h"
#include "cmd.h"
#include "datatx.h"
#include <stdio.h>
 
#include "app-setup.h"

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

/*
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BT_AddrToTxt(CString *pTxt, long long Addr) 
{
  unsigned char buf[6];
  CString txt;

  for(int i = 0; i<6; i++) 
  {
	buf[5-i] = Addr;
	Addr >>= 8;

	if(i!=5)txt.Format("%X:", buf[5-i]);
	else txt.Format("%X", buf[5-i]);
	 
	*pTxt += txt;
  }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BT_AddrToStr(char *pAddr, long long Addr) 
{
  unsigned char buf[6];
  CString txt;

  for(int i = 0; i<6; i++) 
  {
	buf[5-i] = Addr;
	Addr >>= 8;

	if(i!=5)txt.Format("%X:", buf[5-i]);
	else txt.Format("%X", buf[5-i]);
	 
	// *pTxt += txt;
  }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CMainDlg::BLE_Open(void)
{
  int find_adapters = 1;//BLE.FindAdapters();
  if(find_adapters<=0)
  {	  

	//----
	CString txt;
	txt = "Bluetooth adapter not found\r\n\r\n";
	txt += "1. Connect the BCM20702A0 BT USB dongle to PC\r\n";
	txt += "2. Install the WinUSB driver -> button About\r\n";
	txt += "3. Restart the program\r\n";
	pPage6->m_edit_info.SetWindowTextA(txt);
	//----
	return -1;
  }

  BT.SethBLE(&BLE);
  //----

  return 1;
}
 
*/
  /*
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void BT_VecAddrToStr(char *pAddr, std::vector <char> *pAdrr) 
{
  unsigned char buf;
  CString txt;

  for(int i = 0; i<6; i++) 
  {
//	buf = pAdrr->at[i];

	//if(i!=5)txt.Format("%X:", buf[5-i]);
	//else txt.Format("%X", buf[5-i]);
	 
	//*pTxt += txt;
  }
} */

//------------------------------------------------------------------------------
//	BT ADDR: D1:A3:5D:1B:6E:11
//------------------------------------------------------------------------------
/*
void CMainDlg::BLE_SelchangeDevice(void)
{
 int index = m_combo_devices_list.GetCurSel();
 
 CString txt, dig;
 //m_combo_devices_list.GetWindowTextA(txt);

 if(BLE.dev_connected==1)
 {
   BLE.Disconnect();
   //no devices
   Controls_Enable(0);
   LED_Control(0);
   Sleep(200);
 }

 //----
 txt = BLE.Devices_BT_ADDR_str.at(index).c_str();
 /*
 std::vector <char> dev_addr;
 dev_addr = BLE.Devices_BT_ADDR.at(index);
 
 for(int i=0; i<6; i++) 
 {
	unsigned char byte = dev_addr.at(i);
	dig.Format("%0.2X", byte);
	txt += dig;
 }
*/
/*
 BLE.Connect((char*)txt.GetBuffer());
 ///BLE.Connect((char*)"5D1B6E110100");
 ///BLE.Connect((char*)"D1A35D1B6E11");


 /*
 if(pBLE->dev_connected==0)
 {
    ///int ret = pBLE->Connect((char*)"D1A35D1B6E11");
    //pBLE->Connect((char*)"C6049C279ABD");
	 
    //F6:41:33:9A:73:C0 Terry
	//pBLE->Connect((char*)"F641339A73C0");
	//pBLE->Connect((char*)pBLE->DevicesI	
  }
}  */

/*
m_list_devices.ResetContent();
  for(int i=0; i<length; i++)
  {
	CString txt, name, addr_txt;
	unsigned long long addr_dig = pBLE->Devices_BT_ADDR.at(i);
	name = pBLE->DevicesName.at(i).c_str();	 
    BT_AddrToTxt(&addr_txt, addr_dig); 

	txt.Format("%s  %s", name, addr_txt);
	m_list_devices.AddString(txt);
  }*/

//-----------------------------------------------------------------------------
//Function:	BLE Rx Handler
//-----------------------------------------------------------------------------
extern CMainDlg *pCMainDlg;
void CMainDlg::BLE_DataRx(char *pBuf, int length)
{ 
  pCMainDlg->DataRx(pBuf, length);
}

//-----------------------------------------------------------------------------
//Function:	BLE Events Handler
//-----------------------------------------------------------------------------  
void CMainDlg::BLE_Event(int event_id, char *pBuf, int length)
{
  //----
  if(event_id==BT_DEV_ARRIVED || event_id==BT_DEV_EXIT)
  {
    //pCMainDlg->BLE_UpdateDeviceList();
	return;
  }

  //----
  if(event_id==BT_DEV_CONNECTED)
  {
  	//Device was connected	
	pCMainDlg->LED_Control(1);
	pCMainDlg->APP.bt_connected = 1;

	//save last address
	CString str = pCMainDlg->BLE.dev_bd_addr_str.c_str();
	pCMainDlg->REG.SetTxtVar("last_bt_addr", str);
	return;
  }
 
  //----
  if(event_id==BT_DEV_DISCONNECTED)
  {
  	//Device disconnected
	pCMainDlg->LED_Control(0);
	pCMainDlg->APP.bt_connected = 0;
  }

  //----
  if(event_id==BT_DEV_INFO)
  {
	CString txt(pBuf);
	
	if(pCMainDlg->pBTDlg!=NULL)
	{
	  pCMainDlg->pBTDlg->DeviceInfo();
	}
  }
}
