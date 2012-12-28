// VCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SkeletonDemo.h"
#include "VCDlg.h"
#include "cflmfc/FileDialogEx.h"
#include "AdvComboBox.h"
#include "SysUtils.h"
#include "Converter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DECLARE_THE_LOGGER_NAME(_T("CVCDlg"))

#define PF_ATTRIB_ID	"id"
#define PF_ATTRIB_NAME	"name"
#define PF_ATTRIB_ICON	"icon"
#define PF_ATTRIB_DESC	"desc"
#define PF_ATTRIB_FILE	"file"

/////////////////////////////////////////////////////////////////////////////
// CVCDlg dialog

CVCDlg::CVCDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CVCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVCDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMinWidth = -1;
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
	DDX_Control(pDX, IDC_TASK_LISTCTRL, m_taskListCtrl);
	DDX_Control(pDX, IDC_TASK_TREE, m_taskTreeCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVCDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CVCDlg)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCDlg message handlers
int CVCDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CResizableDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcClient;
	GetClientRect(&rcClient);

	//save the min width
	m_nMinWidth = rcClient.Width();

	return 0;
}

BOOL CVCDlg::OnEraseBkgnd(CDC* pDC) 
{
	CRect rcClient;
	GetClientRect(&rcClient);
	
	pDC->FillSolidRect(&rcClient, SYS_APP()->m_crSysBkColor);
	
	return TRUE;
}

HBRUSH CVCDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CResizableDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_STATIC)
    {
		pDC->SetBkMode(TRANSPARENT);
		
		return (HBRUSH)(SYS_APP()->m_pSysBkBrush->GetSafeHandle());
    }
	return hbr;
}

BOOL CVCDlg::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	//Initialize the profile tree firstly
	CString szPath;
	if(SysUtils::GetProfile(szPath, _T("profiles.xml")))
	{
		m_profileLoader.LoadProfileTree(CFL_T2A((LPCTSTR)szPath));
	}

	InitTaskTree();
	InitTaskListCtrl();
	InitPropList();
	InitDeviceComboBox();
	InitProfileComboBox();
	InitSplitters();

	//ResizableDialog Init
	InitResizableDlgAnchor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
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

BOOL CVCDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	BOOL bProcessed = TRUE;

	DWORD nCtrlID = LOWORD(wParam);
	DWORD nNotifyID = HIWORD(wParam);

	switch (nCtrlID)
	{
	case IDC_COMBOBOX_DEVICE:
		{
			if(nNotifyID == CBN_SELCHANGE)
			{
				OnCategorySelChanged();
			}
		}
		break;
	case IDC_COMBOBOX_PROFILE:
		{
			if(nNotifyID == CBN_SELCHANGE)
			{
				OnProfileSelChanged();
			}
		}
		break;
	default:
		{
			bProcessed = FALSE;
		}
		break;
	}
	
	if(bProcessed)
	{
		return TRUE;
	}
	
	return CResizableDialog::OnCommand(wParam, lParam);
}

void CVCDlg::OnSize(UINT nType, int cx, int cy) 
{
	CResizableDialog::OnSize(nType, cx, cy);
	
	SetSplitterTreeTableRange();
	SetSplitterTableOutputRange();
}

void CVCDlg::InitTaskTree()
{

}

void CVCDlg::InitTaskListCtrl()
{
	m_taskListCtrl.Init();
}

void CVCDlg::InitPropList()
{
	CRect rcPropList;
	CWnd* pWnd = GetDlgItem(IDC_OUTPUT_PROP_LIST);
	ASSERT(pWnd != NULL);
	pWnd->GetWindowRect(&rcPropList);
	ScreenToClient(&rcPropList);
	
	pWnd->DestroyWindow();
	
	if(!m_propListMgr.CreatePropList(rcPropList, this, IDC_OUTPUT_PROP_LIST))
	{
		return;
	}
}

