//==============================================================================
//File name:    "MainDlg.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "T962.h"
#include "MainDlg.h"
#include "cmd.h"
#include "datatx.h"
#include <stdio.h>
#include <dbt.h>

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

CMainDlg *pCMainDlg = NULL;

//------------------------------------------------------------------------------
//Function:	CMainDlg dialog
//------------------------------------------------------------------------------
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/): CDialog(CMainDlg::IDD, pParent)
{	
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

  pBTDlg = NULL;

  //Allocate memory for objects
  pPage1 = new CPage1;	
  pPage2 = new CPage2;
  pPage3 = new CPage3;
  pPage4 = new CPage4;
  pPage5 = new CPage5;
  pPage6 = new CPage6;
  
  pCMainDlg = this;
  pDevice = &Device;
  memset(&Device, 0, sizeof(Device_Setup));
  memset(&Params, 0, sizeof(Device_Params));
  
  //----
  memset(&APP, 0, sizeof(ApplSettings));
  APP.struct_length = sizeof(ApplSettings);
  CSETUP.pSetup = (ApplSettings*)&APP;
  CSETUP.hREG = (HANDLE)&REG;
  
  //----
  pPage1->pBT = &BT;
  pPage2->pBT = &BT;
  pPage3->pBT = &BT;
  pPage4->pBT = &BT;
  pPage5->pBT = &BT;  
  pPage6->pBT = &BT;
  
  //ble
//  pPage1->pBLE = &BLE;
  pPage2->pBLE = &BLE;
//  pPage3->pBLE = &BLE;
  pPage4->pBLE = &BLE;
//  pPage5->pBLE = &BLE;
  pPage6->pBLE = &BLE;

  pPage4->pCSETUP = &CSETUP;
  pPage6->pCSETUP = &CSETUP;

  pPage1->pDevice = &DEV;
  pPage2->pDevice = &DEV;
  pPage3->pDevice = &DEV;
  pPage4->pDevice = &DEV;
  pPage5->pDevice = &DEV;
//  pPage6->pDevice = &DEV;

  //----
  pPage1->pParams = &Params;  
//  pPage2->pParams = &Params;
  pPage3->pParams = &Params;
//  pPage4->pParams = &Params;
//  pPage5->pParams = &Params;
//  pPage6->pParams = &Params;

  //----
  pPage1->pAPP = &APP; 
  pPage2->pAPP = &APP;
  pPage3->pAPP = &APP; 
  pPage4->pAPP = &APP;   
  pPage5->pAPP = &APP; 
  pPage6->pAPP = &APP;

  //----
  pPage1->pREG = &REG;
  pPage2->pREG = &REG;
  pPage3->pREG = &REG;
  pPage4->pREG = &REG;
  pPage5->pREG = &REG;
  pPage6->pREG = &REG;

  //----
//  pPage1->pConsole = &Console;
  pPage2->pConsole = &Console;
//  pPage3->pConsole = &Console;
  pPage4->pConsole = &Console;
//  pPage5->pConsole = &Console;
  pPage6->pConsole = &Console;
  pPage6->pTab = &m_Tab;
  
  //----
  pPage4->pPage2 = pPage2;
  pPage4->pPage3 = pPage3;

  //clear pointers
  pThread = NULL;
  first_launch = 1;
  monitoring_ack = 0;
  exit_request = 0;
  tab_opened = 0;
  hTimer = 0;
  Console.SetWindowSize(720, 400);
}

//------------------------------------------------------------------------------
//Function:	
//------------------------------------------------------------------------------
CMainDlg::~CMainDlg()
{
  if(hTimer!=0)KillTimer(hTimer);
  
  BLE.RemoveHandlers();
  BLE.CMD_Disconnect();
  BLE.Close();

  //Free objects memory   
  delete pPage6;
  pPage6 = NULL;
  delete pPage5;
  pPage5 = NULL;  
  delete pPage4;
  pPage4 = NULL;
  delete pPage3;
  pPage3 = NULL;
  delete pPage2;
  pPage2 = NULL;
  delete pPage1;
  pPage1 = NULL;
  
  if(pBTDlg!=NULL) delete [] pBTDlg; 
  pBTDlg = NULL;

  //----
  DeleteObject(hLED_red);
  DeleteObject(hLED_grey);
  DeleteObject(hLED_green);
  hLED_red = NULL;
  hLED_grey = NULL;
  hLED_green = NULL;
}

