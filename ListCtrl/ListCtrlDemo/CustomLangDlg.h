#if !defined(AFX_CUSTOMLANGDLG_H__E874DD3B_8CDF_4263_824F_46F8CD602B40__INCLUDED_)
#define AFX_CUSTOMLANGDLG_H__E874DD3B_8CDF_4263_824F_46F8CD602B40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomLangDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomLangDlg dialog
class CDlgTemplate;
class CCustomLangDlg : public CDialog
{
protected:
	class LayoutInfo
	{
	public:
		CString sPropName;
		CString sPropValue;
		int nGridWidth;
		int nGridHeight;
		LayoutInfo() : sPropName(), sPropValue(), nGridWidth(1), nGridHeight(1)
		{
		}
		LayoutInfo(LPCTSTR lpszName, LPCTSTR lpszDefaultValue, int gridwidth, int gridheight)
			: sPropName(lpszName), sPropValue(lpszDefaultValue), nGridWidth(gridwidth), nGridHeight(gridheight)
		{
		}
	};
// Construction
public:
	CCustomLangDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCustomLangDlg();

	void AddProperty(LPCTSTR lpszName, LPCTSTR lpszDefaultValue = NULL, int gridwidth = 1, int gridheight = 1);
	void AddSeparator();
// Dialog Data
	//{{AFX_DATA(CCustomLangDlg)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomLangDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCustomLangDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL CalcMaxLabelSize(CSize& sizeMax, int& nMaxGridWidth);
private:
	void CalcLabelRect(LPRECT lpRect, int nRow, int nCol, CSize szMax, int nMaxGridWidth);
	void CreateControls();
	CDlgTemplate* m_dlgTemplate;
	CMapStringToPtr m_mapLayout;
	CList<LayoutInfo, LayoutInfo&> m_listLayout;

	CPtrList m_listLabel;
	CPtrList m_listEdit;
	CButton* m_pOkButton;
	CButton* m_pCancelButton;
private:
	static int nStartPosX;
	static int nStartPosY;
	static int nSpaceX;
	static int nSpaceY;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMLANGDLG_H__E874DD3B_8CDF_4263_824F_46F8CD602B40__INCLUDED_)
