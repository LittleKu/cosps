#if !defined(AFX_CHECKLISTCTRL_H__3F0719F5_7A7D_4DF8_AE0B_EAB2462F1677__INCLUDED_)
#define AFX_CHECKLISTCTRL_H__3F0719F5_7A7D_4DF8_AE0B_EAB2462F1677__INCLUDED_

#include "CheckHeaderCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckListCtrl.h : header file
//

class CCheckListItemData
{
public:
	CCheckListItemData()
	{
		pCheckStates         = NULL;
		dwItemData           = 0;
	}
	~CCheckListItemData()
	{
		if(pCheckStates != NULL)
		{
			delete [] pCheckStates;
			pCheckStates = NULL;
		}
	}
	// for checkbox
	int*       pCheckStates;				// -1 = don't show, 0 = unchecked, 1 = checked
	DWORD      dwItemData;		            // pointer to app's data				
};
/////////////////////////////////////////////////////////////////////////////
// CCheckListCtrl window

class CCheckListCtrl : public CListCtrl
{
// Construction
public:
	CCheckListCtrl();

// Attributes
public:
	CCheckHeaderCtrl m_HeaderCtrl;
	COLORREF		m_crBtnFace;
	COLORREF		m_crHighLight;
	COLORREF		m_crHighLightText;
	COLORREF		m_crWindow;
	COLORREF		m_crWindowText;
// Operations
public:
	int InsertItem(const LVITEM* pItem, BOOL bVirgin = FALSE);
	void DrawCheckbox(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect, BOOL bDrawMark);
	void GetDrawColors(int nItem, int nSubItem, COLORREF& colorText, COLORREF& colorBkgnd);
	BOOL GetSubItemRect(int nItem, int nSubItem, int nArea, CRect& rect);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckListCtrl)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCheckListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCheckListCtrl)
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg BOOL OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKLISTCTRL_H__3F0719F5_7A7D_4DF8_AE0B_EAB2462F1677__INCLUDED_)