//------------------------------------------------------------------------------
//Function:	
//------------------------------------------------------------------------------
void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//----radio
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_STATIC_DEVICE_LED, m_led);
	DDX_Control(pDX, IDC_STATIC_DEVICE_TXT, m_led_txt);
	DDX_Control(pDX, IDC_BUTTON_READ_SETUP, m_but_GetSetup);
	DDX_Control(pDX, IDC_BUTTON_CONSOLE_CLS, m_but_LogClear);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_but_DevReset);
	DDX_Control(pDX, IDC_BUTTON_BT, m_but_BT);
}

//------------------------------------------------------------------------------
//Function:	message handlers
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_CLOSE()
  ON_WM_PAINT()
  ON_WM_TIMER()
  ON_WM_DEVICECHANGE()
  ON_WM_QUERYDRAGICON()
  ON_WM_WINDOWPOSCHANGED()
  ON_MESSAGE(UWM_DIALOG_MESSAGES, &CMainDlg::OnDlgMessges)
  ///ON_COMMAND(ID_HELP, &CMainDlg::HtmlHelp)
  //----
  ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab)
  ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnTcnSelchangingTab)
  //----
  ON_BN_CLICKED(IDC_BUTTON_READ_SETUP, &CMainDlg::OnButtonGetSetup)
  //----
  ON_BN_CLICKED(IDC_BUTTON_RESET, &CMainDlg::OnButtonOvenReset)
  ON_BN_CLICKED(IDC_BUTTON_CONSOLE_CLS, &CMainDlg::OnButtonConsoleCls)
  //----
  ON_BN_CLICKED(IDC_BUTTON_BT, &CMainDlg::OnButtonBT)
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
BOOL CMainDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  //Set the icon for this dialog.  The framework does this automatically
  //when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);		// Set big icon
  SetIcon(m_hIcon, FALSE);		// Set small icon
  //TODO: Add extra initialization here

  //Preload LED images
  HINSTANCE hInstance;
  hInstance = AfxGetResourceHandle();
  hLED_red = LoadBitmapA(hInstance, (LPCSTR)IDB_LED_RED);
  hLED_grey = LoadBitmapA(hInstance, (LPCSTR)IDB_LED_GREY);
  hLED_green = LoadBitmapA(hInstance, (LPCSTR)IDB_LED_GREEN);
  
  CString str, address;
  if(REG.FirstLaunch()==1)
  {
    //First ini
	REG.SetIntVar("last_tab", 5);
	REG.SetIntVar("last_com", 1);
	REG.SetIntVar("tx_power", 1);
	REG.SetIntVar("rx_gain",  0);
	str = "00:00:00:00:00:00";
	REG.SetTxtVar("last_bt_addr", str);
  }

  //прочитать настройки приложения
  CSETUP.Read();
  LED.Ini(this->m_hWnd, &m_led, &m_led_txt, hLED_grey, hLED_green, hLED_red);

  //Create and init Tab Pages
  #include "pages.h"
  
  POS.SetWindowPositon(this->m_hWnd);

  //---- Set Last Opened Tab page
  int tab; 
  REG.GetIntVar("last_tab", tab);
  m_Tab.SetCurSel(tab);
  
  switch(tab)
  {
    case 0: pPage1->ShowWindow(SW_SHOW); break;
    case 1: pPage2->ShowWindow(SW_SHOW); break;  
  	case 2: pPage3->ShowWindow(SW_SHOW); break;
    case 3: pPage4->ShowWindow(SW_SHOW); break;
    case 4: pPage5->ShowWindow(SW_SHOW); break;
    case 5: pPage6->ShowWindow(SW_SHOW); break;
  }
  //current tab
  tab_opened = tab;

  LED_Control(0); 

  ///pPage2->Controls_Ini();

  //==========================================================
  //Set the tooltip (help hints)
  //==========================================================
  struct prj_tooltip tooltips[] = 
  {     
	{&m_but_GetSetup, "Get All Oven Settings"},
    {&m_but_BT, "Bluetooth Adapter control"},
	{&m_but_LogClear, "Clear Console log"},
	{&m_but_DevReset, "Urgent Oven Reset"}
  };

  ToolTip.SetToolTip(FromHandle(this->m_hWnd), tooltips, _countof(tooltips));
  
  //Bluetooth support
  int port, result;
  int tx_power, rx_gain;
  REG.GetIntVar("last_com", port);
  REG.GetTxtVar("last_bt_addr", address);
  REG.GetIntVar("tx_power", tx_power);
  REG.GetIntVar("rx_gain",  rx_gain);
  
  //----
  BT.SethBLE(&BLE);
  BLE.AddEventsHandler(BLE_Event);
  BLE.AddRxHandler(BLE_DataRx);
  BLE.AddFilter_DeviceName("T-962");
  result = BLE.SetAddress((char*)address.GetBuffer());
  if(result!=1){OnButtonBT();  return TRUE;}
  //----
  result = BLE.Open((char)port);
  if(result!=1){OnButtonBT();  return TRUE;}
  //----
  BLE.CMD_SetTxPower((char)tx_power);
  BLE.CMD_SetRxGain((char)rx_gain);
  //----
  result = BLE.Connect();
  if(result==1) GetDeviceSetup();

  //return TRUE  unless you set the focus to a control
  return TRUE;  
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
  if(nIDEvent==ID_TIMER_GET_DATA)
  {	 
    
  }	

  if(nIDEvent==ID_TIMER_GET_DEVICE)
  {
  	KillTimer(hTimer);
	hTimer = 0;
  }
  
  if(nIDEvent==ID_TIMER_MONITORING)
  {
  	KillTimer(hTimer1);
	hTimer1 = 0;
  }

  //do not delete this string!
  CDialog::OnTimer(nIDEvent);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CMainDlg::TimerControl(int period)
{
 if(hTimer==0)
  {	
	//---------------------------------------
    hTimer = SetTimer(ID_TIMER_GET_DATA, period, 0); 
   }
  else 
  {
    KillTimer(hTimer);
	hTimer = 0;
  }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CMainDlg::OpenCommunication(void)
{
  //Translate pointer to all pages
  Controls_Enable(FALSE);
  
  //Get devices list
  hTimer = SetTimer(ID_TIMER_GET_DEVICE, 1000, 0); 

  return 1;  
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CMainDlg::Controls_Enable(BOOL on_off)
{
  //List control`s
  pPage1->Controls_Enable(on_off);
  pPage2->Controls_Enable(on_off);
  pPage3->Controls_Enable(on_off);
  pPage4->Controls_Enable(on_off);
  pPage5->Controls_Enable(on_off);
  pPage6->Controls_Enable(on_off);

  m_but_GetSetup.EnableWindow(on_off);
  m_but_DevReset.EnableWindow(on_off);

///  if(on_off) hTimer1 = SetTimer(ID_TIMER_MONITORING, 500, 0); 
}

//------------------------------------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//------------------------------------------------------------------------------
void CMainDlg::OnPaint()
{
 if(IsIconic())
  {	 
    //device context for painting
    CPaintDC dc(this); 
    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

    //Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    //Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
 else{CDialog::OnPaint();}
}

//------------------------------------------------------------------------------
// The system calls this function to obtain the cursor to display while the 
// user drags the minimized window.
//------------------------------------------------------------------------------
HCURSOR CMainDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

//------------------------------------------------------------------------------
//Handler: page change (rus: обработчик смены закладок)
//------------------------------------------------------------------------------
//#pragma warning(disable : 4100)
void CMainDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{	
  int iTab = m_Tab.GetCurSel();

  TC_ITEM tci;
  tci.mask = TCIF_PARAM;
  m_Tab.GetItem(iTab, &tci);
  CWnd* pWnd = (CWnd *)tci.lParam;
  pWnd->ShowWindow(SW_SHOW); 
  
  //Save current Tab number  
  REG.SetIntVar("last_tab", iTab);

  //Do it, if this page is open 
  switch (iTab)
   {	
     //Open Tab Page1
     case 0:
     //Launch device monitorng
	 pPage1->Monitoring(TRUE);
     break;
        
     //Open Tab Page2
     case 1:
     //Launch device monitorng	
     break;

     //Open Tab Page3
     case 2:
     //Launch device monitorng
	 pPage3->Monitoring(TRUE);
     break;
     
	 //Open Tab Page4
     case 3:
     pPage4->LoadProfile();
     break;
                                                    
     default:
     break;
  };

  tab_opened = iTab;
  
  //Do not delete this string
 *pResult = 0;
}

//-----------------------------------------------------------------------------
//handler  page change  обработчик скрытия закладки
//-----------------------------------------------------------------------------
void CMainDlg::OnTcnSelchangingTab(NMHDR *pNMHDR, LRESULT *pResult)
{	
  int iTab = m_Tab.GetCurSel();

  TC_ITEM tci;
  tci.mask = TCIF_PARAM;
  m_Tab.GetItem(iTab, &tci);
  CWnd* pWnd = (CWnd *)tci.lParam;
  pWnd->ShowWindow(SW_HIDE); 
    
  //Do it, if this page is closed 
  switch (iTab)
   { 
     case 0:
	 //Stop device monitorng
	 pPage1->Monitoring(FALSE);
     break;
     
	 case 2:
	 //Stop device monitorng
	 pPage3->Monitoring(FALSE);
     break;

     default:
     break;
   };	
  
  tab_opened = iTab;   
 *pResult = 0;
}
//#pragma warning(push)

//-----------------------------------------------------------------------------
//Function: запрос к устройству предоставить настройки
//-----------------------------------------------------------------------------
int CMainDlg::GetDeviceSetup(void)
{
  return BT.Tx(CMD_GET_DEVICE_SETUP, NULL, 0);
}

//-----------------------------------------------------------------------------
//Parse Current Car Pin Status
//-----------------------------------------------------------------------------
void CMainDlg::LED_Control(int online)
{ 
 if(online==1)
 {
   m_led.SetBitmap(hLED_green);
   m_led_txt.SetWindowTextA(_T("Connected"));
   pPage4->device_online = 1;
 } 
 else 
 {
   m_led.SetBitmap(hLED_grey);
   m_led_txt.SetWindowTextA(_T("Disconnected"));
   pPage4->device_online = 0;
 }

  m_led.ShowWindow(SW_SHOW);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
  ToolTip.PreTranslateMessage(pMsg);
  return CDialog::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CMainDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
  CDialog::OnWindowPosChanged(lpwndpos);
  POS.WindowPosChanged(this->m_hWnd, lpwndpos);
}

//-----------------------------------------------------------------------------
//USB Hotplug callback
//-----------------------------------------------------------------------------
BOOL CMainDlg::OnDeviceChange(UINT EventType, DWORD_PTR dwData)
{
  if((EventType == DBT_DEVICEARRIVAL) || (EventType == DBT_DEVICEREMOVECOMPLETE)) {}
  else 	return TRUE;
  
  //here not complited code
  int result = BLE.CheckCOM();
  if(result==2) return TRUE;
  
  if(result==1)
  {  
	  result = BLE.Open(BLE.COM.port_number);
	  if(result==1){Sleep(200); result = BLE.Connect();}
      if(result==1)
      {	 
		 Controls_Enable(TRUE);
		 LED_Control(1);

		 pPage1->Controls_Update();
         pPage1->Controls_Enable(TRUE);
         //----
		 pPage2->Controls_Update();
         pPage2->Controls_Enable(TRUE);
         //----
		 pPage3->Controls_Update();
         pPage3->Controls_Enable(TRUE);
         //----
		 pPage4->Controls_Update();
         pPage4->Controls_Enable(TRUE);
         //----
         pPage5->Controls_Update();
         pPage5->Controls_Enable(TRUE);
         //----
         pPage5->Controls_Update();
         pPage5->Controls_Enable(TRUE);
         //----
         pPage6->Controls_Update();
         pPage6->Controls_Enable(TRUE);
       }		 
      return TRUE;	
  }

  BLE.adapter_opened = 0;
  BLE.dev_connected = 0;
  BLE.COM.Close();
  LED_Control(0);
  Controls_Enable(FALSE);
  CString txt;
  txt = "BT Adapter disconnected!\r\n";
  AfxMessageBox(_T(txt), MB_ICONSTOP);
  
  return TRUE;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CMainDlg::OnButtonGetSetup()
{
  //read data from device 	
  GetDeviceSetup();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CMainDlg::OnButtonOvenReset()
{
  pPage6->Clear();
  //Tx
  BT.Tx(CMD_NRF_OVEN_RESET, NULL, 0);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CMainDlg::OnButtonConsoleCls()
{
  ///if(Console.opened==0)Console.Open("Device Log"); 
  if(Console.opened==1) Console.Clear();
  pPage6->Clear();
}
	
extern int boot_mode;

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
 void CMainDlg::DataRx(char *pBuf, int length)
{ /*
  debug_byte_count += length;
  if(transport==TRANSPORT_COM) TRACE2("Read COM %d Cnt %d\n", length, debug_byte_count);
  else TRACE2("Read BLE %d Cnt %d\n", length, debug_byte_count);
  */
  
  ///TRACE("BLE %d\n", length);

  //----
  for(int i=0; i<length; i++)
  {
	//Search data block header 
    int ready = PROTOCOL.Decoder(pBuf[i]);
  	if(ready==1)
	{
	  
      ///TRACE("BLE %d\n", PROTOCOL.data_length);

	  //Data Block was accepted
	  DeviceMsgHandler(PROTOCOL.cmd, PROTOCOL.pDataBuf, PROTOCOL.data_length);
	  PROTOCOL.FreeMem();
	  //reset progress
	  ///m_progress.SetPos(0);
      ///m_progress.SetBkColor(RGB(200, 200, 200));
	}

	//if protocol data header was accepted 
    if(PROTOCOL.header_accepted==1)
	{
	  //ini progress
	  ///m_progress.SetRange(0, PROTOCOL.data_length); //from to
	  ///m_progress.SetBkColor(RGB(255,255,255));
	  ///m_progress.SetPos(0);
	  //clear flag
	  PROTOCOL.header_accepted = 0;
	}
	
	//progress stepping
	///if(PROTOCOL.data_counter!=0) 
	///m_progress.SetPos(PROTOCOL.data_counter); 
  }
}

//-----------------------------------------------------------------------------
//Function: читать структуру настроек
//-----------------------------------------------------------------------------
int CMainDlg::ReadDeviceSetup(char *pBuf, int length)
{
  int stuct_length = sizeof(Device_Setup2);
  if(stuct_length!=length)
  {
  	CString msg = "Device settings do not match\r\n"; 
	msg += "with current PC software version.\r\n";
	msg += "Check for device firmware updates.\r\n";
    msg += "Settings will be to Factory Restore.\r\n";
	msg += "Functionality can be partially damaged.";
	::AfxMessageBox(msg);
	return -1;
  }
	
  //Device_Setup buffer
  memcpy(&DEV, pBuf, stuct_length);
  return 1;
}

//------------------------------------------------------------------------------
//Parse additional parameters for command options 
//------------------------------------------------------------------------------
void CMainDlg::Print_MemBuffer(char *pBuf, int length)
{
  CString txt;
  std::string std_txt;
  long i, j;
  unsigned char data;
  
  //----
  unsigned long address = (unsigned char)*pBuf++;
  address <<= 8;
  address |= (unsigned char)*pBuf++;
  
  //----
  unsigned short page_size = (unsigned char)*pBuf++;
  page_size <<= 8;
  page_size |= (unsigned char)*pBuf++;
  
  if(page_size>length) page_size = 128;
  
  /*
  printf("\nADDR:\t");
  Console.Write("%s", "\nADDR:\t");
 
  //Print Address Header
  for(i=0; i<16; i++)
  {
	if(i==8) Console.Write("%s","| "); 
	Console.Write("%02lX ", i);
  }
  Console.Write("%s", "\n"); 
  
  for(i=0; i<57; i++) Console.Write("%s", "-");
  Console.Write("%s", "\n"); 
*/

  // pres vsechny 16-Bytove bloky
  for (i=0; i<page_size; i+=16) 
  {
    //Print Address
    ///Console.Write("%06lX: ", i+address);
	txt.Format("%06lX: ", i+address);
	std_txt = txt.GetBuffer();
	pPage6->PrintInfo(&std_txt);

    //pres vsechny byte v 16B bloku
    for(j=0; j<16; j++)               
    {
      //Print div by 8 Bytes
      if(j==8)
	  { 
	     ///Console.Write("%s", "| ");
	     txt.Format("%s", "| ");
	     std_txt = txt.GetBuffer();
	     pPage6->PrintInfo(&std_txt);
	  }
    
    if((j+i)<page_size)
    {                 
      //pokud nejsou vypsany vsechny Byte
      data = pBuf[j+i]; 
      ///Console.Write("%02X ", data);
	  txt.Format("%02X ", data);
	  std_txt = txt.GetBuffer();
	  pPage6->PrintInfo(&std_txt);

    }
    else
	{
	  //jinak misto nej vypis mezery							  
      ///Console.Write("%s", "   ");
 	  txt.Format("%s", "   ");
	  std_txt = txt.GetBuffer();
	  pPage6->PrintInfo(&std_txt);
	}
  }
   
  // konec radky
  ///Console.Write("%s", "\n");
  txt.Format("%s", "\n");
  std_txt = txt.GetBuffer();
  pPage6->PrintInfo(&std_txt);
 }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CMainDlg::ParseDeviceEvents(char *pBuf, int length)
{
  char buf[16];
  memcpy(buf, pBuf, sizeof(buf));

  unsigned char event_code = buf[0];

  switch(event_code)
  {
    //----
    case EVT_REFLOW_DONE:
    pPage4->Parser(pBuf);
	break;

    //----
    case EVT_CMD_ACK:
	LED.Blink(pPage4->device_online, 1, 160, NULL);
	break;

    //----
    case EVT_SAVE_EEPROM:
	LED.Blink(pPage4->device_online,  3, 80, "Save..");
	break;

	//----
	case EVT_WRITE_PROFILE:
	pPage4->block_writed = 1;
	break;

	//----
	case EVT_BOOT_MODE:
	//boot_mode = 1;
	break;
  }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CMainDlg::DeviceMsgHandler(char cmd, char *pBuf, int length)
{
  unsigned char command = (unsigned char)cmd;
  std::string txt; CString data;
  int result;
  unsigned char cmd_temp;

  //----
  switch(command)
  {	
    //----
    case CMD_GET_DEVICE_SETUP:
         result = ReadDeviceSetup(pBuf, length);
		 if(result>0)
		 {
		   //Device connected		   
		   //Обновить все параметры
           pPage1->Controls_Update();
           pPage2->Controls_Update();
	       pPage3->Controls_Update();
	       pPage4->Controls_Update();
	       pPage5->Controls_Update();
		   pPage6->Controls_Update();
	       //----
		   LED_Control(1);
		   Controls_Enable(1);

		   //Check, if Tab opened, enable monitoring
		   if(first_launch==1)
		   {
		   	 first_launch = 0;
		     //Do it, if this page is open 
             int tab = m_Tab.GetCurSel(); 
             //Opened Tab Page1, Launch device monitorng
	         if(tab==0) pPage1->Monitoring(TRUE);
             //Opened Tab Page3, Launch device monitorng
	         if(tab==2) pPage3->Monitoring(TRUE);	
			 //Opened Tab Page4, Load T-ptofile
	         if(tab==3) pPage4->LoadProfile();
		   }
		 }
     break;

	//----
	case CMD_DEV_SEND_TXT:
		 for(int i=0; i<length; i++)
		 {
		    txt.push_back(*pBuf++);
		 }
		
	     //TRACE1("%s", t);  
		 //Console.Write("%s", txt.c_str());
		 pPage6->PrintInfo(&txt);
         break;
  	
    //----
	case CMD_BOOT_SEND_TXT:
		 pPage6->ParseBootMsg(pBuf, length);
         break;
		 
    //----
	case CMD_DEV_SEND_EVENT:
		 ParseDeviceEvents(pBuf, length);
         break;

	//----
	case CMD_DEV_SEND_REFLOW_LOG:
		 pPage4->ParseReflowLog(pBuf, length);
		 pPage4->LogFile_Reflow(pBuf);
         break;

	//----
	case CMD_CHECK_PROFILE_CRC:
		 pPage4->CheckProfileCRC(pBuf);
         break;

	//----
	case CMD_DEV_SEND_REFLOW_STEP:
		 pPage4->ParseReflowStep(pBuf, length);
         break;

	//----
    case CMD_GET_EEPROM:
		 Print_MemBuffer(pBuf, length);
    break;			
			
	//----
    case CMD_GET_TEMPERATURE:
		 cmd_temp = pBuf[0];
		 //Page1
		 if(cmd_temp==TC_HEATER_ALL_SENSORS)
		    pPage1->MonitoringParser(&pBuf[1]);
		 //Page4
		 if(cmd_temp==TC_TMPR_HEATER_FAN)
		    pPage3->MonitoringParser(&pBuf[1]);
    break;
 
    //----
    case CMD_GET_PROFILE:
		 pPage4->ParseProfile(pBuf, length); 
    break;

    //----
    case CMD_NRF_GET_POWER_VOLTAGE:
		 float voltage;
		 memcpy(&voltage, pBuf, 4);
		 data.Format("%1.2f v\r\n", voltage);
		 txt = data.GetBuffer();
		 pPage6->PrintInfo(&txt);
    break;
    
	//----
   case CMD_NRF_CHECK_DEVICE:
	    if(((unsigned char)pBuf[0])==0xA5)
		{
		  txt = "nRF OK\r\n";
		  pPage6->PrintInfo(&txt);
		}
    break;
	
	//----
    case CMD_NRF_GET_SERIAL_NUMBER:
	     txt = "Serial: ";
	     for(int i=0; i<16; i++)
	     {
	       data.Format("%02X",((unsigned char)pBuf[i]));
		   txt += data;
	     }
		 txt += "\r\n";
	     pPage6->PrintInfo(&txt);
    break;

	//----
    case CMD_NRF_READ_I2C:
	     txt = "I2C: ";
	     for(int i=0; i<length; i++)
	     {
	       data.Format("%02X ",((unsigned char)pBuf[i]));
		   txt += data;
	     }
		 txt += "\r\n";
	     pPage6->PrintInfo(&txt);
    break;

	 
  }
}

//-----------------------------------------------------------------------------
//Function:
//-----------------------------------------------------------------------------
void CMainDlg::OnButtonBT()
{
  //----
  if(pBTDlg==NULL) 
  { 
    unsigned char cmd = CMD_GET_DEVICE_SETUP;
	short length = sizeof(Device_Setup);
    
	//Open Dlg
    pBTDlg = new CBTDlg();
    pBTDlg->pREG = &REG;
	pBTDlg->pBLE = &BLE;
	pBTDlg->hWndParent = this->m_hWnd;
	pBTDlg->Create(IDD_DIALOG_BLE, this->GetWindow(IDD_DIALOG_BLE));
    pBTDlg->ShowWindow(SW_SHOW);
  }
}

//-----------------------------------------------------------------------------
//Function:
//-----------------------------------------------------------------------------
LRESULT CMainDlg::OnDlgMessges(WPARAM wParam, LPARAM lParam)
{
  unsigned char com = (unsigned char)lParam;
  int ret;

  //----
  switch(wParam)
  {
	  //----
      case DLG_CLOSE:
	  delete [] pBTDlg;
	  pBTDlg = NULL;
      return 1;
	
	  //----
      case CMD_BT_CONNECT:
/*	  BLE.CMD_Connect(NULL);
      Sleep(400);
	  BLE.CMD_NotifyEnable();	*/
      break;	
     
      //----
      case CMD_BT_DISCONNECT:
//	  BLE.CMD_Disconnect();
      break;
     
      //---- 
      case CMD_BT_SCAN:
///	  BLE.CMD_Scan();
      break;
  }
		
  return 1;
}
