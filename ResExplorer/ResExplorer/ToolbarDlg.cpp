// DlgMakeToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "ResExplorer.h"
#include "ToolbarDlg.h"
#include "BitmapRW.h"
#include "TooltipEx.h"
#include "ResDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolbarDlg dialog

CToolbarDlg::CToolbarDlg(CWnd *pParent, CResDoc *pDoc)
	: CDialog(CToolbarDlg::IDD, pParent)
{
   m_pDoc = pDoc;
	//{{AFX_DATA_INIT(CToolbarDlg)
	m_bmpDim = _T("");
	m_width = 0;
	m_images = 0;
	//}}AFX_DATA_INIT
   m_init = FALSE;
   EnableToolTips();
}

BOOL CToolbarDlg::Create(CWnd *pParent)
{
   return CDialog::Create(CToolbarDlg::IDD, pParent);
}

void CToolbarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolbarDlg)
	DDX_Text(pDX, IDC_ED_BMP_DIM, m_bmpDim);
	DDX_Text(pDX, IDC_ED_WIDTH, m_width);
	DDX_Text(pDX, IDC_ED_IMAGES, m_images);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CToolbarDlg, CDialog)
	//{{AFX_MSG_MAP(CToolbarDlg)
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_ED_IMAGES, OnChangeEdImages)
	ON_EN_UPDATE(IDC_ED_WIDTH, OnUpdateEdWidth)
	//}}AFX_MSG_MAP
   ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
END_MESSAGE_MAP()

BOOL CToolbarDlg::OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult)
{
   *pResult = (LRESULT)ToolTipNotify(pNMH);
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CToolbarDlg message handlers

BOOL CToolbarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   m_init = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CToolbarDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
   BM_INFO bmi;
   if (GetImageInfo(m_pDoc->GetResLib(), m_pDoc->GetResName(), RT_BITMAP, &bmi)) {
      m_dim.cx = bmi.bmWidth, m_dim.cy = bmi.bmHeight;
      if (m_init && bShow) {
         // premier affichage d'un nouveau toolbar
         m_bmpDim.Format("%d x %d", m_dim.cx, m_dim.cy);
         m_width = m_pDoc->m_toolbar.x;
         if (m_width <= 0) m_width = m_dim.cy;
         m_imagesOld = m_images = m_dim.cx / m_width;
         m_pDoc->m_toolbar.x = m_width;
         m_pDoc->UpdateAllViews(NULL, RES_VIEW_ITEM);
         UpdateData(FALSE);
      }
   }
	CDialog::OnShowWindow(bShow, nStatus);
}

void CToolbarDlg::OnCancel() 
{
//   m_pDoc->m_toolbar.x = 0;
	m_pDoc->m_toolbar.cnt= 0;	
   m_pDoc->UpdateAllViews(NULL, RES_VIEW_ITEM);
	
	CDialog::OnCancel();
}

void CToolbarDlg::OnChangeEdImages() 
{
   if (m_init && m_dim.cx > 0) {
      UpdateData();
      if (m_images <= 0) m_images = 1;
	   m_width = m_dim.cx / m_images;
      int inc = (m_images > m_imagesOld)? 1: -1;
      while (m_width * m_images < m_dim.cx) {
         m_width = m_dim.cx / (m_images += inc);
         if (m_width <= 4 || m_images <= 0) m_images = 1, m_width = m_dim.cx;
      }
      m_imagesOld = m_images;
	   m_pDoc->m_toolbar.x = m_width;
      m_pDoc->UpdateAllViews(NULL, RES_VIEW_ITEM);
      UpdateData(FALSE);
   }
}

void CToolbarDlg::OnUpdateEdWidth() 
{
   if (m_init && m_dim.cx > 0) {
      UpdateData();
      if (m_width <= 0) m_width = 1;
      m_imagesOld = m_images = m_dim.cx / m_width;
	   m_pDoc->m_toolbar.x = m_width;
      m_pDoc->UpdateAllViews(NULL, RES_VIEW_ITEM);
      UpdateData(FALSE);
   }
}

void CToolbarDlg::OnOK() 
{
   UpdateData();
	m_pDoc->m_toolbar.x  = m_width;
	m_pDoc->m_toolbar.y  = (short)m_dim.cy;
	m_pDoc->m_toolbar.cnt= m_images;	
   m_pDoc->UpdateAllViews(NULL, RES_VIEW_ITEM);

	CDialog::OnOK();
}
