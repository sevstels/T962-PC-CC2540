//==============================================================================
//File name:   ".cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page4.h"

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
//This method handle NM_CLICK messages from XListCtrl
//Кликнуто по строке в таблице
//------------------------------------------------------------------------------
void CPage4::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
  int Item = -1;
  int SubItem = -1;
  if (pNMIA)
   {
     Item = pNMIA->iItem;
     SubItem = pNMIA->iSubItem;
   }

  //проверка наличия параметров и управления в строке 
  if(Item >= 0 && SubItem >= 0)
   {
     CString strText = m_List.GetItemText(Item, SubItem);
     Log(_T("OnClick at (%d,%d):  '%s'"), Item, SubItem, strText);
   }
 
  *pResult = 0;
}

//------------------------------------------------------------------------------
// This method handle NM_DBLCLK messages from XListCtrl
//------------------------------------------------------------------------------
void CPage4::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
  int nItem = -1;
  int nSubItem = -1;
  if(pNMIA)
   {
     nItem = pNMIA->iItem;
     nSubItem = pNMIA->iSubItem;
   }

  if(nItem >= 0 && nSubItem >= 0)
   {
     CString strText = m_List.GetItemText(nItem, nSubItem);
     Log(_T("OnDblClick at (%d,%d):  '%s'"), nItem, nSubItem, strText);
   }

  *pResult = 0;
}

//------------------------------------------------------------------------------
//This method handle LVN_COLUMNCLICK messages from XListCtrl
//------------------------------------------------------------------------------
void CPage4::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  int nItem = -1;
  int nSubItem = -1;
  if(pNMLV)
   {
     nItem = pNMLV->iItem;
     nSubItem = pNMLV->iSubItem;

	 if(nItem==-1 && nSubItem==0)
	 {
	   POINT ipoint;
	   m_List.GetItemPosition(nItem, &ipoint);
	 
	   CPoint point;
	   point.x = ipoint.x;
	   point.y = ipoint.y;

	   //CWnd *pWnd = m_List.GetOwner();
	   //ShowMenu(IDR_MENU2, pWnd, &point);
	 }
   }

  Log(_T("OnColumnClick on column %d"), nSubItem);

  CString strCaption = _T("");
  strCaption.LoadString(AFX_IDS_APP_TITLE);
  strCaption += _T(" *");
  SetWindowText(strCaption);

 *pResult = 0;
}

//------------------------------------------------------------------------------
//This method handle WM_XLISTCTRL_COMBO_SELECTION messages from XListCtrl
//------------------------------------------------------------------------------
LRESULT CPage4::OnComboSelection(WPARAM nItem, LPARAM nSubItem)
{
 if(nItem >= 0 && nSubItem >= 0)
  {
    CString strText = m_List.GetItemText(nItem, nSubItem);
    Log(_T("OnComboSelection at (%d,%d): '%s'"), nItem, nSubItem, strText);

    COLORREF crText, crBackground;
    m_List.GetItemColors(nItem, nSubItem, crText, crBackground);
        
    //subitem was modified
    if(m_List.GetModified(nItem, nSubItem))
     {
       //save
       ///SetColumnSetup(nItem, &strText);

       //if subitem was modified - color it red
       m_List.SetItemText(nItem, nSubItem, strText, RGB(255,0,0), crBackground);
     }
    //subitem not modified
    else
     {
        //subitem not modified - color it black -
        //note that once modified, a subitem will remain marked as modified
        m_List.SetItemText(nItem, nSubItem, strText, RGB(0,0,0), crBackground);
     }
  }

 return 0;
}

//------------------------------------------------------------------------------
// This method handle WM_XLISTCTRL_CHECKBOX_CLICKED messages from XListCtrl
//------------------------------------------------------------------------------
LRESULT CPage4::OnCheckbox(WPARAM nItem, LPARAM nSubItem)
{
  if(nItem<1 && nSubItem<1) return 0;

  Log(_T("OnCheckbox at (%d,%d)"), nItem, nSubItem);
  if(m_List.GetModified(nItem, nSubItem))
   { 
     int status = m_List.GetItemCheckedState(nItem, nSubItem);
     ///SaveCheck(nItem, status);
   }

  return 0;
}

//------------------------------------------------------------------------------
//This method handle LVN_ITEMCHANGED messages from XListCtrl
//------------------------------------------------------------------------------
void CPage4::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  int nItem = -1;
  int nSubItem = -1;
  if(pNMLV)
   {
     nItem = pNMLV->iItem;
     nSubItem = pNMLV->iSubItem;
   }
  //TRACE(_T("in CXListCtrlTestDlg::OnItemChanged:  %d, %d\n"), nItem, nSubItem);

  if(pNMLV && (pNMLV->uNewState == (UINT)(LVIS_FOCUSED|LVIS_SELECTED)))
   {
     CString strText = m_List.GetItemText(nItem, nSubItem);
     Log(_T("Selection changed to item %d"), nItem);
   }

 *pResult = 0;
}

//------------------------------------------------------------------------------
//This method handle WM_XLISTCTRL_EDIT_END messages from XListCtrl
//------------------------------------------------------------------------------
LRESULT CPage4::OnEditEnd(WPARAM nItem, LPARAM nSubItem)
{
    if (nItem >= 0 && nSubItem >= 0)
    {
        CString strText = m_List.GetItemText(nItem, nSubItem);
        Log(_T("OnEditEnd at (%d,%d): '%s'"), nItem, nSubItem, strText);

        COLORREF crText, crBackground;
        m_List.GetItemColors(nItem, nSubItem, crText, crBackground);
        
    //subitem was modified
    if (m_List.GetModified(nItem, nSubItem))
        {
            //check and cheange rule
      //RuleChange(nItem, nSubItem, &strText);

      //subitem was modified - color it red
      //m_List.SetItemText(nItem, nSubItem, strText, RGB(255,0,0), crBackground);
            
      /* CString strCaption = _T("");
            strCaption.LoadString(AFX_IDS_APP_TITLE);
            strCaption += _T(" *");
            SetWindowText(strCaption);*/
        }
        //subitem not modified
    else
        {
            // subitem not modified - color it black -
            // note that once modified, a subitem will remain marked as modified
            //m_List.SetItemText(nItem, nSubItem, strText, RGB(0,0,0), crBackground);
        }
    }

    return 0;
}

//------------------------------------------------------------------------------
// Log - log a message to the m_Log listbox
//------------------------------------------------------------------------------
void _cdecl CPage4::Log(LPCTSTR lpszFmt, ...)
{
  ASSERT(lpszFmt);

  TCHAR buf[2000], fmt[2000];
  va_list marker;
  
  #pragma warning(push)
  #pragma warning(disable : 4996) 

  //format string was passed as parameter
  _tcsncpy(fmt, lpszFmt, sizeof(fmt)/sizeof(TCHAR)-1);
  fmt[sizeof(fmt)/sizeof(TCHAR)-1] = _T('\0');
 
  // combine output string and variables
  va_start(marker, lpszFmt);
  _vsntprintf(buf, (sizeof(buf)/sizeof(TCHAR))-1, fmt, marker);
  va_end(marker);
  buf[sizeof(buf)/sizeof(TCHAR)-1] = _T('\0');
  
  #pragma warning(pop)
  CString strMsg(buf);

  //don't display \r or \n characters
  int i = 0;
  while((i = strMsg.FindOneOf(_T("\r\n"))) != -1)
         strMsg.SetAt(i, _T(' '));
  
  TRACE(_T("%s\n"), strMsg);
}

