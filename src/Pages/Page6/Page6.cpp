//==============================================================================
//File name:   "Page6.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
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

CPage6 *pPG6;

//------------------------------------------------------------------------------
//CPage6 property page
//------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CPage6, CPropertyPage)

//------------------------------------------------------------------------------
//CPage6 constructor
//------------------------------------------------------------------------------
CPage6::CPage6() : CPropertyPage(CPage6::IDD)
{
  pBLE = NULL;
  pAPP = NULL;
  txt_info.Empty();
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

  /*
  int check;
  pWReg->GetIntVar("excel-showfile", check);
  m_check_fileshow.SetCheck(check);

  pWReg->GetIntVar("excel-groupcomp", check);
  m_check_group.SetCheck(check);
  
  pWReg->GetIntVar("excel-onlysmt", check);
  m_check_onlysmt.SetCheck(check);

  //==========================================================
  //Set the tooltip (help hints)
  //==========================================================
  struct prj_tooltip tooltips[] = 
  { 
    {&m_check_group,       "Add same components to 1 string"},
    {&m_check_fileshow,    "Show xls file"},	
    {&m_List,              "Set xls table parameters"},
    {&m_but_CreateBOM,     "Create *.xls BOM file"},
    {&m_check_onlysmt,     "Add to list only R,C components"},  /*
    {&m_but_hole_close,    "Close Hole"} */	 /*
  };

  ToolTip.SetToolTip(FromHandle(this->m_hWnd), tooltips, _countof(tooltips));
         */

  SetTXpointer(pBT);
  pPG6 = this;

  //return TRUE unless you set the focus to a control
  return TRUE;  
  //EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------------------
//Edit Box change handler
//------------------------------------------------------------------------------
BOOL CPage6::PreTranslateMessage(MSG* pMsg)
{	 
  int result;
  CString txt;
  unsigned long value;
  
  //Check message from press ENTER 
  if((WM_KEYDOWN == pMsg->message)&&(VK_RETURN == pMsg->wParam)) 
   { /*
	//---- 
    if(pMsg->hwnd==m_sample_number.m_hWnd)
	{  	
		result = ParseValue(&m_sample_number, value);
		if(!result) return TRUE;
        //----	   
        pREG->SetIntVar("page8_number", value);
	   //запрет дальнейшей обработки
	   return TRUE;
	}

	//---- 
    if(pMsg->hwnd==m_sample_period.m_hWnd)
	{  	
		result = ParseValue(&m_sample_period, value);
		if(!result) return TRUE;
        //----	   
        pREG->SetIntVar("page8_period", value);
	   //запрет дальнейшей обработки
	   return TRUE;
	}

	//---- 
    if(pMsg->hwnd==m_sample_delay.m_hWnd)
	{  	
		result = ParseValue(&m_sample_delay, value);
		if(!result) return TRUE;
        //----	   
        pREG->SetIntVar("page8_delay", value);
	   //запрет дальнейшей обработки
	   return TRUE;
	}

	//---- 
    if(pMsg->hwnd==m_led_on_time.m_hWnd)
	{  	 
		result = ParseValue(&m_led_on_time, value);
		if(!result) return TRUE;
        //----	   
        pREG->SetIntVar("page8_on_time", value);
	   //запрет дальнейшей обработки
	   return TRUE;
	}

     
    //Set Max current
    if(pMsg->hwnd==m_edit_ledcan_id.m_hWnd)
    {	
		unsigned short id; 
		result = ParseValueHex(&m_edit_ledcan_id, id);
		if(result>0)
		{
	      pDevice->LED_ControllerID = (unsigned char)id;
		  ///CAN_SendCommand(pCAN, CAN_device_ID, 0, 
		  CMD_LED_SET_MODE, LED_SET_CAN_ID, pDevice->LED_ControllerID);
		}	
        return TRUE;	
	}*/	    
  }


  //ToolTip.PreTranslateMessage(pMsg);
  return CPropertyPage::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------ 
void CPage6::OnButtonBtInfo(){/*pConsole->*/Clear(); pBT->Tx(CMD_NRF_GET_BT_INFO, NULL, 0);}

//void CPage6::OnButtonBtInfo(){/*pConsole->*/Clear(); pBT->Tx(CMD_NRF_GET_BT_INFO, NULL, 0);}
void CPage6::OnButtonFWInfo(){/*pConsole->*/Clear(); pBT->Tx(CMD_GET_ABOUT, NULL, 0);}
void CPage6::OnButtonHelp() {/*pConsole->*/Clear(); pBT->Tx(CMD_GET_HELP, NULL, 0);}
void CPage6::OnButtonProfileList(){/*pConsole->*/Clear(); pBT->Tx(CMD_GET_PROFILE_LIST, NULL, 0);}
void CPage6::OnButtonCurrProfile(){/*pConsole->*/Clear(); pBT->Tx(CMD_SHOW_CURR_PROFILE, NULL, 0);}
void CPage6::OnButtonSensorsList(){/*pConsole->*/Clear(); pBT->Tx(CMD_GET_SENSORS_LIST, NULL, 0);}
void CPage6::OnShowSettings(){/*pConsole->*/Clear(); pBT->Tx(CMD_SHOW_SETTINGS, NULL, 0);}
void CPage6::OnButtonEepromTest(){/*pConsole->*/Clear(); pBT->Tx(CMD_EEPROM_TEST, NULL, 0);}
void CPage6::OnButtonEepromErase(){/*pConsole->*/Clear(); pBT->Tx(CMD_EEPROM_ERASE, NULL, 0);}
void CPage6::OnButtonSetupDefault(){/*pConsole->*/Clear(); pBT->Tx(CMD_SET_SETUP_DEFAULT, NULL, 0);}
void CPage6::OnButtonEraseCustomProfile(){/*pConsole->*/Clear(); pBT->Tx(CMD_ERASE_CUSTOM_PROFILE, NULL, 0);}

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

  //pConsole->Clear(); 
  Clear();
  CString txt;

  //--------------------------------------------
  ///pConsole->Write("%s", "\nADDR:\t");
  txt_info += ("\r\nADDR:\t");
  
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
  CAboutDlg dlg;
  INT_PTR nResponse = dlg.DoModal();
  if(nResponse == IDOK)
  {
    //TODO: Place code here to handle when the dialog is
    //dismissed with OK
  }
  else if (nResponse == IDCANCEL)
  {
    //TODO: Place code here to handle when the dialog is
    //dismissed with Cancel
  }
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
  CString str, file_path, file_name, txt;

  //---------------------------------------
  // проверка подключена ли печка
  //---------------------------------------
  if(pAPP->bt_connected==0)
  {
  	str = "Error: Device disconnected...";
  	m_edit_info.SetWindowText(_T(str));
    return;
  }


  //Standart file dialog
  CFileDialog fileDialog(NULL, NULL, _T("*.bin"), 
	                     NULL, _T("firmware file (*.bin)"));
  //Call dlg window
  INT_PTR dlg = fileDialog.DoModal();

  if(dlg==IDOK)
  {
	 file_path = fileDialog.GetPathName();
     file_name = fileDialog.GetFileName();
  }

  //---------------------------------------
  //
  //---------------------------------------
  //file_name = "t962fw.bin";
   
  int exist = CheckFile(file_path);
  if(exist!=1)
  {
  	str = "Error: No Firmware file";
  	m_edit_info.SetWindowText(_T(str));
    return;
  }

  unsigned long length = Get_FileSize(file_name);
  if(length<200)
  {
  	str = "Error: Firmware file wrong";
  	m_edit_info.SetWindowText(_T(str));
    return;
  }

  char *pBuf = (char*) new char[length];
  if(pBuf==NULL) return;

  unsigned long read_length = Read_File(file_name, pBuf, 0, length);
  if(read_length != length) return;

  //---------------------------------------
  //Programming
  //---------------------------------------
  ISP_ENVIRONMENT isp;
  //isp.
  NxpDownload(&isp);

  delete [] pBuf;
  pBuf = NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CPage6::ParseBootMsg(char *pBuf, int length)
{
  std::string txt;
  char buf[128];
  memset(buf, 0, sizeof(buf));
  memcpy(buf, pBuf, sizeof(buf));

  for(int i=0; i<length; i++)
  {
	txt.push_back(*pBuf++);
  }
		
  //PrintInfo(&txt);
  boot_txt = txt;
  TRACE(txt.c_str()); 

  //----
  ev_BootDataRx.SetEvent();
}
