// ListCtrlEx.cpp : implementation file (in place editing)
//

#include "stdafx.h"
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

CListCtrlEx::CListCtrlEx()
{
   m_hidRect.SetRectEmpty();
   m_iSubItem = -1;
}

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT_EX(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT_EX(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CListCtrlEx::SubItemEdit(int iItem, int iSubItem)
{
   m_iSubItem = iSubItem;
   return ( EditLabel(iItem) != NULL );
}
/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers

void CListCtrlEx::OnPaint() 
{
   // to correct the clear of the first subitems of the selected line
   ValidateRect(m_hidRect);

	CListCtrl::OnPaint();            // for painting the list
}

BOOL CListCtrlEx::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
   m_iSubItem = ((NM_LISTVIEW*)pNMHDR)->iSubItem;
   return FALSE;                 // to reflect this message to the parent
}

BOOL CListCtrlEx::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
   LVITEM &item = ((LV_DISPINFO*)pNMHDR)->item;
   CRect rect;

   if ( m_iSubItem >= 0 ) {
      item.iSubItem = m_iSubItem;

      // to correct the item line display and the edit window
      GetSubItemRect(item.iItem, m_iSubItem, LVIR_LABEL, rect);
      m_hidRect.SetRect(0, rect.top, rect.left, rect.bottom);
      m_edit.m_cx = rect.Width();
      m_edit.m_x = rect.left;

      // get edit control and subclass
      m_edit.SubclassWindow(GetEditControl()->m_hWnd);
      m_edit.SetWindowText(GetItemText(item.iItem, m_iSubItem));
   }
   return FALSE;                 // to reflect this message to the parent
}

BOOL CListCtrlEx::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
   LVITEM &item = ((LV_DISPINFO*)pNMHDR)->item;

   if ( m_iSubItem >= 0 && m_edit.m_hWnd != NULL ) {
      item.iSubItem = m_iSubItem;   // correct LV_DISPINFO to the parent

      // terminate this request
      m_edit.UnsubclassWindow();
      m_hidRect.SetRectEmpty();
      m_iSubItem = -1;
   }
   return FALSE;                 // to reflect this message to the parent
}
/////////////////////////////////////////////////////////////////////////////
// CEditEx

BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	//{{AFX_MSG_MAP(CEditEx)
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CEditEx message handlers

void CEditEx::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
   lpwndpos->x = m_x, lpwndpos->cx = m_cx;
	CEdit::OnWindowPosChanging(lpwndpos);
}
