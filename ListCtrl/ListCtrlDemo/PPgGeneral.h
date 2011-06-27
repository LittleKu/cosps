#if !defined(AFX_PPGGENERAL_H__21248E10_0C32_422A_AB19_3A9A7D768341__INCLUDED_)
#define AFX_PPGGENERAL_H__21248E10_0C32_422A_AB19_3A9A7D768341__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPgGeneral.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPgGeneral dialog

class CPPgGeneral : public CPropertyPage
{
// Construction
public:
	CPPgGeneral();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPPgGeneral)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPPgGeneral)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void LoadSettings();
	// Generated message map functions
	//{{AFX_MSG(CPPgGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnSettingsChange()		{ SetModified(); }
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPGGENERAL_H__21248E10_0C32_422A_AB19_3A9A7D768341__INCLUDED_)
