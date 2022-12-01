//==============================================================================
//File name:   "ColorStaticST.h"
//Purpose:      Header File
//Version:      1.00
//Copyright:    (c) 2019, Akimov Vladimir  E-mail: decoder@rambler.ru		
//==============================================================================
#ifndef _COLOR_STATIC_STRING_H_
#define _COLOR_STATIC_STRING_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CColorStaticST : public CStatic
{
  // Construction
  public:
  CColorStaticST();
  enum {ST_FLS_SLOW,
        ST_FLS_NORMAL,
        ST_FLS_FAST};

  // Attributes
  public:

  // Operations
  public:

  // Implementation
  public:
  virtual ~CColorStaticST();

  void SetTextColor(COLORREF crTextColor = 0xffffffff);
  COLORREF GetTextColor();

  void SetBkColor(COLORREF crBkColor = 0xffffffff);
  COLORREF GetBkColor();

  void SetBlinkTextColors(COLORREF crBlinkTextColor1, COLORREF crBlinkTextColor2);
  void StartTextBlink(BOOL bStart = TRUE, UINT nElapse = ST_FLS_NORMAL);

  void SetBlinkBkColors(COLORREF crBlinkBkColor1, COLORREF crBlinkBkColor2);
  void StartBkBlink(BOOL bStart = TRUE, UINT nElapse = ST_FLS_NORMAL);

  void EnableNotify(CWnd* pParent = NULL, UINT nMsg = WM_USER);

  static const short GetVersionI();
  static const char* GetVersionC();

  // Generated message map functions
  protected:
  //{{AFX_MSG(CColorStaticST)
  afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnDestroy();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
  
  private:
  UINT_PTR m_nTimerId;

  COLORREF m_crTextColor;
  COLORREF m_crBlinkTextColors[2];
  BOOL m_bTextBlink;
  int m_nTextBlinkStep;

  COLORREF m_crBkColor;
  COLORREF m_crBlinkBkColors[2];
  BOOL m_bBkBlink;
  int m_nBkBlinkStep;

  CBrush m_brBkgnd;
  CBrush m_brBlinkBkgnd[2];

  CWnd* m_pParent;
  UINT m_nMsg;
};

#endif