void CVCDlg::InitDeviceComboBox()
{
	ProfileNode* pRoot = m_profileLoader.GetRootProfile();
	int nChildCount = ((pRoot == NULL) ? 0 : pRoot->GetChildCount());

	std::string szText;
	for(int i = 0; i < nChildCount; i++)
	{
		ProfileNode* pProfile = pRoot->GetChildAt(i);

		if(!pProfile->attribMap.Get(PF_ATTRIB_DESC, szText))
		{
			pProfile->attribMap.Get(PF_ATTRIB_NAME, szText);
		}
		LIST_ITEM item;
		item.strText = CFL_A2T(szText.c_str());
		item.vpItemData = pProfile;
		m_deviceComboBox.AddItem(&item);
	}

	if(nChildCount > 0)
	{
		m_deviceComboBox.SetCurSel(0);
		OnCategorySelChanged();
	}
}
void CVCDlg::InitProfileComboBox()
{
}

void CVCDlg::OnCategorySelChanged()
{
	m_profileComboBox.DeleteAllItems();
	int nCurSel = m_deviceComboBox.GetCurSel();
	if(nCurSel >= 0)
	{
		ProfileNode* pParent = (ProfileNode*)m_deviceComboBox.GetItemDataPtr(nCurSel);
		
		int nChildCount = ((pParent == NULL) ? 0 : pParent->GetChildCount());
		for(int i = 0; i < nChildCount; i++)
		{
			ProfileNode* pProfile = pParent->GetChildAt(i);
			AddProfile(pProfile, NULL);
		}
		
		if(nChildCount > 0)
		{
			m_profileComboBox.SetCurSel(0);
			OnProfileSelChanged();
		}
	}
}

void CVCDlg::OnProfileSelChanged()
{
	int nCurSel = m_profileComboBox.GetCurSel();
	if(nCurSel >= 0)
	{
		ProfileNode* pProfile = (ProfileNode*)m_profileComboBox.GetItemDataPtr(nCurSel);
		std::string szProfileFile;
		if(pProfile != NULL && pProfile->attribMap.Get(PF_ATTRIB_FILE, szProfileFile))
		{
			//Initialize the profile tree firstly
			CString szPath;
			if(SysUtils::GetProfile(szPath, CFL_A2T(szProfileFile.c_str())))
			{
				m_propListMgr.Init(CFL_T2A((LPCTSTR)szPath));
			}
		}
	}
}

PLIST_ITEM CVCDlg::AddProfile(ProfileNode* pProfile, PLIST_ITEM pItemParent)
{
	if(pProfile == NULL)
	{
		return NULL;
	}
	std::string szText;
	if(!pProfile->attribMap.Get(PF_ATTRIB_DESC, szText))
	{
		cfl::tstring szLog;
		cfl::tformat(szLog, _T("Can't find the [desc] attribute"));
		LOG4CPLUS_INFO_STR(THE_LOGGER, szLog)
		return NULL;
	}
	LIST_ITEM item;
	item.strText = CFL_A2T(szText.c_str());
	item.vpItemData = pProfile;
	PLIST_ITEM pCurrItem = m_profileComboBox.AddItem(&item, pItemParent);

	int nChildCount = ((pProfile == NULL) ? 0 : pProfile->GetChildCount());
	for(int i = 0; i < nChildCount; i++)
	{
		AddProfile(pProfile->GetChildAt(i), pCurrItem);
	}

	return pCurrItem;
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
    AddAnchor(IDC_TASK_LISTCTRL, TOP_LEFT, BOTTOM_RIGHT);

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
	
	int nMin = rcTree.left;
	int nMax = -1;
	if(m_nMinWidth > 0)
	{
		nMax = nMin + (m_nMinWidth * 2) / 5 - 10;
	}
	if(nMax <= nMin)
	{
		nMax = nMin + 320;
	}
	m_splitterTreeTable.SetRange(nMin, nMax);
}

