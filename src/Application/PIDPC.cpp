//==============================================================================
//File name:    "PIDPC.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2023, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "PIDPC.h"
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

// CPIDPCApp
BEGIN_MESSAGE_MAP(CPIDPCApp, CWinAppEx)
ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
///ON_COMMAND(ID_HELP, &CWinApp::HtmlHelp)
ON_COMMAND(ID_CONTEXT_HELP, &CPIDPCApp::OnContextHelp)
END_MESSAGE_MAP()

// CPIDPCApp construction
CPIDPCApp::CPIDPCApp()
{
  //TODO: add construction code here,
  //Place all significant initialization in InitInstance
  ///EnableHtmlHelp();
}

// The one and only CPIDPCApp object
CPIDPCApp theApp;

//------------------------------------------------------------------------------
//Function:	CPIDPCApp initialization
//------------------------------------------------------------------------------
BOOL CPIDPCApp::InitInstance()
{
	CWinAppEx::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization

	CAppWinReg_ini;
	CMainDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		// dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		// dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	// application, rather than start the application's message pump.
	return FALSE;
}

//------------------------------------------------------------------------------
//Function:	
//------------------------------------------------------------------------------
void CPIDPCApp::OnContextHelp()
{
  // TODO: Add your command handler code here
}

//------------------------------------------------------------------------------
//Function:	
//------------------------------------------------------------------------------
void CPIDPCApp::HtmlHelp(DWORD_PTR dwData, UINT nCmd)
{
  //TODO: Add your specialized code here and/or call the base class
  CWinAppEx::HtmlHelp(dwData, nCmd);
}
