// VCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SkeletonDemo.h"
#include "VCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVCDlg dialog


CVCDlg::CVCDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CVCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVCDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVCDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVCDlg)
	DDX_Control(pDX, IDC_COMBOBOX_DEVICE, m_deviceComboBox);
	DDX_Control(pDX, IDC_COMBOBOX_PROFILE, m_profileComboBox);
	DDX_Control(pDX, IDC_OUTPUT_PROP_LIST, m_wndOutputPropListPos);
	DDX_Control(pDX, IDC_SPLITTER_TABLE_OUTPUT, m_splitterTableOutput);
	DDX_Control(pDX, IDC_SPLITTER_TREE_TABLE, m_splitterTreeTable);
	DDX_Control(pDX, IDC_TASK_LIST_CTRL, m_taskListCtrl);
	DDX_Control(pDX, IDC_TASK_TREE, m_taskTreeCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVCDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CVCDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCDlg message handlers

BOOL CVCDlg::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	InitTaskTree();
	InitTaskListCtrl();
	InitSplitters();

	//ResizableDialog Init
	InitResizableDlgAnchor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVCDlg::InitTaskTree()
{

}

void CVCDlg::InitTaskListCtrl()
{

}

void CVCDlg::InitSplitters()
{
	m_splitterVBkBitmap.LoadBitmap(IDB_SPLITTER_V_BKG);
	m_splitterHBkBitmap.LoadBitmap(IDB_SPLITTER_H_BKG);
	CRect rc;
	CWnd* pWnd;
	
	//create
	pWnd = GetDlgItem(IDC_SPLITTER_TREE_TABLE);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	pWnd->DestroyWindow();
	m_splitterTreeTable.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER_TREE_TABLE);
	m_splitterTreeTable.m_pBkBitmap = &m_splitterVBkBitmap;
	//set range
	SetSplitterTreeTableRange();
	
	//create
	pWnd = GetDlgItem(IDC_SPLITTER_TABLE_OUTPUT);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	pWnd->DestroyWindow();
	m_splitterTableOutput.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER_TABLE_OUTPUT);
	m_splitterTableOutput.m_pBkBitmap = &m_splitterVBkBitmap;

	//set range
	SetSplitterTableOutputRange();
}

void CVCDlg::InitResizableDlgAnchor()
{	
	AddAnchor(IDC_TASK_TREE, TOP_LEFT, BOTTOM_LEFT);
    AddAnchor(IDC_TASK_LIST_CTRL, TOP_LEFT, BOTTOM_RIGHT);

	AddAnchor(IDC_SPLITTER_TREE_TABLE, TOP_LEFT, BOTTOM_LEFT);
	AddAnchor(IDC_SPLITTER_TABLE_OUTPUT, TOP_RIGHT, BOTTOM_RIGHT);

	AddAnchor(IDC_OUTPUT_GROUPBOX, TOP_RIGHT, TOP_RIGHT);
	AddAnchor(IDC_LABEL_DEVICE, TOP_RIGHT, TOP_RIGHT);
	AddAnchor(IDC_LABEL_PROFILE, TOP_RIGHT, TOP_RIGHT);
	AddAnchor(IDC_COMBOBOX_DEVICE, TOP_RIGHT, TOP_RIGHT);
	AddAnchor(IDC_COMBOBOX_PROFILE, TOP_RIGHT, TOP_RIGHT);

	AddAnchor(IDC_OUTPUT_PROP_LIST, TOP_RIGHT, BOTTOM_RIGHT);
	
	SetSizeGripVisibility(FALSE);
	UpdateSizeGrip();
}

void CVCDlg::SetSplitterTreeTableRange()
{	
	if(!::IsWindow(m_splitterTreeTable.GetSafeHwnd()))
	{
		return;
	}

	CRect rcTree;
	m_taskTreeCtrl.GetWindowRect(&rcTree);
	ScreenToClient(&rcTree);
	
	m_splitterTreeTable.SetRange(rcTree.left, rcTree.left + 320);	
}

void CVCDlg::SetSplitterTableOutputRange()
{
	if(!::IsWindow(m_splitterTableOutput.GetSafeHwnd()))
	{
		return;
	}

	CRect rcDlg;
	GetClientRect(&rcDlg);

	int nMin = rcDlg.right - 500;
	int nMax = rcDlg.right - 120;
	m_splitterTableOutput.SetRange(nMin, nMax);
}

void CVCDlg::DoSizeTreeTable(int delta)
{
	//width change
	cfl::CSplitterControl::ChangeWidth(GetDlgItem(IDC_TASK_TREE), delta, CW_LEFTALIGN);
	cfl::CSplitterControl::ChangeWidth(GetDlgItem(IDC_TASK_LIST_CTRL), -delta, CW_RIGHTALIGN);
	
	RefreshAllAnchors();

	Invalidate();
	UpdateWindow();

	SetSplitterTreeTableRange();
	SetSplitterTableOutputRange();
}
void CVCDlg::DoSizeTableOutput(int delta)
{
	//width change
	cfl::CSplitterControl::ChangeWidth(GetDlgItem(IDC_TASK_LIST_CTRL), delta, CW_LEFTALIGN);

	cfl::CSplitterControl::ChangeWidth(GetDlgItem(IDC_OUTPUT_GROUPBOX), -delta, CW_RIGHTALIGN);
	cfl::CSplitterControl::ChangeWidth(GetDlgItem(IDC_COMBOBOX_DEVICE), -delta, CW_RIGHTALIGN);
	cfl::CSplitterControl::ChangeWidth(GetDlgItem(IDC_COMBOBOX_PROFILE), -delta, CW_RIGHTALIGN);
	cfl::CSplitterControl::ChangeWidth(GetDlgItem(IDC_OUTPUT_PROP_LIST), -delta, CW_RIGHTALIGN);

	//position change
	cfl::CSplitterControl::ChangePos(GetDlgItem(IDC_LABEL_DEVICE), -delta, 0);
	cfl::CSplitterControl::ChangePos(GetDlgItem(IDC_LABEL_PROFILE), -delta, 0);
	
	RefreshAllAnchors();

	Invalidate();
	UpdateWindow();

	SetSplitterTreeTableRange();
	SetSplitterTableOutputRange();
}

LRESULT CVCDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_NOTIFY)
	{
		if (wParam == IDC_SPLITTER_TREE_TABLE)
		{	
			cfl::SPC_NMHDR* pHdr = (cfl::SPC_NMHDR*) lParam;
			DoSizeTreeTable(pHdr->delta);
		}
		else if (wParam == IDC_SPLITTER_TABLE_OUTPUT)
		{
			cfl::SPC_NMHDR* pHdr = (cfl::SPC_NMHDR*) lParam;
			DoSizeTableOutput(pHdr->delta);
		}
	}
	
	return CResizableDialog::DefWindowProc(message, wParam, lParam);
}

void CVCDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizableDialog::OnSize(nType, cx, cy);
	
	SetSplitterTreeTableRange();
	SetSplitterTableOutputRange();
}