void CVCDlg::SetSplitterTableOutputRange()
{
	if(!::IsWindow(m_splitterTableOutput.GetSafeHwnd()))
	{
		return;
	}

	CRect rcDlg;
	GetClientRect(&rcDlg);

	int nMin = -1;
	if(m_nMinWidth > 0)
	{
		nMin = rcDlg.right - (m_nMinWidth * 3) / 5 + 10;
	}
	if(nMin <= 0)
	{
		nMin = rcDlg.right - 500;
	}
	int nMax = rcDlg.right - 120;	
	
	m_splitterTableOutput.SetRange(nMin, nMax);
}

void CVCDlg::DoSizeTreeTable(int delta)
{
	//width change
	cfl::CSplitterControl::ChangeWidth(GetDlgItem(IDC_TASK_TREE), delta, CW_LEFTALIGN);
	cfl::CSplitterControl::ChangeWidth(GetDlgItem(IDC_TASK_LISTCTRL), -delta, CW_RIGHTALIGN);
	
	RefreshAllAnchors();

	Invalidate();
	UpdateWindow();

	SetSplitterTreeTableRange();
	SetSplitterTableOutputRange();
}
void CVCDlg::DoSizeTableOutput(int delta)
{
	//width change
	cfl::CSplitterControl::ChangeWidth(GetDlgItem(IDC_TASK_LISTCTRL), delta, CW_LEFTALIGN);

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

void CVCDlg::UpdateTaskTreeWindow()
{
	CRect rect;
	m_taskTreeCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	m_taskTreeCtrl.Invalidate();
	m_taskTreeCtrl.UpdateWindow();
	
	InvalidateRect(&rect);
	UpdateWindow();
}

void CVCDlg::AddFiles()
{
	static const TCHAR *FILTERS =
		_T("Windows Media Files (*.wmv;*.avi;*.asf;*.dvr-ms;*.ms-dvr)|*.wmv; *.avi; *.asf; *.dvr-ms; *.ms-dvr|")
        _T("MPEG4 Files (*.mp4;*.m4v;*.mpeg4)|*.mp4; *.m4v; *.mpeg4|")
		_T("MPEG2 Video Files (*.m2v;*.mpg;*.vob;*.mpeg;*.mpeg2;*.m2p;*.mod)|*.m2v; *.mpg; *.vob; *.mpeg; *.mpeg2; *.m2p; *.mod|")
        _T("MPEG1 Video Files (*.m1v;*.mpg;*.mpv,*.mpeg;*.mpeg1;*.dat)|*.m1v; *.mpg; *.mpv, *.mpeg; *.mpeg1; *.dat|")
		_T("Real Media Files (*.rm;*.rmvb;*.rv)|*.rm; *.rmvb; *.rv|")
		_T("Flash Media Files (*.flv;*.f4v)|*.flv; *.f4v|")
		_T("All Files (*.*)|*.*|")
		_T("|");
	cfl::CFileDialogEx dlg(TRUE, _T(""), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT, FILTERS);
	
	int nFileBufferLen = (1 << 15);
	TCHAR* lpFileBuffer = new TCHAR[nFileBufferLen];
	memset(lpFileBuffer, 0, sizeof(TCHAR) * nFileBufferLen);
	dlg.m_ofn.lpstrFile = lpFileBuffer;
	dlg.m_ofn.nMaxFile = nFileBufferLen;

	int nResult = dlg.DoModal();
	if(nResult == IDOK)
	{
		CString szFileName;

		POSITION pos = dlg.GetStartPosition();
		while(pos != NULL)
		{
			szFileName = dlg.GetNextPathName(pos);
			if(!szFileName.IsEmpty())
			{
				m_taskListCtrl.AddTask(szFileName);
			}
		}
	}

	delete [] lpFileBuffer;
	lpFileBuffer = NULL;
}

void CVCDlg::OnStartButton()
{
	CDefaultConverter cvter;
	
	POSITION pos = m_taskListCtrl.GetFirstSelectedItemPosition();
	while(pos != NULL)
	{
		int nItem = m_taskListCtrl.GetNextSelectedItem(pos);
		CTaskInfo* pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(nItem);
		ASSERT(pTaskInfo);
		cvter.Convert(pTaskInfo->m_szFileName);
	}
}
