//==============================================================================
//File name:   "Page6.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page6.h"
#include "cmd.h"
#include "datatx.h"
#include "about_dlg.h"
#include "lpc21isp.h"
#include "files-cpp.h"

//---- Using multimedia timer
#include "Mmsystem.h"				//MM_Timer
#pragma comment(lib,"Winmm.lib")	//MM_Timer

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

CPage6 *pCPage6;

//------------------------------------------------------------------------------
//CPage6 property page
//------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CPage6, CPropertyPage)

//------------------------------------------------------------------------------
//CPage6 constructor
//------------------------------------------------------------------------------
CPage6::CPage6() : CPropertyPage(CPage6::IDD)
{
  pDevice = NULL;
  pConsole = NULL;
  pREG = NULL;
  pAPP = NULL;
  pCSETUP = NULL;
  pBT = NULL;
  pTab = NULL;
  pThread = NULL;
  pBLE = NULL;
  pAPP = NULL;
  program_run = 0;
  txt_info.Empty();
  pCPage6 = this;
}

//------------------------------------------------------------------------------
//CPage6 destructor
//------------------------------------------------------------------------------
CPage6::~CPage6()
{

}

//------------------------------------------------------------------------------
//CPage6 property page
//------------------------------------------------------------------------------
void CPage6::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //----
    DDX_Control(pDX, IDC_EDIT_INFO, m_edit_info);
    DDX_Control(pDX, IDC_FW_PROGRESS, m_progress);
    DDX_Control(pDX, IDC_BUTTON_SHOW_EEPROM, m_but_eep_show);
    DDX_Control(pDX, IDC_BUTTON_EEPROM_ERASE, m_but_eep_erase);
    DDX_Control(pDX, IDC_BUTTON_EEPROM_TEST, m_but_eep_test);
    DDX_Control(pDX, IDC_BUTTON_UPDATE, m_but_fw_update);
    DDX_Control(pDX, IDC_BUTTON_BT_INFO, m_but_bt_info);
    DDX_Control(pDX, IDC_BUTTON_FW_INFO, m_but_fw_info);
    DDX_Control(pDX, IDC_BUTTON_HELP, m_but_help);
    DDX_Control(pDX, IDC_BUTTON_PROFILE_LIST, m_but_profiles);
    DDX_Control(pDX, IDC_BUTTON_CURR_PROFILE, m_but_profile);
    DDX_Control(pDX, IDC_BUTTON_ERASE_CUSTOM_PR, m_but_ers_profile);
    DDX_Control(pDX, IDC_BUTTON_SENSORS_LIST, m_but_sensors);
    DDX_Control(pDX, IDC_BUTTON_SHOW_SETTINGS, m_but_settings);
    DDX_Control(pDX, IDC_BUTTON_SET_DEFAULT, m_but_resetsetup);
}

