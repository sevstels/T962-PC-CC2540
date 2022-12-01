//==============================================================================
//File name:   "Page5.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page5.h"
#include "cmd.h"

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

//------------------------------------------------------------------------------
//CPage5 property page
//------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CPage5, CPropertyPage)

//------------------------------------------------------------------------------
//CPage5 constructor
//------------------------------------------------------------------------------
CPage5::CPage5() : CPropertyPage(CPage5::IDD)
{
  pDlg[0] = NULL;
  pDlg[1] = NULL;
  pDlg[2] = NULL;
  pDlg[3] = NULL;
}

//------------------------------------------------------------------------------
//CPage5 destructor
//------------------------------------------------------------------------------
CPage5::~CPage5()
{
  //Delete opened dlg windows
  for(int i=0; i<4; i++)
  {
	if(pDlg[i]!=NULL)
	{ 
	  pDlg[i]->DestroyWindow();
	  delete [] pDlg[i];
	  pDlg[i] = NULL;
	}
  }
}

//------------------------------------------------------------------------------
//CPage5 property page
//------------------------------------------------------------------------------
void CPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//----
	DDX_Control(pDX, IDC_COMBO_TS1, m_combo_ts[0]);
	DDX_Control(pDX, IDC_COMBO_TS2, m_combo_ts[1]);
	DDX_Control(pDX, IDC_COMBO_TS3, m_combo_ts[2]);
	DDX_Control(pDX, IDC_COMBO_TS4, m_combo_ts[3]);

	DDX_Control(pDX, IDC_COMBO_TS_CALC,  m_combo_ts_calc);
	DDX_Control(pDX, IDC_COMBO_HTR_CALC, m_combo_htr_calc);
	DDX_Control(pDX, IDC_CHECK_TS1, m_check_pcbs[0]);
	DDX_Control(pDX, IDC_CHECK_TS2, m_check_pcbs[1]);
	DDX_Control(pDX, IDC_CHECK_TS3, m_check_pcbs[2]);
	DDX_Control(pDX, IDC_CHECK_TS4, m_check_pcbs[3]);
	DDX_Control(pDX, IDC_CHECK_TS_TOP_LEFT, m_check_htrs[0]);
	DDX_Control(pDX, IDC_CHECK_TS_TOP_RIGHT, m_check_htrs[1]);
}

//------------------------------------------------------------------------------
//CPage5 message map
//------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CPage5, CPropertyPage)
  //----
  ON_BN_CLICKED(IDC_CHECK_TS_TOP_LEFT, &CPage5::OnCheckTsTopLeft)
  ON_BN_CLICKED(IDC_CHECK_TS_TOP_RIGHT, &CPage5::OnCheckTsTopRight)
  ON_BN_CLICKED(IDC_BUTTON_CFG_TS1, &CPage5::OnButtonCfgTS1)
  ON_BN_CLICKED(IDC_BUTTON_CFG_TS2, &CPage5::OnButtonCfgTS2)
  ON_BN_CLICKED(IDC_BUTTON_CFG_TS3, &CPage5::OnButtonCfgTS3)
  ON_BN_CLICKED(IDC_BUTTON_CFG_TS4, &CPage5::OnButtonCfgTS4)
  ON_BN_CLICKED(IDC_CHECK_TS1, &CPage5::OnCheckTS1)
  ON_BN_CLICKED(IDC_CHECK_TS2, &CPage5::OnCheckTS2)
  ON_BN_CLICKED(IDC_CHECK_TS3, &CPage5::OnCheckTS3)
  ON_BN_CLICKED(IDC_CHECK_TS4, &CPage5::OnCheckTS4)
  ON_CBN_SELCHANGE(IDC_COMBO_HTR_CALC, &CPage5::OnComboCalcHTR)
  ON_CBN_SELCHANGE(IDC_COMBO_TS_CALC, &CPage5::OnComboCalcTS)

