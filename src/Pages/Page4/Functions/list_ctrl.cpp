//==============================================================================
//File name:   "xlist.cpp"
//Purpose:      Source File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru	
//==============================================================================
#include "stdafx.h"
#include "Page4.h"
#include <stdlib.h>

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
//initialization
//------------------------------------------------------------------------------
void CPage4::TableIni(void)
{
  m_bGridlines = TRUE;

  //-------------------------------------------------------
  //XListCtrl ini must have:
  //LVS_EX_FULLROWSELECT if combo or edit boxes are used
  DWORD dwExStyle = LVS_EX_FULLROWSELECT;//| LVS_EX_TRACKSELECT for hot tracking
  if(m_bGridlines) dwExStyle |= LVS_EX_GRIDLINES;

  m_List.SetExtendedStyle(dwExStyle);
  
  cellName.RemoveAll(); 
  cellName.Add("1 ");
  cellName.Add("2 ");
  cellName.Add("3 ");
  cellName.Add("4 ");
  cellName.Add("5 ");
  cellName.Add("6 ");
  cellName.Add("7 ");
  cellName.Add("8 ");

  //инициализируем шапку таблицы
  TableHeader();
  
  //заливаем таблицу
  TableFill();
}

//------------------------------------------------------------------------------ 
// InitListCtrl
//------------------------------------------------------------------------------
void CPage4::TableHeader(void)
{
  ASSERT(m_List);
  if(!m_List) return;

  int i=0, vscroll, last = 0, width, total_width = 0;
  
  //set column width according to window rect
  CRect rect;
  m_List.GetWindowRect(&rect);
  
  //get width
  width = rect.Width();
  LV_COLUMN lvcolumn;
  memset(&lvcolumn, 0, sizeof(lvcolumn));

  //width of vertical scroll bar
  vscroll = ::GetSystemMetrics(SM_CXVSCROLL);	
  width = width - vscroll -4; 

  //-----------------------------------------------------------------
  //Заголовок таблицы
  //-----------------------------------------------------------------
  for(;;)
  {
    lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;

    //---- колонка 1
    if(i==0)
    {
      lvcolumn.fmt = LVCFMT_CENTER;//LVCFMT_LEFT; //LVCFMT_RIGHT
	  lvcolumn.pszText = _T("№");
      //Width of the column, in pixels
      lvcolumn.cx = 30;
    }                                   
    
    //---- колонка 2
    if(i==1)
    {
      lvcolumn.fmt = LVCFMT_CENTER;
      lvcolumn.pszText =  _T("Time");
      //Width of the column, in pixels
      lvcolumn.cx = 50;
    }

    //---- колонка 3
    if(i==2)
    {
      lvcolumn.fmt = LVCFMT_CENTER;
      lvcolumn.pszText =  _T("T °C");
      //Width of the column, in pixels
      lvcolumn.cx = 60;
    }

    //---- колонка 4
    if(i==3)
    {
      lvcolumn.fmt = LVCFMT_CENTER;
      lvcolumn.pszText =  _T("R");
      //Width of the column, in pixels
      lvcolumn.cx = 24;
    }
    //---- колонка 5
    if(i==4)
    {
      lvcolumn.fmt = LVCFMT_CENTER;
      lvcolumn.pszText =  _T("G");
      //Width of the column, in pixels
      lvcolumn.cx = 24;
    }
    //---- колонка 6
    if(i==5)
    {
      lvcolumn.fmt = LVCFMT_CENTER;
      lvcolumn.pszText =  _T("B");
      //Width of the column, in pixels
      lvcolumn.cx = 24;
    }

    //---- колонка 7
    if(i==6)
    {
      lvcolumn.fmt = LVCFMT_CENTER;
      lvcolumn.pszText =  _T("Channel");
      //Width of the column, in pixels
      lvcolumn.cx = (width - total_width - 4);
 	  last = 1;
    }

    //Index of subitem associated with the column.
    lvcolumn.iSubItem = i;
    m_List.InsertColumn(i, &lvcolumn);
    if(last==1) break;
    
    i++;
    total_width += lvcolumn.cx;
  }

#if 0  
  // -----------------------------------------------------------
  // create the image list from bitmap resource
  VERIFY(pList->m_cImageList.Create(13, 13, ILC_COLOR24, 3, 1));
  CBitmap bm;
  bm.LoadBitmap(IDB_CHECKBOXES);
  pList->m_cImageList.Add(&bm, RGB(255, 0, 255));
  pList->m_HeaderCtrl.SetImageList(&pList->m_cImageList);
#endif // -----------------------------------------------------------

  // iterate through header items and attach the image list
  HDITEM hditem;
  int count = m_List.m_HeaderCtrl.GetItemCount();
  for (i = 0; i<count; i++)
  {
    hditem.mask = HDI_IMAGE | HDI_FORMAT;
    m_List.m_HeaderCtrl.GetItem(i, &hditem);
    hditem.fmt |=  HDF_IMAGE;
    if (i == 0 || i == 3)
      {/*hditem.iImage = XHEADERCTRL_UNCHECKED_IMAGE;*/}
    else
      hditem.iImage = XHEADERCTRL_NO_IMAGE;

    m_List.m_HeaderCtrl.SetItem(i, &hditem);
  } 

  //переставляем форматирование колонок снова, если оно не совпадает с хедером
  //set the format again - must do this twice or first column does not get set
  memset(&lvcolumn, 0, sizeof(lvcolumn));
  
  for (i = 0; i<5; i++)
  {
    lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
    lvcolumn.fmt = LVCFMT_CENTER;
    //if(i==2) lvcolumn.fmt = LVCFMT_LEFT;
    //lvcolumn.fmt = (i == 5) ? LVCFMT_RIGHT : lvcolumn.fmt;
    lvcolumn.iSubItem = i;
    m_List.SetColumn(i, &lvcolumn);
  }
}

