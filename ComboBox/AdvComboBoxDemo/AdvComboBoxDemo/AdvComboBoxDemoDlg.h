/////////////////////////////////////////////////////////////////////////////
// AdvComboBoxDemoDlg.h : header file
//
// CAdvComboBox Control
// Version: 2,0
// Date: August 2002
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

#if !defined(AFX_ADVCOMBOBOXDEMODLG_H__564F38A7_4E17_4863_86E9_1E4BEA7CBF30__INCLUDED_)
#define AFX_ADVCOMBOBOXDEMODLG_H__564F38A7_4E17_4863_86E9_1E4BEA7CBF30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAdvComboBoxDemoDlg dialog
#include "AdvComboBox.h"

class CAdvComboBoxDemoDlg : public CDialog
{
// Construction
public:
	CAdvComboBoxDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAdvComboBoxDemoDlg)
	enum { IDD = IDD_ADVCOMBOBOXDEMO_DIALOG };
	CSpinButtonCtrl	m_ctlIndexSpin;
	CSpinButtonCtrl	m_ctlVisibleItemsSpin;
	CEdit	m_ctlVisibleItemsEdit;
	CEdit	m_ctlIndexEdit;
	CAdvComboBox	m_ctlAdvCombo;
	BOOL	m_bCheckedList;
	BOOL	m_bFlatStyle;
	BOOL	m_bDisabledIndex;
	BOOL	m_bCheckedIndex;
	int		m_nIndex;
	BOOL	m_bAutoAppend;
	BOOL	m_bAutoSuggest;
	int		m_nVisibleItems;
	//}}AFX_DATA
	int		m_nAdvCombo;
	CString	m_strAdvCombo;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvComboBoxDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAdvComboBoxDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCheckedCheck();
	afx_msg void OnFlatCheck();
	afx_msg void OnIdxDisabledCheck();
	afx_msg void OnIdxCheckedCheck();
	afx_msg void OnChangeIndexEdit();
	afx_msg void OnGettextButton();
	afx_msg void OnGetidxButton();
	afx_msg void OnAutosuggestCheck();
	afx_msg void OnAutoappendCheck();
	afx_msg void OnChangeVisibleItemsEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitAdvComboBox();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVCOMBOBOXDEMODLG_H__564F38A7_4E17_4863_86E9_1E4BEA7CBF30__INCLUDED_)
