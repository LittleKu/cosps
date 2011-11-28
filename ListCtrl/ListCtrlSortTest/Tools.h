// Tools.h: interface for the CTools class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLS_H__E7B4EDF2_0499_4D22_B42E_7356C4EA8799__INCLUDED_)
#define AFX_TOOLS_H__E7B4EDF2_0499_4D22_B42E_7356C4EA8799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTools  
{
public:
	CTools();
	virtual ~CTools();
public:
	static void DrawCheckBox(CDC* pDC, LPCRECT lpcRect, BOOL bDrawMark, COLORREF crBkg = ::GetSysColor(COLOR_WINDOW), 
		COLORREF crBorder = RGB(51,102,153), COLORREF crMark = RGB(51,153,51));
	static BOOL CalcCheckBoxRect(const CRect& boundRect, CRect& checkboxRect, BOOL bCenter = FALSE, int h = 13);
};

#endif // !defined(AFX_TOOLS_H__E7B4EDF2_0499_4D22_B42E_7356C4EA8799__INCLUDED_)
