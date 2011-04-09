// ListTooltip.cpp : implementation file
//

#include "stdafx.h"
#include "TooltipEx.h"

#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT ToolTipNotify(NMHDR *pNMH)
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT*)pNMH;
   UINT nID = pNMH->idFrom;
   // si flag : idForm est actuellement le HWND de l'element
	if (pTTT->uFlags & TTF_IDISHWND) nID = ::GetDlgCtrlID((HWND)nID);
	pTTT->hinst = AfxGetResourceHandle();
	pTTT->lpszText = MAKEINTRESOURCE(nID);
   return nID;
}

/////////////////////////////////////////////////////////////////////////////
// CTooltipsEx

CTooltipList::CTooltipList()
{
   m_pList = NULL;
   m_tCnt = -1;
   m_timOut = 100;                // affiche 10 secondes
   m_bkColor = RGB(255,255,230);
   m_textColor = RGB(0,0,0);

   m_font.CreatePointFont(86, "Tahoma");
}

CTooltipList::~CTooltipList()
{
}

BOOL CTooltipList::Create(CWnd *pParent, CListCtrl *pList)
{
   m_pParent = pParent;
   m_pList = pList;

   BOOL ret = CreateEx(0,
      AfxRegisterWndClass(0),
         "", ES_MULTILINE|WS_POPUP|ES_READONLY|WS_BORDER,
         0, 0, 0, 0, pList->m_hWnd, NULL);

   return ret;
}

BEGIN_MESSAGE_MAP(CTooltipList, CWnd)
	//{{AFX_MSG_MAP(CTooltipList)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
   ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CTooltipList message handlers

void CTooltipList::ListChange()
{
   if (m_hWnd) m_tCnt = -4, ShowWindow(SW_HIDE);
}

void CTooltipList::RelayEvent(MSG* pMsg)
{
   if ( m_pList && (pMsg->message == WM_MOUSEMOVE
     || pMsg->message == WM_NCMOUSEMOVE) ) {
      CPoint pos = pMsg->pt;
      CRect rect;

      // Position du tooltip
      m_curPos = CPoint(-2,18) + pos;

      // On passe en coordonnees liste
      m_pList->ScreenToClient(&pos);
      m_pList->GetClientRect(rect);
      // Teste l'interception d'un nouvel objet
      if ( m_tCnt < 0 && rect.PtInRect(pos) ) {
         int iItem = -1, iSubItem;

         // teste si le point est dans le header
         CHeaderCtrl *pHdr = m_pList->GetHeaderCtrl();
         for (iSubItem = pHdr->GetItemCount(); --iSubItem >= 0;) {
            pHdr->GetItemRect(iSubItem, &m_toolInfo.rect);
            if ( ::PtInRect(&m_toolInfo.rect, pos) ) break;
         }

         if (iSubItem < 0) {
            // teste si le point est sur une position valide
            LVHITTESTINFO hi;
            hi.pt = pos;
            hi.flags = 0;
            m_pList->SubItemHitTest( &hi );
            if (hi.flags & 0x0004) {
               // indique le rectangle de la case interceptee
               ListView_GetSubItemRect(m_pList->m_hWnd, hi.iItem, hi.iSubItem,
                                            LVIR_LABEL, &m_toolInfo.rect);
               iItem = hi.iItem;
               iSubItem = hi.iSubItem;
            }
         }

         if (iSubItem >= 0) {
            // Genere le message correspondant
            m_toolInfo.cbSize = 0;// pour ne pas interferer avec les autres usages
            m_toolInfo.hwnd = m_pList->m_hWnd;
            m_toolInfo.uFlags = 0;
            m_toolInfo.uId = m_pList->GetDlgCtrlID();
            m_toolInfo.hinst = AfxGetResourceHandle();
            m_toolInfo.lpszText = NULL;
            // Demande le texte au parent
            m_pParent->SendMessage( TTM_GETTEXT, (iItem << 16) | iSubItem,
                                            (LPARAM)&m_toolInfo );
            m_tCnt = - m_tCnt - 1;
            m_timer = SetTimer(1, 100, 0);
         }

      } else if (! ::PtInRect(&m_toolInfo.rect, pos)) {
         // La souris n'est plus sur l'objet
         ShowWindow(SW_HIDE);
         m_tCnt = -1;
      }
   }
}
/////////////////////////////////////////////////////////////////////////////
// CToolView message handlers

HBRUSH CTooltipList::CtlColor(CDC *pDC, UINT)
{
   // Permet de changer la couleur
   m_brush.DeleteObject();
   m_brush.CreateSolidBrush(m_bkColor);// couleur du fond
   pDC->SetBkColor(m_bkColor);         // couleur de fond du texte
   pDC->SetTextColor(m_textColor);     // couleur du texte
   return (HBRUSH)m_brush;
}

void CTooltipList::OnTimer(UINT nIDEvent) 
{
   CPoint pos;
   ::GetCursorPos(&pos);
   m_pList->ScreenToClient(&pos);

   if (m_tCnt < 0) {

      ::SetRectEmpty(&m_toolInfo.rect);
      KillTimer(m_timer);
   } else if ( !::PtInRect(&m_toolInfo.rect, pos)
        || m_toolInfo.lpszText == NULL || m_toolInfo.lpszText[0] == 0) {

      ShowWindow(SW_HIDE);
      m_tCnt = -1;
   } else if (m_tCnt > m_timOut) {

      ShowWindow(SW_HIDE);
      KillTimer(m_timer);
   } else if (++m_tCnt == 4 && m_toolInfo.lpszText) {

      // Commande la visualisation du tooltip
      CRect rdim(0,0,0,0);
      CClientDC dc(this);
      CFont *pFnt = dc.SelectObject(&m_font);
      dc.DrawText(m_toolInfo.lpszText, rdim, DT_CALCRECT);
      dc.SelectObject(pFnt);
      SetWindowPos(NULL, m_curPos.x, m_curPos.y,
               rdim.Width() + 6, rdim.Height() + 4, SWP_NOZORDER|SWP_SHOWWINDOW);
      m_pList->SetFocus();
   }

	CWnd::OnTimer(nIDEvent);
}

void CTooltipList::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
   CRect rect;
   GetClientRect(rect);
   dc.FillSolidRect(rect, m_bkColor);
   rect.left += 2;
   COLORREF tColor = dc.SetTextColor(m_textColor);
   CFont *pFnt = dc.SelectObject(&m_font);
   dc.DrawText(m_toolInfo.lpszText, rect, 0);
   dc.SelectObject(pFnt);
   dc.SetTextColor(tColor);
}