//------------------------------------------------------------------------------ 
//FillListCtrl
//------------------------------------------------------------------------------
void CPage4::TableFill(void)
{ 
  //lock window updates while filling list
  m_List.LockWindowUpdate();	
  m_List.DeleteAllItems();
  
  CString txt; 
  int item, subitem;

  //insert the items and subitems into the list	   pDO->BOM.xlc_length
  for(item=0; item < 100; item++)
  {   
	//По числу столбцов
    for(subitem=0; subitem<6; subitem++)
    {
      //---------------------------------------------------------------
      //
      //---------------------------------------------------------------
      if(subitem==0)
	  {
		 m_List.InsertItem(item, "1");
	  }

      //---------------------------------------------------------------
      //Number
      //---------------------------------------------------------------
      if(subitem==0)	//   pDO->BOM.xlc_length
	  {	 /*
		 int selected = 5;	//pDO->BOM.XLC[item].column
		 m_List.SetComboBox(item, subitem, 
                            TRUE,//enable combobox
                           &cellName,//pointer to CStringArray
                            cellName.GetCount(),//size of dropdown
                            selected-1,//initial combo selection
                            FALSE);//nosort CStringArray
		 
		 //не работает почему то
		 //CString text = cellName.GetAt(item);
		 //m_List.SetItemText(item, subitem, text, RGB(200,200,0), RGB(0xFF,0xFF,0xFF));
		 //m_List.SetItemColors(item,0,RGB(0,200,0),RGB(255,255,255));
		 m_List.SetBold(item, subitem, 1); */
		 txt.Format("%d", item);
		 m_List.SetItemText(item, subitem, txt);

	  }

      //---------------------------------------------------------------
      //добавить чек бокс "On"
      //---------------------------------------------------------------      
      if(subitem==1)
      {	/*
		//setup_atribytes
		int check = 1;//GetCheck(item);
		  
        //проверка использования правила        
        m_List.SetCheckbox(item, subitem, check);
        //показать на экране
        if(check==1)
         {
	       m_List.SetBold(item, 0, 1);
	       m_List.SetItemColors(item,0,RGB(0,0,0),RGB(255,255,255));
         }
        else{m_List.SetItemColors(item,0,RGB(255,255,255),RGB(200,200,200));}
		*/
	   	 txt.Format("%d", item);
		 m_List.SetItemText(item, subitem, txt);
      }
      
      //---------------------------------------------------------------
      //добавить имя параметра 
      //---------------------------------------------------------------      
      if(subitem==2)
      {
		m_List.SetCheckbox(item, subitem, 1);
		m_List.SetItemText(item, subitem, txt);
      } 
      //---------------------------------------------------------------
      //добавить имя параметра 
      //---------------------------------------------------------------      
      if(subitem==3)
      {
		m_List.SetCheckbox(item, subitem, 1);
      }  
      if(subitem==4)
      {
		m_List.SetCheckbox(item, subitem, 1);
      }
      if(subitem==5)
      {
		m_List.SetCheckbox(item, subitem, 1);
      }

     } 
    }//for
       
#ifndef NO_XLISTCTRL_TOOL_TIPS
      /*
  // set tool tips in second row
  nItem = 1;
  for (SubItem = 0; SubItem < 6; SubItem++)
  {
    str.Format(_T("This is %d,%d"), nItem, SubItem);
    pList->SetItemToolTipText(nItem, SubItem, str);
  }
    */
#endif
  //pList->SetCurSel(0,0);
  //pList->SetItemCheckedState(0,0,1);
  //pList->SetFocus();
  //pList->SetItemState(1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
  
  //DWORD dwSel = LVIS_SELECTED|LVIS_FOCUSED;
  //pList->SetItem(0, 0, LVIF_STATE, 0, 0, dwSel, dwSel, 0);

  //unlock window updates
  m_List.UnlockWindowUpdate();
}
