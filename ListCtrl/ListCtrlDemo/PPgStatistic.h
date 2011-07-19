#if !defined(AFX_PPGSTATISTIC_H__2EE7A980_F572_4CBF_B36C_F73491AF6B9B__INCLUDED_)
#define AFX_PPGSTATISTIC_H__2EE7A980_F572_4CBF_B36C_F73491AF6B9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPgStatistic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPgStatistic dialog

class CPPgStatistic : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPgStatistic)

// Construction
public:
	CPPgStatistic();
	~CPPgStatistic();

// Dialog Data
	//{{AFX_DATA(CPPgStatistic)
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPgStatistic)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void LoadSettings();
	// Generated message map functions
	//{{AFX_MSG(CPPgStatistic)
	virtual BOOL OnInitDialog();
	afx_msg void OnOnSettingsChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPGSTATISTIC_H__2EE7A980_F572_4CBF_B36C_F73491AF6B9B__INCLUDED_)
