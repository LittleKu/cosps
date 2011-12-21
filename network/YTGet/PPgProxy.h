#if !defined(AFX_PPGPROXY_H__B8A5C765_15A2_4565_952F_8DE62941D2B2__INCLUDED_)
#define AFX_PPGPROXY_H__B8A5C765_15A2_4565_952F_8DE62941D2B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPgProxy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPPgProxy dialog

class CPPgProxy : public CPropertyPage
{
	DECLARE_DYNCREATE(CPPgProxy)

// Construction
public:
	CPPgProxy();
	~CPPgProxy();

// Dialog Data
	//{{AFX_DATA(CPPgProxy)
	enum { IDD = IDD_PROPPAGE_PROXY };
	CEdit	m_editAddress;
	CEdit	m_editPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPPgProxy)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPPgProxy)
	virtual BOOL OnInitDialog();
	afx_msg void OnSettingsChange()		{ SetModified(); }
	afx_msg void OnRadioNoProxy();
	afx_msg void OnRadioProxyManually();
	afx_msg void OnRadioProxyFromIe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL SaveProxy();
	void RadioSelChanged(int nIDCheckButton);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPGPROXY_H__B8A5C765_15A2_4565_952F_8DE62941D2B2__INCLUDED_)
