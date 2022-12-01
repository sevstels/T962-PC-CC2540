//==============================================================================
//File name:   "AppWinReg.h"
//Purpose:      Header File
//Version:      2.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _APPLICATION_WINREG_SERVICE_H_
#define _APPLICATION_WINREG_SERVICE_H_

//Add this MACRO to InitInstance() or OnInitDialog
#define CAppWinReg_ini SetRegistryKey(_T((LPCTSTR)(AfxGetApp()->m_pszExeName)))

//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CAppWinReg
{	 
  //Construction
  public:
  CAppWinReg();
  virtual ~CAppWinReg();

  //Operations
  public:
  //Application first launch
  int FirstLaunch(void); 
  //In/Out
  int SetIntVar(CString var_name, int value);
  int GetIntVar(CString var_name, int &pVar);
  //----  
  int SetTxtVar(CString var_name, CString &pTxt);
  int GetTxtVar(CString var_name, CString &pTxt);
  //----
  int SetBinVar(CString var_name, const char *pBuf, long length);
  int GetBinVar(CString var_name, const char *pBuf, long length);
  
  int SetWndColor(void);

  //Attributes
  private:
  //Application pointer указатель на обьект *приложение* (на базовый класс)
  CWinApp *pApp;

  //Implementation
  protected:
  CString section;
};

#endif