//------------------------------------------------------------------------------
//CPage6 message map
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPage6, CPropertyPage)

  //----
  ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CPage6::OnButtonAbout) 
  ON_BN_CLICKED(IDC_BUTTON_HELP, &CPage6::OnButtonHelp)
  ON_BN_CLICKED(IDC_BUTTON_FW_INFO, &CPage6::OnButtonFWInfo)
  ON_BN_CLICKED(IDC_BUTTON_PROFILE_LIST, &CPage6::OnButtonProfileList)
  ON_BN_CLICKED(IDC_BUTTON_SENSORS_LIST, &CPage6::OnButtonSensorsList)
  ON_BN_CLICKED(IDC_BUTTON_SHOW_EEPROM, &CPage6::OnButtonShowEeprom)
  ON_BN_CLICKED(IDC_BUTTON_SHOW_SETTINGS, &CPage6::OnShowSettings)
  ON_BN_CLICKED(IDC_BUTTON_EEPROM_TEST, &CPage6::OnButtonEepromTest)
  ON_BN_CLICKED(IDC_BUTTON_EEPROM_ERASE, &CPage6::OnButtonEepromErase)
  ON_BN_CLICKED(IDC_BUTTON_SET_DEFAULT, &CPage6::OnButtonSetupDefault)
  ON_BN_CLICKED(IDC_BUTTON_CURR_PROFILE, &CPage6::OnButtonCurrProfile)
  ON_BN_CLICKED(IDC_BUTTON_ERASE_CUSTOM_PR, &CPage6::OnButtonEraseCustomProfile)
  ON_BN_CLICKED(IDC_BUTTON_BT_INFO, &CPage6::OnButtonBtInfo)
  ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CPage6::OnButtonFWUpdate)

  ON_BN_CLICKED(IDC_BUTTON_GET_SERIAL, &CPage6::OnButtonGetSerial)
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
BOOL CPage6::OnInitDialog()
{
  CPropertyPage::OnInitDialog();
  //Add extra initialization here  
  //----
  Controls_Ini();

  //==========================================================
  //Set the tooltip (help hints)
  //==========================================================
  struct prj_tooltip tooltips[] = 
  { 
    {&m_but_eep_show,    "Show EEPROM dump"},
    {&m_but_eep_erase,   "EEPROM full erase"},	
    {&m_but_eep_test,    "EEPROM erase and write pattern"},
    {&m_but_fw_update,   "Oven controller update"},
    {&m_but_bt_info,     "BT controller info"},
    {&m_but_fw_info,     "Oven controller info"},
    {&m_but_help,        "Oven info"},
    {&m_but_profiles,    "Profiles list"},
    {&m_but_profile,     "Current profile points"},
    {&m_but_ers_profile, "Erase user custom profile"},
    {&m_but_sensors,     "Show Oven sensors list"},
    {&m_but_settings,    "Show settings"},
    {&m_but_resetsetup,  "Reset oven setup"}
  };

  ToolTip.SetToolTip(FromHandle(this->m_hWnd), tooltips, _countof(tooltips));
  
  //----    
  SetTXpointer(pBT);

  //return TRUE unless you set the focus to a control
  return TRUE;  
  //EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------------------
//Edit Box change handler
//------------------------------------------------------------------------------
BOOL CPage6::PreTranslateMessage(MSG* pMsg)
{  
  ToolTip.PreTranslateMessage(pMsg);
  return CPropertyPage::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage6::Test(void)
{
  //pBT->Tx(CMD_NRF_GET_POWER_VOLTAGE, NULL, 0); //+
  //pBT->Tx(CMD_NRF_LED_BLINK, NULL, 0); //+
  //pBT->Tx(CMD_NRF_SET_SETUP_DEFAULT, NULL, 0); //+
  //pBT->Tx(CMD_NRF_CHECK_DEVICE, NULL, 0); //+
  //I2C_Read((0xA0>>1), 0, 16);	//+
  ///char buf[4] = {1,2,3,4};
  ///I2C_Write((0xA0>>1), 0, buf, 4); //+
  ///I2C_Read((0xA0>>1), 0, 16);	//+
  //pBT->Tx(, NULL, 0); //
}

//------------------------------------------------------------------------------
//Function:	  Read EEPROM AT24C02
//------------------------------------------------------------------------------
void CPage6::I2C_Read(unsigned char dev_addr, unsigned short mem_addr, short len)
{
  char data[5];
  data[0] = dev_addr; //dev addr
  data[1] = 1;        //length for adress write
  data[2] = (unsigned char) mem_addr>>8;//length for data read 
  data[3] = (unsigned char) mem_addr;   //params here ... 
  data[4] = len-1;

  pBT->Tx(CMD_NRF_READ_I2C, data, 5); //+
}

//------------------------------------------------------------------------------
//Function:	 write EEPROM AT24C02
//------------------------------------------------------------------------------
void CPage6::I2C_Write(unsigned char dev_addr, 
	                   unsigned short mem_addr, char *pBuf, short len)
{
  char data[5+255];
  data[0] = dev_addr; //dev addr
  data[1] = (unsigned char) mem_addr;
  memcpy(&data[2], pBuf, len);

  pBT->Tx(CMD_NRF_WRITE_I2C, data, 2+len); //+
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------ 
void CPage6::OnButtonBtInfo(){Clear(); pBT->Tx(CMD_NRF_GET_BT_INFO, NULL, 0);}
void CPage6::OnButtonFWInfo(){Clear(); pBT->Tx(CMD_GET_ABOUT, NULL, 0);}
void CPage6::OnButtonHelp() {Clear(); pBT->Tx(CMD_GET_HELP, NULL, 0);}
void CPage6::OnButtonProfileList(){Clear(); pBT->Tx(CMD_GET_PROFILE_LIST, NULL, 0);}
void CPage6::OnButtonCurrProfile(){Clear(); pBT->Tx(CMD_SHOW_CURR_PROFILE, NULL, 0);}
void CPage6::OnButtonSensorsList(){Clear(); pBT->Tx(CMD_GET_SENSORS_LIST, NULL, 0);}
void CPage6::OnShowSettings(){Clear(); pBT->Tx(CMD_SHOW_SETTINGS, NULL, 0);}
void CPage6::OnButtonEepromTest(){Clear(); pBT->Tx(CMD_EEPROM_TEST, NULL, 0);}
void CPage6::OnButtonEepromErase(){Clear(); pBT->Tx(CMD_EEPROM_ERASE, NULL, 0);}
void CPage6::OnButtonSetupDefault(){Clear(); pBT->Tx(CMD_SET_SETUP_DEFAULT, NULL, 0);}
void CPage6::OnButtonEraseCustomProfile(){Clear(); pBT->Tx(CMD_ERASE_CUSTOM_PROFILE, NULL, 0);}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage6::OnButtonShowEeprom() 
{
  unsigned char data[4];
  data[0] = 0;
  data[1] = 0; //high byte
  data[2] = 0;
  data[3] = 1; //high byte 256 or 2048 

  Clear();
  CString txt;

  //--------------------------------------------
  ///pConsole->Write("%s", "\nADDR:\t");
  txt_info += ("ADDR:  ");
  
  //Print Address Header
  for(int i=0; i<16; i++)
  {
    if(i==8)
    {
      ///pConsole->Write("%s","| ");
      txt.Format("| ");
      txt_info += txt;
    }

    ///pConsole->Write("%02lX ", i);
    txt.Format("%02lX ", i);
    txt_info += txt;

  }
  ///pConsole->Write("%s", "\n"); 
  txt.Format("%s", "\r\n");
  txt_info += txt;

  
  for(int i=0; i<70; i++)
  { 
    ///pConsole->Write("%s", "-");
    txt.Format("%s", "-");
    txt_info += txt;
  }

  //pConsole->Write("%s", "\n");
  txt.Format("%s", "\r\n");
  txt_info += txt;
   
  m_edit_info.SetWindowTextA(txt_info);
  //---------------------------------------------

  pBT->Tx(CMD_GET_EEPROM, (char*)data, 4);
}

//------------------------------------------------------------------------------
// Function
//------------------------------------------------------------------------------
void CPage6::OnButtonAbout()
{
  ///Test();
 
  CAboutDlg dlg;
  INT_PTR nResponse = dlg.DoModal();
  if(nResponse == IDOK){}
  else if (nResponse == IDCANCEL){}
}


//------------------------------------------------------------------------------
// Function
//------------------------------------------------------------------------------
void CPage6::PrintInfo(std::string *pTxt)
{
  CString txt(pTxt->c_str());
  txt.Replace("\n", "\r\n");
  txt_info += txt;
  m_edit_info.SetWindowTextA(txt_info);
}
  
//------------------------------------------------------------------------------
// Function
//------------------------------------------------------------------------------
void CPage6::Clear(void)
{
  txt_info.Empty();
  m_edit_info.SetWindowTextA("");
}

//------------------------------------------------------------------------------
// Function
//------------------------------------------------------------------------------
void CPage6::OnButtonFWUpdate()
{
  if(program_run==0) 
  {

  //===========================================================
  //Переключить процессор печи в режим программирования
  //===========================================================
  CString txt;
  txt = "Press and Hold Oven button F1\r\n";
  //txt += "Then press button 'Oven Reset'\r\n";
  //txt += "Then press button 'FW Update'\r\n";
  //AfxMessageBox(_T(txt));

    //---------------------------------------
    // Check if the oven is connected ?
    //---------------------------------------
    if(pAPP->bt_connected==0)
    {
      CString str = "Error: Device disconnected...";
      m_edit_info.SetWindowText(_T(str));
      return;
    }

    //Do not change Tab if chip programming
    ///pTab->EnableWindow(FALSE);
	program_run = 1;
	m_but_fw_update.SetWindowTextA("Update Stop");
  	ThreadLaunch();
  }
  else
  {
	//pTab->EnableWindow(TRUE);
	m_but_fw_update.SetWindowTextA("FW Update");
	ev_ExitRequest.SetEvent();
	program_run = 0;
  }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPage6::OnButtonGetSerial()
{
  Clear();
  pBT->Tx(CMD_NRF_GET_SERIAL_NUMBER, NULL, 0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPage6::ParseBootMsg(char *pBuf, int length)
{
  std::string txt;

  for(int i=0; i<length; i++)
  {
    txt.push_back(*pBuf++);
  }
        
  //PrintInfo(&txt);
  bootloader_msg = txt;
  ///TRACE(txt.c_str()); 

  //----
  ev_BootDataRx.SetEvent();
}

