// DirPickDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DirPickDlg.h"
#include <Shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirPickDlg dialog

CDirPickDlg::CDirPickDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirPickDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDirPickDlg)
	//}}AFX_DATA_INIT
}


void CDirPickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirPickDlg)
	DDX_Control(pDX, IDC_ED_SELECT, m_edSelect);
	DDX_Control(pDX, IDC_TREE_FILES, m_treeFiles);
	DDX_Control(pDX, IDC_COMBO_EXT, m_comboExt);
	//}}AFX_DATA_MAP
}

BOOL CDirPickDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

   SetFilter(m_filter);
   m_treeFiles.NewTree(m_curFilter, m_path);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDirPickDlg, CDialog)
	//{{AFX_MSG_MAP(CDirPickDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_EXT, OnSelchangeComboExt)
	ON_EN_CHANGE(IDC_ED_SELECT, OnChangeEdSelect)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILES, OnSelchangedTreeFiles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirPickDlg message handlers

void CDirPickDlg::OnSelchangeComboExt() 
{
   m_curFilter = m_filtStr[m_comboExt.GetCurSel()];
   m_treeFiles.NewTree(m_curFilter, m_path);
}

void CDirPickDlg::OnChangeEdSelect() 
{
   CString path;
   m_edSelect.GetWindowText(path);
   // pour eviter les recursions intempestives
   if (m_path.CompareNoCase(path) != 0) m_treeFiles.PathSelect(path, TRUE);
}

void CDirPickDlg::OnSelchangedTreeFiles(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
   CString path = m_treeFiles.GetItemPath(NULL);
   if (!path.IsEmpty()) {
      m_path = path;
      m_edSelect.SetWindowText(path);
   }
	*pResult = 0;
}

void CDirPickDlg::SetFilter(CString filter)
{
   filter.Replace('|', 0);
   LPTSTR pStr = filter.GetBuffer(0), pFil;
   m_filtStr.RemoveAll();
   m_comboExt.ResetContent();
   while (*pStr) {
      pFil = pStr + strlen(pStr) + 1;
      if (*pFil == 0) break;

      m_comboExt.AddString(pStr);
      m_filtStr.Add(pFil);
      pStr = pFil + strlen(pFil) + 1;
   }
   m_comboExt.SetCurSel(0);
   m_curFilter = m_filtStr[0];
}
