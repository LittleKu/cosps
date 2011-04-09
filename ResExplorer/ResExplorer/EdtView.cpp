// EdtView.cpp : implementation file
//

#include "stdafx.h"
#include "resexplorer.h"
#include "ResDoc.h"
#include "BitmapRW.h"
#include "EdtView.h"
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h >       // MFC OLE automation classes
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CEdtView

IMPLEMENT_DYNCREATE(CEdtView, CEditView)

CEdtView::CEdtView()
{
   m_font.CreatePointFont(86, "Tahoma");
   m_nFlags = 0xFFFF;
   m_sel = 0;
}

CEdtView::~CEdtView()
{
}

BOOL CEdtView::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CEditView::PreCreateWindow(cs);
}

BEGIN_MESSAGE_MAP(CEdtView, CEditView)
	//{{AFX_MSG_MAP(CEdtView)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CEdtView drawing

///////////////////////////////////////////////////////////////////////////////
// CEdtView diagnostics

#ifdef _DEBUG
void CEdtView::AssertValid() const
{
	CEditView::AssertValid();
}

void CEdtView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CResDoc* CEdtView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CResDoc)));
	return (CResDoc*)m_pDocument;
}
#endif //_DEBUG
///////////////////////////////////////////////////////////////////////////////
// CEdtView message handlers

void CEdtView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
   SetWindowContextHelpId(0x20000+IDR_TEXTVIEW);

   if (pHint) m_pParam = (CParams*)pHint;

   if ( m_pParam && (lHint & RES_VIEW) && m_pParam->hLib ) {
      DWORD rSize;
      PVOID pVoid = RawData(m_pParam->hLib, m_pParam->resName, m_pParam->resType, &rSize);
      HLOCAL hLoc = ::LocalAlloc(LMEM_MOVEABLE|LMEM_ZEROINIT, rSize + 1);
      memcpy(::LocalLock(hLoc), pVoid, rSize );
      ::LocalUnlock(hLoc);
      GetEditCtrl().SetHandle(hLoc);
   }
}

int CEdtView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetFont(&m_font);
   GetEditCtrl().SetReadOnly();

	return 0;
}

HBRUSH CEdtView::CtlColor(CDC* pDC, UINT nCtlColor) 
{
   // return a different handle to customize the color of controls
   if ( nCtlColor != CTLCOLOR_STATIC ) return NULL;
   else return (HBRUSH)::GetStockObject(WHITE_BRUSH);
}

void CEdtView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
   CMenu menu; // Load float edit menu
   menu.LoadMenu( IDR_EVCOPYPAST );
   CMenu *pSbm = menu.GetSubMenu(0);

   if (GetEditCtrl().GetSel() != 0)
      pSbm->EnableMenuItem(ID_EDIT_COPY, MF_BYCOMMAND|MF_ENABLED);

   // Display a menu at the cursor position
   pSbm->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x,point.y, this);
}

void CEdtView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_nFlags = nFlags;
   m_sel = GetEditCtrl().GetSel();
	CEditView::OnRButtonDown(nFlags, point);
}

void CEdtView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_nFlags == nFlags) Begindrag();
   m_nFlags = 0xFFFF;
	CEditView::OnMouseMove(nFlags, point);
}

void CEdtView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_nFlags = nFlags;
   m_sel = GetEditCtrl().GetSel();
	CEditView::OnLButtonDown(nFlags, point);
}
///////////////////////////////////////////////////////////////////////////////
// Subroutine

void CEdtView::Begindrag()
{
   // It is used either during Drag/Drop operation

   CString text;
   GetSelectedText(text);
   if ( ! text.IsEmpty() ) {

      CSharedFile	sf( GMEM_MOVEABLE|GMEM_DDESHARE );
      sf.Write( text, text.GetLength() + 1 );

      COleDataSource* pSource = new COleDataSource();
      if (pSource) {
         pSource->CacheGlobalData( CF_TEXT, sf.Detach() );
         pSource->DoDragDrop(DROPEFFECT_COPY);
         delete pSource;
      }
   }
}
