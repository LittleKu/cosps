/////////////////////////////////////////////////////////////////////////////
// DropListBox.h : header file
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

#if !defined(AFX_DROPLISTBOX_H__E49DED7C_CBC2_4458_A72C_F4C428927132__INCLUDED_)
#define AFX_DROPLISTBOX_H__E49DED7C_CBC2_4458_A72C_F4C428927132__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdvComboBoxDef.h"
#include <list>

class CAdvComboBox;
class CDropWnd;

/////////////////////////////////////////////////////////////////////////////
// CDropListBox window

#define LBE_COLLAPSE			1
#define LBE_EXPAND				2
#define LBE_TOGGLE				3

class CDropListBox : public CListBox
{
// Construction
public:
	//Use CDropWnd for accessing data directly
	CDropListBox( CAdvComboBox* pComboParent, CDropWnd* pDropWnd );
	virtual ~CDropListBox();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDropListBox)
	public:
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	int AddListItem( PLIST_ITEM pItem );
	int InsertListItem(int nIndex, PLIST_ITEM pItem);
	int DeleteListItem( UINT nIndex );
	PLIST_ITEM GetListItem(UINT nIndex);
	int GetItemIndex(PLIST_ITEM pItem);
	
	int GetTotalItemHeight(int nStartIndex, int nCount = -1);
	int PointTest(CPoint point);
	int SetCurSel( int nSelect );
	void GetTextSize( LPCTSTR lpszText, int nCount, CSize& size );
	void SetTopIdx( int nPos, BOOL bUpdateScrollbar = FALSE );
	int GetBottomIndex();
	
	BOOL Expand(PLIST_ITEM pItem, UINT nCode);
	BOOL Expand(PLIST_ITEM pItem);
	BOOL Collapse(PLIST_ITEM pItem);

	// Generated message map functions
protected:
	//{{AFX_MSG(CDropListBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LONG OnSetCapture( WPARAM wParam, LPARAM lParam );
	afx_msg LONG OnReleaseCapture( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

private:
	CAdvComboBox* m_pComboParent;
	CDropWnd* m_pDropWnd;
	int m_nLastTopIdx;

	std::list<PLIST_ITEM> m_list;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DROPLISTBOX_H__E49DED7C_CBC2_4458_A72C_F4C428927132__INCLUDED_)
