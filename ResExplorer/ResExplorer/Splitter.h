// Splitter.h : header file
//
#if !defined(AFX_SPLITTER_H__31CDA561_ED8E_4714_A772_239CEE9C1631__INCLUDED_)
#define AFX_SPLITTER_H__31CDA561_ED8E_4714_A772_239CEE9C1631__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSplitter frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CSplitterWndEx : public CSplitterWnd
{
	DECLARE_DYNAMIC(CSplitterWndEx)

// Implementation
public:
	CSplitterWndEx();
	~CSplitterWndEx();


   virtual void DeleteView(int row, int col);
	virtual BOOL DestroyWindow();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITTER_H__31CDA561_ED8E_4714_A772_239CEE9C1631__INCLUDED_)
