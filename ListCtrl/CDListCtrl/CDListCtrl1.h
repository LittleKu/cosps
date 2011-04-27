#if !defined(AFX_CDLISTCTRL1_H__49C04633_53F9_4DBF_B868_14A80515CA7D__INCLUDED_)
#define AFX_CDLISTCTRL1_H__49C04633_53F9_4DBF_B868_14A80515CA7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDListCtrl1.h : header file
//

#include "CDHeaderCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCDListCtrl window
class CCDListCtrlData
{
public:
	CCDListCtrlData()
	{
		pCheckStates         = NULL;
		dwItemData           = 0;
	}
	~CCDListCtrlData()
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
class CCDListCtrl : public CListCtrl
{
// Construction
public:
	CCDListCtrl();
	virtual ~CCDListCtrl();
// Attributes
public:
	CCDHeaderCtrl   m_HeaderCtrl;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateSubItem(int nItem, int nSubItem);
	void SetItemCheckedState(int nItem, int nSubItem, int nCheckedState);
	int GetItemCheckedState(int nItem, int nSubItem);
	BOOL GetSubItemRect(int iItem, int iSubItem, int nArea, CRect& ref);
	void GetDrawColors(int nItem, int nSubItem, COLORREF& colorText, COLORREF& colorBkgnd);
	BOOL	SetHeaderCheckedState(int nSubItem, int nCheckedState);
	int GetHeaderCheckedState(int nSubItem);
	int CountCheckedItems(int nSubItem);
	int InsertItem(const LVITEM* pItem);
	DWORD GetItemData(int nItem) const;
	BOOL	DeleteAllItems();
	BOOL	DeleteItem(int nItem);
	// Generated message map functions
protected:
	void DrawCheckbox(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect& rect, CCDListCtrlData *pListCtrlData);
	//{{AFX_MSG(CCDListCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg BOOL OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	COLORREF		m_cr3DFace;
	COLORREF		m_cr3DHighLight;
	COLORREF		m_cr3DShadow;
	COLORREF		m_crActiveCaption;
	COLORREF		m_crBtnFace;
	COLORREF		m_crBtnShadow;
	COLORREF		m_crBtnText;
	COLORREF		m_crGrayText;
	COLORREF		m_crHighLight;
	COLORREF		m_crHighLightText;
	COLORREF		m_crInactiveCaption;
	COLORREF		m_crInactiveCaptionText;
	COLORREF		m_crWindow;
	COLORREF		m_crWindowText;
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLISTCTRL1_H__49C04633_53F9_4DBF_B868_14A80515CA7D__INCLUDED_)
