//==============================================================================
//File name:   "Page5.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page4.h"
#include "MainDlg.h"
#include "files-cpp.h"
#include "anls-dlg-settings.h"
#include "cmd.h"
#include "datatx.h"

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

//Thread
DWORD PAGE5_THREAD(LPVOID lParam);

//------------------------------------------------------------------------------
//CPage4 property page
//------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CPage4, CPropertyPage)

//------------------------------------------------------------------------------
//CPage4 constructor
//------------------------------------------------------------------------------
CPage4::CPage4() : CPropertyPage(CPage4::IDD)
{
  file_selected = 0;
  file_parsed = 0;
  file_length = 0;
  file_saved = 0;
  file_name.Empty();
  write_log = 0;
  from_begin = 0;
  load_profile = 0;
  //----
  m_PCR_number = 1;
  m_Reflow_enabled = 0;
  m_Reflow_pause = 0;
  pProgress = &m_Progress_Write;
  pThread = NULL;
  pPage2 = NULL;
  PID_log = 0;
  EVNT_log = 0;
  pDlg = NULL;
}

//------------------------------------------------------------------------------
//CPage4 destructor
//------------------------------------------------------------------------------
CPage4::~CPage4()
{
  ThreadStop();
}

//------------------------------------------------------------------------------
//CPage4 property page
//------------------------------------------------------------------------------
void CPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_MONITOR, m_monitor);
	DDX_Control(pDX, IDC_BUTTON_SHOW_GRAPH, m_but_ShowGraph);
	DDX_Control(pDX, IDC_BUTTON_SHOW_FILE, m_but_FileShow);

	DDX_Control(pDX, IDC_BUTTON_READ_PROFILE, m_but_ProfileRead);
	DDX_Control(pDX, IDC_BUTTON_WRITE_PROFILE, m_but_ProfileWrite);
	DDX_Control(pDX, IDC_BUTTON_REFLOW_RUN, m_but_ReflowRun);
	DDX_Control(pDX, IDC_BUTTON_FOPEN_PCR, m_but_FileOpen);
	DDX_Control(pDX, IDC_BUTTON_FSAVE_PCR, m_but_FileSave);
	DDX_Control(pDX, IDC_BUTTON_FROM_BEGIN, m_but_FromBegin);

	DDX_Control(pDX, IDC_PROGRESS_WRITE, m_Progress_Write);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_Scroll);
	DDX_Control(pDX, IDC_COMBO_PCR_WRITE, m_combo_SelectProfile);
	DDX_Control(pDX, IDC_BUTTON_ANALYSIS, m_but_Analysis);
	DDX_Control(pDX, IDC_CHECK_SHOW_LOG, m_check_show_log_console);
	DDX_Control(pDX, IDC_CHECK_DATA_TXT_LOG, m_check_write_log_file);
	DDX_Control(pDX, IDC_BUTTON_SETTINGS, m_but_Settings);
	DDX_Control(pDX, IDC_BUTTON_PCR_PAUSE, m_but_ReflowPause);
}

