#if !defined(AFX_ENTEREDIT_H__F79E97AF_7253_4FF1_9B5D_D9D3570896AA__INCLUDED_)
#define AFX_ENTEREDIT_H__F79E97AF_7253_4FF1_9B5D_D9D3570896AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnterEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnterEdit window

class CEnterEdit : public CEdit
{
// Construction
public:
	CEnterEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnterEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEnterEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEnterEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTEREDIT_H__F79E97AF_7253_4FF1_9B5D_D9D3570896AA__INCLUDED_)
