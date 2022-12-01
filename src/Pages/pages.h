//==============================================================================
//File name:   "pages.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2022, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _PAGES_H_
#define _PAGES_H_

//==============================================================================
// Initialization for Pages
//==============================================================================
  //в структуру записываются параметры создаваемой закладки
  TC_ITEM tci; 
  tci.mask = TCIF_TEXT;		// у закладки будет только текст
  tci.iImage = -1; 
 
  //Page 1
  tci.pszText = "TC";//Tab page name
  m_Tab.InsertItem(0, &tci);   //Insert
  
  //Page 2
  tci.pszText = "PID"; // название закладки
  m_Tab.InsertItem(1, &tci);//
  			
  //Page 3
  tci.pszText = "Heater";// название закладки
  m_Tab.InsertItem(2, &tci);//

  //Page 4
  tci.pszText = "Reflow"; // название закладки
  m_Tab.InsertItem(3, &tci);//
  
  //Page 5
  tci.pszText = "T-Sensors"; // название закладки
  m_Tab.InsertItem(4, &tci);// 
  
  //Page 6
  tci.pszText = "Common"; // название закладки
  m_Tab.InsertItem(5, &tci);// 

  //------------------------------------------------------------------------------
  //Initialization for Page 1
  //------------------------------------------------------------------------------
  //Tab settings
  tci.mask = TCIF_PARAM;		 //Tab have only text
  tci.lParam = (LPARAM)pPage1;	 //
  m_Tab.SetItem(0, &tci);		 //Index 0
  
  pPage1->Create(CPage1::IDD, &m_Tab);
  pPage1->SetWindowPos(NULL, 2, 22, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  pPage1->ShowWindow(SW_HIDE);
  //pPage1->InitPage();

  //------------------------------------------------------------------------------
  //Initialization for Page 2
  //------------------------------------------------------------------------------
  //Tab settings
  tci.mask = TCIF_PARAM; //Tab have only text
  tci.lParam = (LPARAM)pPage2; //
  m_Tab.SetItem(1, &tci);	   //Index 1
  
  pPage2->Create(CPage2::IDD, &m_Tab);
  pPage2->SetWindowPos(NULL, 2, 22, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  pPage2->ShowWindow(SW_HIDE);
  //pPage2->DrawWindow();
  //pPage2->InitPage();

  //------------------------------------------------------------------------------
  //Initialization for Page 3
  //------------------------------------------------------------------------------
  //Tab settings
  tci.mask = TCIF_PARAM;
  tci.lParam = (LPARAM)pPage3;
  m_Tab.SetItem(2, &tci);

  pPage3->Create(CPage3::IDD, &m_Tab);
  pPage3->SetWindowPos(NULL, 2, 22, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  pPage3->ShowWindow(SW_HIDE);
  //pPage4->DrawWindow();
  //pPage4->InitPage();

  //------------------------------------------------------------------------------
  //Initialization for Page 4
  //------------------------------------------------------------------------------
  //Tab settings
  tci.mask = TCIF_PARAM;
  tci.lParam = (LPARAM)pPage4;
  m_Tab.SetItem(3, &tci);

  pPage4->Create(CPage4::IDD, &m_Tab);
  pPage4->SetWindowPos(NULL, 2, 22, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  pPage4->ShowWindow(SW_HIDE);
  //pPage4->DrawWindow();
  //pPage4->InitPage();
    
  //------------------------------------------------------------------------------
  //Initialization for Page 5
  //------------------------------------------------------------------------------
  //Tab settings
  tci.mask = TCIF_PARAM;
  tci.lParam = (LPARAM)pPage5;
  m_Tab.SetItem(4, &tci);

  pPage5->Create(CPage5::IDD, &m_Tab);
  pPage5->SetWindowPos(NULL, 2, 22, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  pPage5->ShowWindow(SW_HIDE);
  //pPage5->DrawWindow();
  //pPage5->InitPage();

  //------------------------------------------------------------------------------
  //Initialization for Page 6
  //------------------------------------------------------------------------------
  //Tab settings
  tci.mask = TCIF_PARAM;
  tci.lParam = (LPARAM)pPage6;
  m_Tab.SetItem(5, &tci);

  pPage6->Create(CPage6::IDD, &m_Tab);
  pPage6->SetWindowPos(NULL, 2, 22, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  pPage6->ShowWindow(SW_HIDE);
  //pPage8->DrawWindow();
  //pPage8->InitPage(); 

 #endif
