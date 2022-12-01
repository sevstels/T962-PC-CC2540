//==============================================================================
//File name:    "parse-value.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2021, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "parse-value.h"

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
//
//------------------------------------------------------------------------------
int ParseValueFl(CEdit *pEdit, float &out, float min, float max)
{
  CString txt;
  double value;
  int result;
  float out_value;

  pEdit->GetWindowTextA(txt);

  //read value in any format
  result = sscanf_s(txt,"%Lf",(double*)&value);
  
  if(result<1)
  {
    txt = "This is not digit!\r\nEnter a number...";  
	::AfxMessageBox(txt);
	return -1;
  }

  if(value>max || value<min)
   {
	  txt.Format("Value must be\r\nbetween: %f.. %f", min, max);      
      ::AfxMessageBox(txt);
      return -1;
   } 

  out_value = (float)value;
  out = out_value;

  return 1;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int ParseValueSh(CEdit *pEdit, short &out, short min, short max)
{
  CString txt;
  double value;
  int result;
  short out_value;
 
  pEdit->GetWindowTextA(txt);
  
  //read value in any format
  result = sscanf_s(txt,"%Lf",(double*)&value);
  
  if(result<1)
  {
    txt = "This is not digit!\r\nEnter a number...";  
	::AfxMessageBox(txt);
	return -1;
  }
  
  if(value>max || value<min)
  {
	txt.Format("Value must be\r\nbetween: %d.. %d", min, max);      
    ::AfxMessageBox(txt);
    return -1;
  } 

  out_value = (short)value;
  out = out_value;

  return 1;
}

//------------------------------------------------------------------------------
//Function:
//------------------------------------------------------------------------------
int ParseValueCh(CEdit *pEdit, char &out, char min, char max)
{
  CString txt;
  double value;
  int result;
  char out_value;
 
  pEdit->GetWindowTextA(txt);
  
  //read value in any format
  result = sscanf_s(txt,"%Lf",(double*)&value);
  
  if(result<1)
  {
    txt = "This is not digit!\r\nEnter a number...";  
	::AfxMessageBox(txt);
	return -1;
  }
  
  if(value>max || value<min)
  {
	txt.Format("Value must be\r\nbetween: %d.. %d", min, max);      
    ::AfxMessageBox(txt);
    return -1;
  } 

  out_value = (char)value;
  out = out_value;

  return 1;
}

 int ParseValueUCh(CEdit *pEdit, unsigned char &out, 
	               unsigned char min, unsigned char max)
 {
  CString txt;
  double value;
  int result;
  char out_value;
 
  pEdit->GetWindowTextA(txt);
  
  //read value in any format
  result = sscanf_s(txt,"%Lf",(double*)&value);
  
  if(result<1)
  {
    txt = "This is not digit!\r\nEnter a number...";  
	::AfxMessageBox(txt);
	return -1;
  }
  
  if(value>max || value<min)
  {
	txt.Format("Value must be\r\nbetween: %d.. %d", min, max);      
    ::AfxMessageBox(txt);
    return -1;
  } 

  out_value = (char)value;
  out = out_value;

  return 1;
 }