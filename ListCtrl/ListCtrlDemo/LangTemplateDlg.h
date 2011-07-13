#if !defined(_LANG_TEMPLATE_DLG_H_)
#define _LANG_TEMPLATE_DLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LangTemplateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLangTemplateDlg dialog
class CDlgTemplate;

#define LAYOUT_TYPE_PROP			0
#define LAYOUT_TYPE_SEPARATOR		1
#define LAYOUT_TYPE_CHECKBOX		2

class CLangTemplateDlg : public CDialog
{
protected:
	class LayoutInfo
	{
	public:
		int nType;
		CString sPropName;
		CString sPropValue;
		int nGridWidth;
		LayoutInfo() : nType(LAYOUT_TYPE_PROP), sPropName(), sPropValue(), nGridWidth(1)
		{
		}
		LayoutInfo(int type, LPCTSTR lpszName, LPCTSTR lpszDefaultValue, int gridwidth)
			: nType(type), sPropName(lpszName), sPropValue(lpszDefaultValue), nGridWidth(gridwidth)
		{
		}
	};
// Construction
public:
	void UpdatePropValue();
	CLangTemplateDlg();   // standard constructor
	virtual ~CLangTemplateDlg();
	void SetModal(BOOL bModal = TRUE, CWnd* pParentWnd = NULL);
	void SetTitle(LPCTSTR lpTitle);
	void AddProperty(LPCTSTR lpszName, int gridwidth = 1, LPCTSTR lpszDefaultValue = NULL, int type = LAYOUT_TYPE_PROP);
	void AddSeparator();
	
// Dialog Data
	//{{AFX_DATA(CLangTemplateDlg)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLangTemplateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLangTemplateDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL CalcMaxLabelSize(CSize& sizeMax, int& nMaxGridWidth);
	void CalcLabelRect(LPRECT lpRect, LPCRECT lpLastRect, int nRow, int nCol, CSize szMax, int nGridWidthInPixel);
	void CalcSepRect(LPRECT lpRect, LPCRECT lpLastRect, int nClientWidth);
	void CreateControls();
protected:
	CDlgTemplate* m_dlgTemplate;
	CString m_szTitle;
	CMapStringToPtr m_mapLayout;
	CList<LayoutInfo, LayoutInfo&> m_listLayout;
	CMapStringToString m_mapPropOut;

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

#endif // !defined(_LANG_TEMPLATE_DLG_H_)
