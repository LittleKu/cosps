#if !defined(AFX_HISTORYCOMBOBOX_H__489644B8_92FF_4EBE_987E_79CF199F7952__INCLUDED_)
#define AFX_HISTORYCOMBOBOX_H__489644B8_92FF_4EBE_987E_79CF199F7952__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistoryComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboBox window

class CHistoryComboBox : public CComboBox
{
// Construction
public:
	CHistoryComboBox();
	void SetIniKeyNames(LPCTSTR lpFileName, LPCTSTR lpSectionName, LPCTSTR lpKeyBaseName);
	void Init();
	void SaveHistory();
	void LoadHistory();
	int AddString( LPCTSTR lpszString );
// Attributes
public:
	int m_nMaxHistoryItems;
	CString m_sIniFile;
	CString m_sSectionName;
	CString m_sKeyBaseName;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoryComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHistoryComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHistoryComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYCOMBOBOX_H__489644B8_92FF_4EBE_987E_79CF199F7952__INCLUDED_)
