// LangGrammarDlg.h: interface for the CLangGrammarDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANGGRAMMARDLG_H__15C17BC9_0EBF_4EC0_B45C_036B4514CFF1__INCLUDED_)
#define AFX_LANGGRAMMARDLG_H__15C17BC9_0EBF_4EC0_B45C_036B4514CFF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LangGrammarMap.h"

class CDlgTemplate;

#define LAYOUT_TYPE_PROP			0
#define LAYOUT_TYPE_SEPARATOR		1
#define LAYOUT_TYPE_CHECKBOX		2
class CLangTemplateDlg  : public CDialog
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
	CLangTemplateDlg();   // standard constructor
	virtual ~CLangTemplateDlg();
	void UpdatePropValue();
	void AddProperty(LPCTSTR lpszName, int gridwidth = 1, LPCTSTR lpszDefaultValue = NULL, int type = LAYOUT_TYPE_PROP);
	void AddSeparator();
	void CreateModeless(CWnd* pParentWnd = NULL);
	void SetEditWidth(int nEditWidth) { m_nEditWidth = nEditWidth; }
protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	void CreateControls();
	BOOL CalcMaxLabelSize(CSize& szMax, int& nMaxGridWidth);
	void CalcLabelRect(LPRECT lpRect, LPCRECT lpLastRect, int nRow, int nCol, CSize szMax, int nGridWidthInPixel);
	void CalcSepRect(LPRECT lpRect, LPCRECT lpLastRect, int nClientWidth);

	CDlgTemplate* m_dlgTemplate;
	CMapStringToPtr m_mapLayout;
	CList<LayoutInfo, LayoutInfo&> m_listLayout;
	CMapStringToString m_mapPropOut;
	CPtrList m_listLabel;
	CPtrList m_listEdit;
	int m_nEditWidth;
private:	
	static int nStartPosX;
	static int nStartPosY;
	static int nSpaceX;
	static int nSpaceY;
};

class CLangGrammarDlg : public CLangTemplateDlg
{
public:
	CLangGrammarDlg();
	virtual ~CLangGrammarDlg();
	BOOL GetLangGrammarInfo(CLangGrammarInfo*& pLangGrammarInfo, BOOL bValidate = FALSE, BOOL bShowError = FALSE);
	BOOL Validate(BOOL bShowError = FALSE);
public:
	static LPCTSTR lpszLangName;
	static LPCTSTR lpszLineComment;
	static LPCTSTR lpszEscapeString;
	static LPCTSTR lpszBlockCommentOn;
	static LPCTSTR lpszBlockCommentOff;
	static LPCTSTR lpszStringOn;
	static LPCTSTR lpszStringOff;
	static LPCTSTR lpszCharacterOn;
	static LPCTSTR lpszCharacterOff;
protected:
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_LANGGRAMMARDLG_H__15C17BC9_0EBF_4EC0_B45C_036B4514CFF1__INCLUDED_)
