#include "stdafx.h"
#include "CoolTabDemo.h"
#include "CCommonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CCommonDlg, CDialog)

CCommonDlg::CCommonDlg() : CResizableDialog()
{
}

CCommonDlg::CCommonDlg(UINT nIDTemplate, CWnd* pParentWnd/* = NULL*/)
: CResizableDialog(nIDTemplate, pParentWnd)
{
}

CCommonDlg::~CCommonDlg()
{
}


BEGIN_MESSAGE_MAP(CCommonDlg, CResizableDialog)
END_MESSAGE_MAP()



