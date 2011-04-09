#if !defined(AFX_LISTTOOLTIP_H__46850DED_6582_486A_AFB1_5C0C4845D570__INCLUDED_)
#define AFX_LISTTOOLTIP_H__46850DED_6582_486A_AFB1_5C0C4845D570__INCLUDED_

//#include "ResUtilities.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TooltipsEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTooltipList window

// Les messages snt mis a jour grace a la notification TTN_NEEDTEXT
// envoyee au parent du CTooltipList
//    BOOL OnToolTipText(UINT id, TOOLINFO *pInfo);
//       id = iSubItem | (iItem << 16);
//       pInfo->lpszText = "Texte du Tooltips"

class CTooltipList : public CWnd
{
// Construction
public:
	CTooltipList();

// Attributes
public:

// Operations
public:
	void RelayEvent(MSG* pMsg);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTooltipList)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void ListChange();
	virtual ~CTooltipList();

   COLORREF m_bkColor, m_textColor;
	BOOL Create(CWnd *pParent, CListCtrl *pList);

protected:
   CWnd     *m_pParent;
   CListCtrl*m_pList;
   CBrush   m_brush;
   CFont    m_font;
   CPoint   m_curPos;   // position du tooltip (screen coordonnees)
   int      m_timer;
   int      m_tCnt;     // compte le temps en 0.1 secondes
   int      m_timOut;   // duree d'affichage en 0.1 secondes
   TOOLINFO m_toolInfo; // message affiche

	// Generated message map functions
protected:
	//{{AFX_MSG(CTooltipList)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	//}}AFX_MSG
   afx_msg HBRUSH CtlColor(CDC *pDC, UINT);
	DECLARE_MESSAGE_MAP()
};

extern UINT ToolTipNotify(NMHDR *pNMH);
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTTOOLTIP_H__46850DED_6582_486A_AFB1_5C0C4845D570__INCLUDED_)
