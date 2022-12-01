// EnterEdit.cpp : implementation file
//

#include "stdafx.h"
#include "EnterEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnterEdit

CEnterEdit::CEnterEdit()
{
}

CEnterEdit::~CEnterEdit()
{
}


BEGIN_MESSAGE_MAP(CEnterEdit, CEdit)
    //{{AFX_MSG_MAP(CEnterEdit)
    ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnterEdit message handlers

void CEnterEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if(nChar == VK_RETURN)
    {													   /*
        CEditDlgDlg* pDlg = (CEditDlgDlg*) GetParent();
        GetWindowText(pDlg->m_edText);
        pDlg->GetNextDlgTabItem(this)->SetFocus();		 */
		return;
    }
    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEnterEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_RETURN)
		return;
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
