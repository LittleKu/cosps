#if !defined(AFX_PPGGENERAL_H__0E903951_8955_4A8F_A34C_2D47994C10B4__INCLUDED_)
#define AFX_PPGGENERAL_H__0E903951_8955_4A8F_A34C_2D47994C10B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPgGeneral.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPgGeneral dialog

class CPPgGeneral : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPgGeneral)

// Construction
public:
	CPPgGeneral();
	~CPPgGeneral();

// Dialog Data
	//{{AFX_DATA(CPPgGeneral)
	enum { IDD = IDD_PROPPAGE_GENERAL };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPgGeneral)
public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPgGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnSettingsChange()		{ SetModified(); }
	afx_msg void OnBtnOutputDir();
	afx_msg void OnBtnTempDir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void LoadSettings();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPGGENERAL_H__0E903951_8955_4A8F_A34C_2D47994C10B4__INCLUDED_)
