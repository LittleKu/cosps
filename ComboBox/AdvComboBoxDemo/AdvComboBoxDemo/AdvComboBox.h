/////////////////////////////////////////////////////////////////////////////
// AdvComboBox.h : header file
// 
// CAdvComboBox Control
// Version: 2.1
// Date: September 2002
// Author: Mathias Tunared
// Email: Mathias@inorbit.com
// Copyright (c) 2002. All Rights Reserved.
//
// This code, in compiled form or as source code, may be redistributed 
// unmodified PROVIDING it is not sold for profit without the authors 
// written consent, and providing that this notice and the authors name 
// and all copyright notices remains intact.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADVCOMBOBOX_H__F643B3EA_6268_4EF9_8479_7E3C737DBD53__INCLUDED_)
#define AFX_ADVCOMBOBOX_H__F643B3EA_6268_4EF9_8479_7E3C737DBD53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4251 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4275 )

#include "AdvComboBoxDef.h"
#include <vector>

void AFXAPI DDX_ACBIndex( CDataExchange* pDX, int nIDC, int& index );
void AFXAPI DDX_ACBString( CDataExchange* pDX, int nIDC, CString& value );


/////////////////////////////////////////////////////////////////////////////
// CAdvComboBox window
#define ADVCOMBOBOXCTRL_CLASSNAME    _T("AdvComboBoxCtrl")  // Window class name

class CDropWnd;

class CAdvComboBox : public CWnd
{
// Construction
public:
	CAdvComboBox();

	DECLARE_DYNAMIC(CAdvComboBox)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvComboBox)
	public:
	virtual BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void LoadString( UINT nStringID = 0 );

	//String Operations
	int AddString( LPCTSTR lpszString );
	int InsertString( int nIndex, LPCTSTR lpszString );
	int DeleteString( UINT nIndex );
	void DeleteAllItems();
	void ResetContent();
	int FindString( int nStartAfter, LPCTSTR lpszString );
	int FindStringExact( int nIndexStart, LPCTSTR lpszFind );
	int SelectString( int nStartAfter, LPCTSTR lpszString );

	//Item Operations
	PLIST_ITEM AddItem(const LIST_ITEM* pListItem, PLIST_ITEM pParent = NULL);
	int DeleteItem(UINT nIndex);

	int GetDefaultVisibleItems();
	void SetDefaultVisibleItems( int nItems = -1 );
	virtual CFont* GetFont() {return m_pFont;}
	int GetMinVisibleItems();
	void SetMinVisibleItems( int nMinItems );
	BOOL LimitText( int nMaxChars );
	BOOL SetEditSel( int nStartChar, int nEndChar );
	DWORD GetEditSel();
	BOOL GetExtendedUI();
	int SetExtendedUI( BOOL bExtended = TRUE );
	BOOL GetDroppedState();
	void GetDroppedControlRect( LPRECT lprect );
	void ShowDropDown( BOOL bShowIt = TRUE );
	int InitStorage( int nItems, UINT nBytes );
	int SetTopIndex( int nIndex );
	int GetTopIndex();
	void SetItemChecked( int nIndex, BOOL bChecked );
	BOOL GetItemChecked( int nIndex );
	void SetItemDisabled( int nIndex, BOOL bDisabled );
	BOOL GetItemDisabled( int nIndex );
	
	void* GetItemDataPtr( int nIndex );
	int SetItemDataPtr( int nIndex, void* pData );
	DWORD GetItemData( int nIndex );
	int SetItemData( int nIndex, DWORD dwItemData );
	int SetCurSel( int nSelect );
	int GetCount();
	int GetCurSel();
	void SetText( LPCTSTR lpszText );
	void GetText( CString& rString );
	int GetText( LPTSTR lpszText );
	void GetLBText( int nIndex, CString& rString );
	int GetLBText( int nIndex, LPTSTR lpszText );
	int GetLBTextLen(int nIndex );
	int SetItemHeight( int nIndex,  int nHeight );
	virtual ~CAdvComboBox();

	DWORD GetACBStyle() const { return m_dwACBStyle; }
	void ModifyACBStyle( UINT nRemoveStyle, UINT nAddStyle );
	BOOL PointInWindow( CPoint ptScreenPoint );

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdvComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnChildActivate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnMouseLeave();
	afx_msg LONG OnSelectedItem( WPARAM wParam, LPARAM lParam );
	afx_msg LONG OnDropdownButton( WPARAM wParam, LPARAM lParam );
	afx_msg LONG OnDestroyDropdownList( WPARAM wParam, LPARAM lParam );
	afx_msg void OnKillfocusEdit();
	afx_msg void OnSetfocusEdit();
	afx_msg void OnChangeEdit();
	afx_msg void OnUpdateEdit();

	// ComboBox messages
	afx_msg LONG OnAddString( WPARAM wParam, LPARAM lString );
	afx_msg LONG OnSetCurSel( WPARAM wIndex, LPARAM lParam );
	afx_msg LONG OnGetCurSel( WPARAM wParam, LPARAM lParam );
	afx_msg LONG OnSelectString( WPARAM wItemStart, LPARAM lString );
	afx_msg LONG OnGetCount( WPARAM wParam, LPARAM lParam );
	afx_msg LONG OnResetContent( WPARAM wParam, LPARAM lParam );
	afx_msg LONG OnGetLBText( WPARAM wIndex, LPARAM lString );
	afx_msg LONG OnGetLBTextLen( WPARAM wIndex, LPARAM lParam );
	afx_msg LONG OnGetTopIndex( WPARAM wParam, LPARAM lParam );
	afx_msg LONG OnSetTopIndex( WPARAM wIndex, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

    BOOL RegisterWindowClass();
	virtual void DrawDropDown(CDC* pDC);
	virtual void DrawDropDownList(CDC* pDC);
	BOOL DrawDropDownXPStyle(CDC* pDC);
	void DrawDropDownDefaultStyle(CDC* pDC);

private:
	void CreateDropList( std::vector<PLIST_ITEM> &droplist );
	void SelPrevItem();
	void SelNextItem();
	PLIST_ITEM GetListItem(int nIndex);
	int ToComboBoxIndex(PLIST_ITEM pItem);

	int m_nCurSel;
	CEdit* m_pEdit;
	CString m_strEdit;
	CFont* m_pFont;
	CPen m_pen;
	DWORD m_dwACBStyle;

	CRect m_rcDropButton;

	CDropWnd* m_pDropWnd;
	BOOL m_bDropListVisible;

	std::vector<PLIST_ITEM> m_list;

	int m_zDelta; // MouseWheel...

	bool m_bHasSentFocus;
	bool m_bSelItem;
	bool m_bFirstPaint;
	bool m_bCodeCreate;
	BOOL m_bAutoAppend;
	bool m_bDropButtonHot;
	bool m_bTrackMouseLeave;

	int m_nMinVisItems;
	int m_nDefaultDropItems; // Number of items to be shown when dropwindow first shown.
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVCOMBOBOX_H__F643B3EA_6268_4EF9_8479_7E3C737DBD53__INCLUDED_)