//------------------------------------------------------------------------------
//CPage4 message map
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPage4, CPropertyPage)
  //----
  ON_WM_LBUTTONUP()
  ON_WM_LBUTTONDOWN()
  ON_WM_MOUSEMOVE()
  ON_WM_MOUSEWHEEL()

  ON_WM_PAINT()
  ON_WM_CONTEXTMENU()
  ON_WM_HSCROLL()

  //---- 
  ON_BN_CLICKED(IDC_BUTTON_FOPEN_PCR, &CPage4::OnButtonFileOpen)
  ON_BN_CLICKED(IDC_BUTTON_FSAVE_PCR, &CPage4::OnButtonFileSave)
  ON_BN_CLICKED(IDC_BUTTON_SHOW_GRAPH, &CPage4::OnButtonShowGraph)

  ON_BN_CLICKED(IDC_BUTTON_REFLOW_RUN, &CPage4::OnButtonReflowRun)
  ON_BN_CLICKED(IDC_BUTTON_FROM_BEGIN, &CPage4::OnButtonReflowReset)
  ON_BN_CLICKED(IDC_BUTTON_READ_PROFILE, &CPage4::OnButtonProfileRead)
  ON_BN_CLICKED(IDC_BUTTON_WRITE_PROFILE, &CPage4::OnButtonProfileWrite)

  ON_COMMAND(ID_PAGE5_ADDVERTEX, &CPage4::OnPage5_AddVertex)
  ON_COMMAND(ID_PAGE5_DELETEVERTEX, &CPage4::OnPage5_DeleteVertex)
  ON_COMMAND(ID_PAGE5_DELETESEGMENT, &CPage4::OnPage5_DeleteSegment)  
  ON_BN_CLICKED(IDC_BUTTON_SHOW_FILE, &CPage4::OnButtonShowFile)
  ON_BN_CLICKED(IDC_BUTTON_ANALYSIS, &CPage4::OnButtonAnalysis)
  //----

  ON_BN_CLICKED(IDC_CHECK_SHOW_LOG, &CPage4::OnCheckConsoleLogEnable)
  ON_BN_CLICKED(IDC_CHECK_DATA_TXT_LOG, &CPage4::OnCheckPCRdataLog)
  ON_BN_CLICKED(IDC_BUTTON_SETTINGS, &CPage4::OnButtonSettings)
  ON_BN_CLICKED(IDC_BUTTON_PCR_PAUSE, &CPage4::OnButtonReflowPause)
  ON_CBN_SELCHANGE(IDC_COMBO_PCR_WRITE, &CPage4::OnSelchangeProfile)