END_MESSAGE_MAP()

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
BOOL CPage5::OnInitDialog()
{
  CPropertyPage::OnInitDialog();
  //Add extra initialization here  
  //----
  Controls_Ini();
  //Disable controls
  Controls_Enable(FALSE);	
  
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
  //return TRUE unless you set the focus to a control
  return TRUE;  
  //EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------------------
//Functions:
//------------------------------------------------------------------------------
void CPage5::OnCheckTsTopLeft(){Sensors_Enable();}
void CPage5::OnCheckTsTopRight(){Sensors_Enable();}
void CPage5::OnCheckTS1(){Sensors_Enable();}
void CPage5::OnCheckTS2(){Sensors_Enable();}
void CPage5::OnCheckTS3(){Sensors_Enable();}
void CPage5::OnCheckTS4(){Sensors_Enable();}

void CPage5::Sensors_Enable(void)
{
  char enable[2] = {0,0};
  //----
  if(m_check_htrs[0].GetCheck()) enable[0] |= 0x01;
  if(m_check_htrs[1].GetCheck()) enable[0] |= 0x02;  
  
  //----
  if(m_check_pcbs[0].GetCheck()) enable[1] |= 0x01;
  if(m_check_pcbs[1].GetCheck()) enable[1] |= 0x02;
  if(m_check_pcbs[2].GetCheck()) enable[1] |= 0x04;
  if(m_check_pcbs[3].GetCheck()) enable[1] |= 0x08;
  
  pBT->Tx(CMD_SENSORS_ENABLE, (char*)&enable, 2);
}

//------------------------------------------------------------------------------
//Functions:
//------------------------------------------------------------------------------
void CPage5::OnComboCalcHTR(){Sensors_CalcMode();}
void CPage5::OnComboCalcTS(){Sensors_CalcMode();}
void CPage5::Sensors_CalcMode(void)
{
  char mode[2] = {0,0};
  
  //----
  mode[0] = (char) m_combo_htr_calc.GetCurSel(); 
  mode[1] = (char) m_combo_ts_calc.GetCurSel(); 
  //TX  
  pBT->Tx(CMD_SENSORS_CALC_MODE, (char*)&mode, 2);
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage5::OnButtonCfgTS1(){OpenSensorDlg(0);}
void CPage5::OnButtonCfgTS2(){OpenSensorDlg(1);}
void CPage5::OnButtonCfgTS3(){OpenSensorDlg(2);}
void CPage5::OnButtonCfgTS4(){OpenSensorDlg(3);}

//------------------------------------------------------------------------------
//Function: 
//------------------------------------------------------------------------------
void CPage5::OpenSensorDlg(int chn)
{			 
 if(pDlg[chn]==NULL) 
  {
    pDlg[chn] = new CSensorSetupDlg();
    pDlg[chn]->pDev = pDevice;
    pDlg[chn]->pREG = pREG;
	pDlg[chn]->chn = chn;
	pDlg[chn]->enabled = pAPP->bt_connected;
	pDlg[chn]->pCWnd = FromHandle(this->m_hWnd); 
    pDlg[chn]->Create(IDD_SENSOR_SETUP_DIALOG, FromHandle(this->m_hWnd));
    pDlg[chn]->ShowWindow(SW_SHOW);
  }
}

//-----------------------------------------------------------------------------
//Function:
//-----------------------------------------------------------------------------
/*
LRESULT CPage5::OnDlgMessgess(WPARAM wParam, LPARAM lParam)
{
  char old_scan = 0;
  unsigned char chn = (unsigned char)lParam; 
   
  return 1;
}

  //----
  switch(wParam)
  {
	  //----
      case DEV_DIALG_CLOSE:
	  pDlg[chn] = NULL;
      return 1;

	  //----
      case DEV_SAVE_SETUP:
      break;

	  //----
      case DEV_SET_SENSOR_TYPE:
/*	  cs_MCP.Lock();
      TS[chn].SetSensorType(DEV.TS[chn].sensor_type);
	  cs_MCP.Unlock();	*/	 /*
	  TRACE2("Set Sensor Type: %d Ch: %d\n", pDevice->TS[chn].sensor_type, chn+1);
      break;	
     
      //----
      case DEV_SET_FILTER:
/*	  cs_MCP.Lock();
	  TS[chn].SetFilter(DEV.TS[chn].filter);
	  cs_MCP.Unlock(); */  /*
	  TRACE2("Set Filter: %d Ch: %d\n", pDevice->TS[chn].filter, chn+1);
      break;
      
      //----
      case DEV_SET_MSR_RES:
/*	  cs_MCP.Lock();
	  TS[chn].SetMeasureResolution(DEV.TS[chn].measure_resolution);
	  cs_MCP.Unlock();	*/ /*
	  TRACE2("Set Measure Resolution: %d Ch: %d\n", pDevice->TS[chn].measure_resolution, chn+1);
      break;

      //----
      case DEV_SET_AMB_RES:
/*	  cs_MCP.Lock();
	  TS[chn].SetAmbientResolution(DEV.TS[chn].ambient_resolution);
	  cs_MCP.Unlock();	 */	 /*
	  TRACE2("Set Ambient Resolution: %d Ch: %d\n", pDevice->TS[chn].ambient_resolution, chn+1);
      break;

      //----
      case DEV_SET_BURST:
/*	  cs_MCP.Lock();
	  TS[chn].SetBurstSampleNumber(DEV.TS[chn].burst_samples);
	  cs_MCP.Unlock(); */  /*
	  TRACE2("Set Burst Samples: %d Ch: %d\n", pDevice->TS[chn].burst_samples, chn+1);
      break;

      //----
      case DEV_SET_MODE:
/*	  cs_MCP.Lock();
	  TS[chn].SetMode(DEV.TS[chn].mode);
	  if(DEV.TS[chn].mode)  if(pDlg[chn]!=NULL) pDlg[chn]->LED_Control(0);
	  cs_MCP.Unlock();	*/	/*
	  TRACE2("Set Mode: %d Ch: %d\n", pDevice->TS[chn].mode, chn+1);
      break;
    }
/*
  //Calc new setup crc
  DEV.TS[chn].crc = TS[chn].CalcSettingsCRC(&DEV.TS[chn], 10);
  
  //Save settings to Registry
  REG.SetBinVar("dev_setup", (char*)&DEV, sizeof(DEV));
 */	 /*
  return 1;
}
*/

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CPage5::Sensors_SetSetupDefault(void *pSetup)
{
  MCP9600_Setup *pSTP;
  pSTP = (MCP9600_Setup*)pSetup;
  memset(pSTP, 0, sizeof(MCP9600_Setup));
  
  //----
  pSTP->calibration = 0;   //no any offsets
  pSTP->sensor_type = 0;   //MCP9600_TYPE_K;
  pSTP->filter = 4;        //Mid filter
  pSTP->mode = 0;          //normal operation
  pSTP->burst_samples = 3; //3 sample
  pSTP->measure_resolution = 1;//MCP9600_ADC_16bit;
  pSTP->ambient_resolution = 1;//MCP9600_Amb_025C;
}

//-----------------------------------------------------------------------------
//Function:
//-----------------------------------------------------------------------------
BOOL CPage5::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
  if(message==UWM_DIALOG_MESSAGES)
  {
 
   unsigned char chn = (unsigned char)lParam; 

   switch(wParam)
   {
	  //----
      case DEV_DIALG_CLOSE:
	  pDlg[chn] = NULL;
      return 1;

	  //----
      case DEV_SAVE_SETUP:
      break;

	  //----
      case DEV_SET_STP_DEFAULT:
      Sensors_SetSetupDefault((void*)&pDevice->TS[chn]);
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

	//data
	char buffer[sizeof(MCP9600_Setup)+1];
	int length = sizeof(MCP9600_Setup);
	memcpy(&buffer[1], &pDevice->TS[chn], length);
	buffer[0] = chn; 

  	//TX
  	pBT->Tx(CMD_SET_SENSORS_SETUP, (char*)buffer, length+1);
  	return 1;
  }

  return CPropertyPage::OnWndMsg(message, wParam, lParam, pResult);
}
