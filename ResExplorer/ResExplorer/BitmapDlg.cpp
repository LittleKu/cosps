// BitmapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resexplorer.h"
#include "TooltipEx.h"
#include "BitmapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
// CBitmapDlg dialog

CBitmapDlg::CBitmapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBitmapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBitmapDlg)
	m_bkColor = -1;
	//}}AFX_DATA_INIT
   EnableToolTips();
}

void CBitmapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBitmapDlg)
	DDX_Radio(pDX, IDC_RD_BKCOLOR_1, m_bkColor);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBitmapDlg, CDialog)
	//{{AFX_MSG_MAP(CBitmapDlg)
	//}}AFX_MSG_MAP
   ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
END_MESSAGE_MAP()

BOOL CBitmapDlg::OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult)
{
   *pResult = (LRESULT)ToolTipNotify(pNMH);
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
// CBitmapDlg message handlers

void CBitmapDlg::OnOK() 
{
   UpdateData(TRUE);
	CDialog::OnOK();
}