END_MESSAGE_MAP()

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
BOOL CPage4::OnInitDialog()
{
  CPropertyPage::OnInitDialog();
  ThreadLaunch();

  //Add extra initialization here
  GRPH.SetHWnd(this->m_hWnd);
 
  //----
  DSPL.pMonitor = &m_monitor;
  DSPL.DisplayIni(this->m_hWnd, 2,-2, 240, 390);
  DSPL.Draw_BackGround();
  //---- Scale Range:
  DSPL.ScaleOffset(30, 12);//px  
  DSPL.ScaleSize(40*8, 200);  //(340, 208);//px  
  
  // X = 0... 10 sec, 1 pix = 100 ms
  
  // X = 0... 10 sec, 1 pix = 1s
  //один шаг = 1 pix = 250 ms
  //10 секунд = 1 горизонтальная клетка шкалы = 40px
  //всего 8 клеток по горизонтали = 80 сек

  // Y = 100-40 = 60 Degr, 1 pix = 0.5 degr

  ///DSPL.ScaleRange(0, 100, SCALE_X_CELLS_NUM, 44, 98, 8);	
  DSPL.ScaleRange(0, 400, SCALE_X_CELLS_NUM, 25, 275, 10);	
    
  //----
  DSPL.GraphIni(30, 12, 370, 224);
  //DSPL.Scroll_Setup(&m_Scroll);	 
  //DSPL.Load_Profile(PCR_STANDARD); //Standard pcr
  //DSPL.ev_Redraw.SetEvent();
///  PCRF.pTMPR = pTMPR;

  //Controls 
  Controls_Ini();
  ControlsGrop(FALSE);
  
  //For log
  LOG_EEP.Ini("", "PCR_DeviceLog_", "csv");
  LOG_RFLW.Ini("", "ReflowLog_", "csv");

  /*  
  //----
  int check;
  pWReg->GetIntVar("excel-showfile", check);
  m_check_fileshow.SetCheck(check);

  pWReg->GetIntVar("excel-groupcomp", check);
  m_check_group.SetCheck(check);
  
  pWReg->GetIntVar("excel-onlysmt", check);
  m_check_onlysmt.SetCheck(check);
  */  

  //m_txt_PCRnumber.SetFocus();
  
  //==========================================================
  //Set the tooltip (help hints)
  //==========================================================
  struct prj_tooltip tooltips[] = 
  { 
    {&m_but_FileOpen,    "Load from file"},
    {&m_but_FileSave,    "Save to file"},
    {&m_but_ShowGraph,   "Show Temperature graph"},
    {&m_but_FileShow,    "Show *.csv file"},	
	{&m_but_ProfileWrite,"Write custom profile to device"},
    {&m_but_ProfileRead, "Read T-profile from device"},
    {&m_but_ReflowRun,   "Reflow Run/Stop"},
    {&m_but_FromBegin,   "Jump to begin point"},
    {&m_combo_SelectProfile,"Select profile for use"},
	{&m_but_Settings,    "Reflow Setup"},
	{&m_but_Analysis,    "Show result"}
  };

  ToolTip.SetToolTip(FromHandle(this->m_hWnd), tooltips, _countof(tooltips)); 

  //return TRUE unless you set the focus to a control
  return TRUE;  
  //EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------------------
//Functions:
//------------------------------------------------------------------------------
void CPage4::TabOpen(void)
{

}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnButtonTest()
{

}
/*
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnButtonFileOpen()
{
  CString str, file_path, txt;
  signed long digit;
  unsigned long txt_file_length;
  char Buf[10];
  char *pBuf, *pBuf1, *pBuf2;
  int errors=0;
  int err_string=0;
  int j, index, result;
  file_parsed = 0;
  file_selected = 0;
  
  //Standart file dialog
  CFileDialog fileDialog(TRUE, NULL, _T("*.xls"), 
	                     NULL, _T("PCR profile file (*.xls)"));
							    
  //Call window
  INT_PTR dlg = fileDialog.DoModal();

  if(dlg==IDOK)
   {
     file_path = fileDialog.GetPathName();
     file_name = fileDialog.GetFileName();
     
     /*
     str = file_path[0];
     
     LPSTR szTemp = (LPSTR)(LPCTSTR)str;
     PathRemoveFileSpecA(szTemp);
     CString fld(szTemp);
     folder_path[0] = fld;
     folder_path[0] += "\\";
     *//*
	 //Debug: show full path
	 //AfxMessageBox(file_path[0]);
     m_monitor.Clear();
	 //m_editbox.SetWindowText(_T("Selected file: \""+file_name[0]+"\""));
     str = "Selected file: \""+file_name+"\""; 
	 m_monitor.UpdateWindow(); 
	 
	 //Check file
    txt_file_length = Get_FileSize(file_path);
    if(txt_file_length>256000 || 
	   txt_file_length==0)
	   {AfxMessageBox(_T("File size error"), MB_ICONSTOP); return;}

     file_selected = 1;
   }
  else return;

  //mem for file data 
  char *pTxtBuf = new char[txt_file_length];
  Read_File(file_path, (unsigned char*)pTxtBuf, 0, txt_file_length); 

  //-------------------------------------------------------------
  //parse file and convert to digital array to DataBufer[2048];
  //-------------------------------------------------------------
  unsigned int i;
  file_length = 0;
  pBuf = pTxtBuf;
  pBuf1 = pTxtBuf;
  pBuf2 = pTxtBuf;

  j=0;
  index = 0;
  
  for(i=0; i<=txt_file_length; i++)
  {
    //search end of string code
    char symbol_0 = pBuf[i];
    char symbol_1 = pBuf[i+1];
   
    if(symbol_0==0x0D && symbol_1==0x0A) //if(i == file_length)
    {
      //clear buffer
      memset(Buf, 0, sizeof(Buf));
      memcpy(Buf, pBuf1, &pBuf[i]-pBuf1);
      pBuf1 = &pBuf[i+2];

      //(i-Читает целое в любом формате)
      result = sscanf_s(Buf,"%i",(unsigned long*)&digit);
      if(result==1)
       { 
         //Put sample digit to signal array
         DataBufer[j++] = (signed short)digit; //1:1
                              
         file_length++;
         index++; 
        } 
      else 
       { 
         errors++;
         if(err_string==0) err_string = index+1;
       }
     }     

      if(index>2048){AfxMessageBox(_T("File to long, more then 2048 samples")); break;} 
    }     
   
  //errors
  if(errors==0 && index>0)
  {
     file_parsed = 1;
     txt.Format("%d", index); 
     str += "\r\nThe file successfully parsed to: ";
     str +=txt + " points"; 
     m_monitor.SetWindowText(str); 
  }  

  if(index==0) 
   {
     file_length=0;
	 txt = "File format is not for Windows OS!";
	 txt +="\nEnd of string must have Windows CR+LF (ASCII 0x0D 0x0A)";
     AfxMessageBox(_T(txt), MB_ICONSTOP);
   }   
  
  if(errors!=0) 
   {
     file_length=0;
     str = _T("Sample file parsed with ");
     txt.Format(" %d error(s)!", errors);
     str += txt; 
                                                 
     str += _T("\nCheck please value syntax.\nError on string: ");
     txt.Format("%d", err_string);
     str += txt; 
     AfxMessageBox(str);
   }   
    
  delete [] pTxtBuf;
  pTxtBuf = NULL;
   
  //Create_picture();
  GRPH.graph_height = 360;
  GRPH.graph_width = 2000;
  GRPH.AutoResize(DataBufer, 2000);
  GRPH.CreateGraph(DataBufer, 360, 2000, "Test");
  //BZI.LoadArray(DataBufer, 2000);
  //BZI.DoBezier();
  m_but_ShowGraph.EnableWindow(TRUE);
}*/

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnButtonFileOpen()
{
  CString str;
  unsigned long file_length;
  int result;
  /*
  file_path = "E:\\NUS\\Projects\\1NanoBioLab\\TECPID-controller\\Software\\TECPID\\PCR-files\\pcr-example.xls";
  file_name = "pcr-example.xls"; 
  */
	
  //Standart file dialog
  CFileDialog fileDialog(TRUE, NULL, _T("*.csv"), 
	                     NULL, _T("T-profile file (*.csv)"));
							    
  //Call window
  INT_PTR dlg = fileDialog.DoModal();

  if(dlg==IDOK)
   {
	 file_parsed = 0;
     file_selected = 0;

	 file_path.Empty();
     file_name.Empty();
     
	 file_path = fileDialog.GetPathName();
     file_name = fileDialog.GetFileName();
     
     /*
     str = file_path[0];
     LPSTR szTemp = (LPSTR)(LPCTSTR)str;
     PathRemoveFileSpecA(szTemp);
     CString fld(szTemp);
     folder_path[0] = fld;
     folder_path[0] += "\\";
     */	 
	 //Debug: show full path
	 //AfxMessageBox(file_path[0]);
     m_monitor.Clear();
     str = "\""+file_name+"\"";
	 m_monitor.SetWindowText(_T(str));
	 
	 //Check file
    file_length = Get_FileSize(file_path);
    if(file_length<10){AfxMessageBox(_T("File error"), MB_ICONSTOP); return;}
   }
  else return;
	
  file_selected = 1; 

  //Parse file and convert
  PCRF.Ini(&DSPL.profile);
  DSPL.index_cycles.RemoveAll();
  PCRF.pIndex_cycles = &DSPL.index_cycles;
  result = PCRF.FromCSV(file_path);

  if(result==1)
  {
    DSPL.Scroll_Setup();
    DSPL.ev_Redraw.SetEvent();
	//----
	str += "\r\nParsed: OK";
	m_monitor.SetWindowText(_T(str));
	//----
	m_Progress_Write.EnableWindow(TRUE);   
    m_Scroll.EnableWindow(TRUE);
    
	//Buttons
    m_but_ShowGraph.EnableWindow(TRUE);
    m_but_FileShow.EnableWindow(TRUE);
    m_but_FileSave.EnableWindow(TRUE);
    m_but_FromBegin.EnableWindow(TRUE);
	
	if(device_online)
	{
	  m_but_ProfileWrite.EnableWindow(TRUE);
	}
	//----
	file_parsed = 1;
  }
  else
  {
	//----
	str += "\r\nFile Parser Error!";
	switch(result)
	{
	  case -1:
	  str += "\r\n'begin' not found.";
	  break;
	  
	  case -2:
	  str += "\r\n'end' not found.";
	  break;	
	  
	  case -3:
	  str += "\r\n'cycles' param error.";
	  break;

	  case -4:
	  str += "\r\n'Sample point' error.";
	  break;
	}

	str += "\r\nCheck the syntax.";
	m_monitor.SetWindowText(_T(str));  
 	file_parsed = 0;
  }
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnButtonFileSave()
{
  //Standart file dialog
  CFileDialog fileDialog(NULL, NULL, _T("*.csv"), 
	                     NULL, _T("T-profile file (*.csv)"));
  //Call dlg window
  INT_PTR dlg = fileDialog.DoModal();

  if(dlg==IDOK)
  {
	 file_path = fileDialog.GetPathName();
     file_name = fileDialog.GetFileName();
       
	 CString str;
	 str = "\""+file_name+"\"";

     //---- Encode to file
     PCRF.Ini(&DSPL.profile);
     file_saved = PCRF.ToCSV(file_path);
     if(file_saved) str += "\r\nSaved: OK";
	 else str += "\r\nSave: Error";

	 m_monitor.SetWindowText(_T(str));
   }
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnButtonShowGraph()
{
  m_but_ShowGraph.EnableWindow(FALSE);

  //===============================================
  //Create picture
  //===============================================
  //By Y x10 = 0.1degr step 
  GRPH.SetMulY(2);
  //By X x10, 1Sec = 100px 
  GRPH.SetMulX(2); 
  //Add data
  GRPH.Parser(&DSPL.profile);
  GRPH.ParserDots(&PidLog);
  //Set horisontal scale, from 0 to AUTO, step 0.1 sec 
  GRPH.SetRangeX(0, GRPH_WIDTH_AUTO);
  //set vertical scale, from 40deg.. to 100, 0.1degr step 
  GRPH.SetRangeY(25, 300);
  //Height = 800 px = (100-20)*10, 0.1degr step
  GRPH.CreateGraph(310, GRPH_WIDTH_AUTO, "Prf");
  //BZI.LoadArray(DataBufer, 2000);
  //BZI.DoBezier();

  //Show picture
  GRPH.ShowGraph(NULL);

  m_but_ShowGraph.EnableWindow(TRUE);
}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnButtonShowFile(){PCRF.ShowFile(file_path);}

//------------------------------------------------------------------------------
//Function:	Read from remote device
//------------------------------------------------------------------------------
void CPage4::OnButtonProfileRead()
{
  m_monitor.SetWindowText(_T("Read Profile.."));
  pProgress->SetPos(0);
  char index = (char) m_combo_SelectProfile.GetCurSel();

  //Tx
  pBT->Tx(CMD_GET_PROFILE, &index, 1);
}

//------------------------------------------------------------------------------
//Function:	Read from remote device
//------------------------------------------------------------------------------
void CPage4::LoadProfile(void)
{
  if(load_profile==0) OnButtonProfileRead();
}

//------------------------------------------------------------------------------
//Function: Write to remote device
//------------------------------------------------------------------------------
void CPage4::OnButtonProfileWrite()
{
  m_monitor.SetWindowText(_T("Wait, preparation.."));
  m_monitor.UpdateWindow();

  DSPL.Scroll_Setup();
  //Update data buffer with profile points
  DSPL.Draw_Profile();
  Sleep(200);

  //Create file for load to microcontroller
  PCRF.CreateControllerFile();
  
  m_monitor.SetWindowText(_T("Transmission.."));
  m_monitor.UpdateWindow();
  Sleep(100);

  //Write file to remote device
  int ret = DeviceProfileWrite();
  if(ret!=1) m_but_ReflowRun.EnableWindow(FALSE);
  else m_but_ReflowRun.EnableWindow(TRUE);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::ButtonSoldering(int state)
{
  if(state==1)
  {
  	//Процесс запущен
	m_but_ReflowRun.SetWindowTextA("Stop\r\nReflow");
    //m_Reflow_enabled = 1;
  }
  else 
  {
  	//Процесс остановлен
	m_but_ReflowRun.SetWindowTextA("Launch\r\nReflow");
	//m_Reflow_enabled = 0;
  }
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnButtonReflowPause()
{
  if(m_Reflow_enabled==1)
  {
	char pause = 40;
	 /*
    //Если процесс запущен
    if(m_Reflow_pause==0)
    {
	  pause = 0;
	  m_monitor.SetWindowText(_T("Resume..."));
	  m_Reflow_pause = 1;
	  m_but_ReflowPause.SetWindowText(_T("Resume"));
    }
   else
   {
	 pause = 20;
	 m_monitor.SetWindowText(_T("Pause..."));
	 m_Reflow_pause = 0;
	 m_but_ReflowPause.SetWindowText(_T("Pause"));
   }  */
   
   //----
   pBT->Tx(CMD_SET_PAUSE, &pause, 1);
  }
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnButtonReflowRun()
{
  int result;
 //спросить устройство о состоянии
/// if(pPage2!=NULL) result = pPage2->GetDeviceSetup(CAN_device_ID);

  if(m_Reflow_enabled==0)
  {
	if(m_Reflow_pause==0)
	{
	  pConsole->Clear();

	  //---------------------------------------
      //Open new log file
      //---------------------------------------
      //all time create new file
      LOG_RFLW.NewFile(); 
      //LOG_RFLW.TimeStamp();

	  //Launch soldering process
	  pBT->Tx(CMD_SOLDERING_RUN, NULL, 0);
	  m_but_FromBegin.EnableWindow(FALSE);
	  m_combo_SelectProfile.EnableWindow(FALSE);
	  m_but_ReflowPause.EnableWindow(TRUE);
	  m_but_ProfileWrite.EnableWindow(FALSE);
      m_but_ProfileRead.EnableWindow(FALSE);
	  m_Reflow_pause = 0;
	}
	else
	{
	  //Pause soldering process
	  pBT->Tx(CMD_SOLDERING_PAUSE, NULL, 0);
	  m_but_ReflowPause.EnableWindow(TRUE);
	  m_Reflow_pause = 0;
	}
	  
	//Set button
	ButtonSoldering(1);
	m_monitor.SetWindowText(_T("Reflow begin"));
	  
	//сбросить кнопку From Begin
	m_Reflow_enabled = 1;
	from_begin = 0;

	//Включить мониторинг, потом убрать это и сделать 
	//чтение температуры через сообщения устройства
	//pMonitoring->Control(CAN_device_ID, 1);
  }
  else 
  {
	//Stop soldering process
	pBT->Tx(CMD_SOLDERING_STOP, NULL, 0);
	
	//----
	ButtonSoldering(0);
	m_monitor.SetWindowText(_T("Device stoped"));
    m_but_FromBegin.EnableWindow(TRUE);
	m_combo_SelectProfile.EnableWindow(TRUE);
	m_but_ReflowPause.EnableWindow(FALSE);
	m_Reflow_enabled = 0;
	m_Reflow_pause = 0;
	m_but_ReflowPause.SetWindowText("Pause");
	m_but_ProfileWrite.EnableWindow(TRUE);
    m_but_ProfileRead.EnableWindow(TRUE);
	//установить кнопку From Begin - продолжить
	from_begin = 1;
  }
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnButtonReflowReset()
{
  DSPL.Scroll_Handler(SB_RESET, -1);
  //нажали кнопку From Begin
  from_begin = 0; 
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnPaint()
{
  //device context for painting 
  CPaintDC dc(this); 
  DSPL.ev_Redraw.SetEvent();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage4::ThreadLaunch(void)
{
  //Stop threads 
  ThreadStop();

  //Restart thread
  pThread = AfxBeginThread((AFX_THREADPROC)PAGE5_THREAD, 
	                        this, THREAD_PRIORITY_IDLE);// THREAD_PRIORITY_NORMAL
  pThread->m_bAutoDelete = FALSE;
}   

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage4::ThreadStop(void)
{
 ev_Quit.SetEvent();

 if(pThread)
 {
   WaitForSingleObject(pThread->m_hThread, INFINITE);
   delete pThread; 
   pThread = NULL;
 }

 ev_Quit.ResetEvent();
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnLButtonDown(UINT nFlags, CPoint point)
{
  DSPL.LButtonDown(nFlags, point);
  //m_txt_PCRnumber.SetFocus();
  CPropertyPage::OnLButtonDown(nFlags, point);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
BOOL CPage4::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  DSPL.MouseWheel(nFlags,zDelta,pt);
  return CPropertyPage::OnMouseWheel(nFlags, zDelta, pt);
}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnMouseMove(UINT nFlags, CPoint point)
{
  DSPL.MouseMove(nFlags, point);
  CPropertyPage::OnMouseMove(nFlags, point);
}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnLButtonUp(UINT nFlags, CPoint point)
{
  DSPL.LButtonUp(nFlags, point);
  CPropertyPage::OnLButtonUp(nFlags, point);
}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  DSPL.Scroll_Handler(nSBCode, nPos);
  CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}
//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
BOOL CPage4::PreTranslateMessage(MSG* pMsg)
{
  //Tooltip msg handler
  ToolTip.PreTranslateMessage(pMsg);
  return CPropertyPage::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnButtonAnalysis()
{
  //CAN_RxChannelClose(pCAN, MONITORING_CHN, 0);
  //monitoring = 0;
  
  //----
  AnalysisRead();
  
  //restore channel 
  //int res = CAN_RxChannelOpen(pCAN,&ev_DataAccepted, MONITORING_CHN, 0);
  //if(res>0) monitoring = 1;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnCheckConsoleLogEnable()
{
  char check = (char) m_check_show_log_console.GetCheck();
  
  pBT->Tx(CMD_SET_LOG_CONFIG, &check, 1);
  EVNT_log = 0;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnCheckPCRdataLog()
{
  int check = m_check_show_log_console.GetCheck();	
  pREG->SetIntVar("page5_log", check);
  write_log = check;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnButtonSettings()
{ /*
  CAnGraphDlg *pSetupDlg;
  pSetupDlg = new CAnGraphDlg();
  pSetupDlg->Create(IDD_IMG_SETUP_DIALOG, this->GetWindow(IDD_Page5));
  pSetupDlg->ShowWindow(SW_SHOW);
  pSetupDlg->IniPointers(this);	
 */
  OpenSetupDlg();
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
void CPage4::OnSelchangeProfile()
{
  m_monitor.SetWindowText(_T("Set ptofile..."));
  char index = (char) m_combo_SelectProfile.GetCurSel(); 
	  
  //Tx
  pBT->Tx(CMD_SET_PROFILE, &index, 1);
  pConsole->Clear();
}
 
//------------------------------------------------------------------------------
//Function: 
//------------------------------------------------------------------------------
void CPage4::OpenSetupDlg(void)
{			 
 if(pDlg==NULL) 
  {
    pDlg = new CReflowSetupDlg();
    pDlg->pDev = pDevice;
    pDlg->pREG = pREG;
//	pDlg->chn = chn;
//	pDlg->enabled = pAPP->bt_connected;
	pDlg->pCWnd = FromHandle(this->m_hWnd); 
    pDlg->Create(IDD_REFLOW_SETUP_DIALOG, FromHandle(this->m_hWnd));
    pDlg->ShowWindow(SW_SHOW);
  }
}
//-----------------------------------------------------------------------------
//Function:
//-----------------------------------------------------------------------------
BOOL CPage4::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
  if(message==UWM_DIALOG_MESSAGES)
  {
 
   unsigned char chn = (unsigned char)lParam; 

   switch(wParam)
   {
	  //----
      case DEV_DIALG_CLOSE:
	  pDlg = NULL;
      return 1;

	  //----
      case DEV_SAVE_SETUP:
      break;

	  //----
      case DEV_SET_STP_DEFAULT:
 //     Sensors_SetSetupDefault((void*)&pDevice->TS[chn]);
	  TRACE("Set Setup Default, Ch: %d\n", chn+1);
	  break;

	  //----
      case DEV_SET_SENSOR_TYPE:
      TRACE2("Set Sensor Type: %d Ch: %d\n", pDevice->TS[chn].sensor_type, chn+1);
      break;	
     
      //----
      case DEV_SET_FILTER:
      TRACE2("Set Filter: %d Ch: %d\n", pDevice->TS[chn].filter, chn+1);
      break;
      
      //----
      case DEV_SET_MSR_RES:
 	  TRACE2("Set Measure Resolution: %d Ch: %d\n", pDevice->TS[chn].measure_resolution, chn+1);
      break;

      //----
      case DEV_SET_AMB_RES:
	  TRACE2("Set Ambient Resolution: %d Ch: %d\n", pDevice->TS[chn].ambient_resolution, chn+1);
      break;

      //----
      case DEV_SET_BURST:
	  TRACE2("Set Burst Samples: %d Ch: %d\n", pDevice->TS[chn].burst_samples, chn+1);
      break;

      //----
      case DEV_SET_MODE:
	  TRACE2("Set Mode: %d Ch: %d\n", pDevice->TS[chn].mode, chn+1);
      break;
      
	  //----
      case DEV_SET_CALIBR:
	  TRACE2("Set Calibration: %f Ch: %d\n", pDevice->TS[chn].calibration, chn+1);
      break;

	  default: return 1;
   }
   /*
	//data
	char buffer[sizeof(MCP9600_Setup)+1];
	int length = sizeof(MCP9600_Setup);
	memcpy(&buffer[1], &pDevice->TS[chn], length);
	buffer[0] = chn; 

  	//TX
  	pBT->Tx(CMD_SET_SENSORS_SETUP, (char*)buffer, length+1);   */
  	return 1;
  }

  return CPropertyPage::OnWndMsg(message, wParam, lParam, pResult);
}
