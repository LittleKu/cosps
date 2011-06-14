#if !defined(AFX_PROPDLG_H__7457BB9D_2478_43C8_B8F3_7666EC9C38CC__INCLUDED_)
#define AFX_PROPDLG_H__7457BB9D_2478_43C8_B8F3_7666EC9C38CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropDlg dialog

class CPropDlg : public CDialog
{
// Construction
public:
	CPropDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropDlg();
	void SetTitle(LPCTSTR lpszTitle);
	void AddProperty(LPCTSTR lpszName, LPCTSTR lpszDefaultValue = NULL);
	BOOL GetProperty(LPCTSTR lpszName, CString& szValue);
	CString GetProperty(LPCTSTR lpszName);
// Dialog Data
	//{{AFX_DATA(CPropDlg)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	LPCTSTR m_lpTitle;
	CMapStringToString m_mapPropIn;
	CMap<UINT, UINT, CString, CString&> m_mapPropInOrder;
	CMapStringToString m_mapPropOut;
	// Generated message map functions
	//{{AFX_MSG(CPropDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CPtrList m_LabelList;
	CPtrList m_EditList;
	void MoveWindowTo(int nWndID, int x = -1, int y = -1);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPDLG_H__7457BB9D_2478_43C8_B8F3_7666EC9C38CC__INCLUDED_)
