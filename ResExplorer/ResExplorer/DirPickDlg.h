// DirPickDlg.h : interface of the CDirPickDlg class
//                               child of the CDialog class 
//
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_DIRPICKDLG_H__5BA77A63_B634_4389_BBD7_95C92196CC13__INCLUDED_)
#define AFX_DIRPICKDLG_H__5BA77A63_B634_4389_BBD7_95C92196CC13__INCLUDED_

#include "Resource.h"
#include "DirTreeCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDirPickDlg dialog

class CDirPickDlg : public CDialog
{
// Construction
public:
	CDirPickDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDirPickDlg)
	enum { IDD = IDD_DIRPICK_DLG };
	CEdit	m_edSelect;
	CDirTreeCtrl m_treeFiles;
	CComboBox	m_comboExt;
	//}}AFX_DATA

   CString m_path;
   CString m_curFilter;
   CString m_filter;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirPickDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   void SetFilter(CString filter);
   CStringArray m_filtStr;

	// Generated message map functions
	//{{AFX_MSG(CDirPickDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboExt();
	afx_msg void OnChangeEdSelect();
	afx_msg void OnSelchangedTreeFiles(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern void CFileDialogInit(CFileDialog *pCFD, LPCTSTR pDefFn=NULL, LPCTSTR pFilter=NULL);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRPICKDLG_H__5BA77A63_B634_4389_BBD7_95C92196CC13__INCLUDED_)
