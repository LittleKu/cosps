#if !defined(AFX_CHECKLISTCTRL_H__3F0719F5_7A7D_4DF8_AE0B_EAB2462F1677__INCLUDED_)
#define AFX_CHECKLISTCTRL_H__3F0719F5_7A7D_4DF8_AE0B_EAB2462F1677__INCLUDED_

#include "CheckHeaderCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckListCtrl.h : header file
//
extern UINT WM_CHECK_LIST_CTRL_CHECKBOX_CLICKED;

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
	int             m_nRowHeight;
	CImageList      m_ILRowHeight;
// Operations
public:
	int  InsertItem( int nItem, LPCTSTR lpszItem );
	int  InsertItem(const LVITEM* pItem, BOOL bVirgin = FALSE);
	BOOL DeleteItem(int nItem);
	BOOL DeleteAllItems();
	void ValidateCheck();
	void SetRowHeight(int nRowHeight);
	int  GetRowHeight();
	DWORD GetItemData(int nItem) const;

	void InvalidateSubItem(int nItem, int nSubItem);
	void SetItemCheckedState(int nItem, int nSubItem, int nCheckedState, BOOL bUpdateImmediately = TRUE);
	int  GetItemCheckedState(int nItem, int nSubItem);
	BOOL GetSubItemRect(int nItem, int nSubItem, int nArea, CRect& rect);
	int  CountCheckedItems(int nSubItem);
	int	 GetHeaderCheckedState(int nSubItem);
	void SetHeaderCheckedState(int nSubItem, int nCheckedState);
	void DrawCheckbox(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect, BOOL bDrawMark);
	void DrawText(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect& rect);
	void GetDrawColors(int nItem, int nSubItem, COLORREF& colorText, COLORREF& colorBkgnd);
	void CalcCheckBoxRect(int nItem, int nSubItem, CRect& checkboxRect, BOOL bCenter = FALSE, int h = 13);
	inline int* GetCheckedState(int nItem);
private:
	void SetItemCheckedStateByClick(int nItem, int nSubItem, int nCheckedState, BOOL bUpdateHeader = TRUE);
	BOOL IsPtInSubItemCheckBox(int nItem, int nSubItem, POINT pt);
	inline int SwitchCheckedState(int nCheckedState);
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
