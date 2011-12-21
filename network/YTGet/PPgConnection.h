#if !defined(AFX_PPGCONNECTION_H__9F38E6F8_84A0_4839_B563_45D6BB78AF2C__INCLUDED_)
#define AFX_PPGCONNECTION_H__9F38E6F8_84A0_4839_B563_45D6BB78AF2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPgConnection.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPgConnection dialog

class CPPgConnection : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPgConnection)

// Construction
public:
	CPPgConnection();
	~CPPgConnection();

// Dialog Data
	//{{AFX_DATA(CPPgConnection)
	enum { IDD = IDD_PROPPAGE_CONNECTION };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPgConnection)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPgConnection)
	virtual BOOL OnInitDialog();
	afx_msg void OnSettingsChange()		{ SetModified(); }
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL SaveParameter(UINT nCtrlID, UINT& nParam, UINT nMin, UINT nMax, LPCTSTR lpszParamName, UINT nMultiple = 1);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPGCONNECTION_H__9F38E6F8_84A0_4839_B563_45D6BB78AF2C__INCLUDED_)
