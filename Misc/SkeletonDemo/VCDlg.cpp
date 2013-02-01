// VCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SkeletonDemo.h"
#include "VCDlg.h"
#include "cflmfc/FileDialogEx.h"
#include "AdvComboBox.h"
#include "SysUtils.h"
#include "Converter.h"
#include "cflbase/FileUtils.h"
#include <Shlwapi.h>
#include "Preferences.h"
#include "StdStreamParser.h"
#include "MainDlg.h"
#include "MPlayerMetaProbe.h"

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
	m_pProcExecutor = new ProcessExecutor();

	m_cmdInfos.clear();
	m_nCurCmd = -1;
	m_delList.clear();

	m_nCurRow = -1;
	m_selRows.clear();

	m_nProgressBase = 0;

	m_nCurTaskState = TSE_READY;

	m_lpFileBuffer = NULL;
	m_nFileBufferLen = (1 << 15);
}

CVCDlg::~CVCDlg()
{
	if(m_pProcExecutor)
	{
		delete m_pProcExecutor;
		m_pProcExecutor = NULL;
	}

	if(m_lpFileBuffer)
	{
		delete [] m_lpFileBuffer;
		m_lpFileBuffer = NULL;
	}
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
	ON_WM_DESTROY()

	ON_MESSAGE(RSP_START_PROCESS, OnRspStart)
	ON_MESSAGE(NOTIFY_PROCESS_END, OnNotifyProcEnd)
	ON_MESSAGE(NOTIFY_PROCESS_PROGRESS, OnNotifyProcProgress)
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


void CVCDlg::OnDestroy() 
{
	m_pProcExecutor->Destroy();
	
	CDialog::OnDestroy();	
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

	InitTaskTree();
	InitTaskListCtrl();
	InitPropList();
	InitDeviceComboBox();
	InitProfileComboBox();
	InitSplitters();

	//ResizableDialog Init
	InitResizableDlgAnchor();

	m_pProcExecutor->SetResponseWnd(GetSafeHwnd());
	
	if(m_pProcExecutor->Create())
	{
		EndDialog(-1);
		TRACE0("Warning: Message queue create failed during dialog init.\n");
		return FALSE;
	}
	
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
	ProfileNode* pRoot = ProfileMgr::GetInstance()->GetRootProfile();
	int nChildCount = ((pRoot == NULL) ? 0 : pRoot->GetChildCount());

	std::string szText;
	AttribMap* pAttribMap = NULL;
	for(int i = 0; i < nChildCount; i++)
	{
		ProfileNode* pProfile = (ProfileNode*)pRoot->GetChild(i);

		pAttribMap = (AttribMap*)pProfile->GetData();
		if(!pAttribMap->Get(PF_ATTRIB_DESC, szText))
		{
			pAttribMap->Get(PF_ATTRIB_NAME, szText);
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
			ProfileNode* pProfile = (ProfileNode*)pParent->GetChild(i);
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
		AttribMap* pAttribMap = NULL;
		std::string szProfileFile;
		if(pProfile != NULL)
		{
			pAttribMap = (AttribMap*)pProfile->GetData();
			if(pAttribMap->Get(PF_ATTRIB_FILE, szProfileFile))
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
}

ProfileNode* CVCDlg::GetSelectedProfile()
{
	int nCurSel = m_profileComboBox.GetCurSel();
	if(nCurSel >= 0)
	{
		ProfileNode* pProfile = (ProfileNode*)m_profileComboBox.GetItemDataPtr(nCurSel);
		return pProfile;
	}
	return NULL;
}

PLIST_ITEM CVCDlg::AddProfile(ProfileNode* pProfile, PLIST_ITEM pItemParent)
{
	if(pProfile == NULL)
	{
		return NULL;
	}
	std::string szText;
	AttribMap* pAttribMap = (AttribMap*)pProfile->GetData();
	if(!pAttribMap->Get(PF_ATTRIB_TAG, szText) || (szText.compare("device") != 0 && szText.compare("profile") != 0))
	{
		return NULL;
	}

	if(!pAttribMap->Get(PF_ATTRIB_DESC, szText))
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
		AddProfile((ProfileNode*)pProfile->GetChild(i), pCurrItem);
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
	cfl::CFileDialogEx dlg(TRUE, _T(""), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT, 
		SysUtils::lpszInputFileFilter);
	
	if(m_lpFileBuffer == NULL)
	{
		m_nFileBufferLen = (1 << 15);
		m_lpFileBuffer = new TCHAR[m_nFileBufferLen];
	}
	ASSERT(m_lpFileBuffer != NULL);
	memset(m_lpFileBuffer, 0, sizeof(TCHAR) * m_nFileBufferLen);
	dlg.m_ofn.lpstrFile = m_lpFileBuffer;
	dlg.m_ofn.nMaxFile = m_nFileBufferLen;

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
}

void CVCDlg::GetStreamDumpFile(cfl::tstring& szDumpFile, const cfl::tstring& szBinFQPath, LPCTSTR lpszStreamName, int nIndex)
{
	cfl::FileUtils::GetFileNameOnly(szDumpFile, szBinFQPath);
	cfl::tformat(szDumpFile, _T("%s\\%d_%s_%s.dump"), (LPCTSTR)(SYS_PREF()->szTempFolder), 
		nIndex, lpszStreamName, szDumpFile.c_str());
}

void CVCDlg::OnStartButton()
{
	m_nCurRow = 0;
	m_selRows.clear();

	int nItem;
	POSITION pos = m_taskListCtrl.GetFirstSelectedItemPosition();
	while(pos != NULL)
	{
		nItem = m_taskListCtrl.GetNextSelectedItem(pos);
		m_selRows.push_back(nItem);

		CTaskInfo taskInfo;
		taskInfo.mask = TIF_STATUS | TIF_PROGRESS;
		taskInfo.m_nState = TSE_READY;
		taskInfo.m_dProgress = 0;
		m_taskListCtrl.UpdateRow(nItem, &taskInfo);
	}

	if(m_selRows.empty())
	{
		return;
	}
	ChangeState(TSE_RUNNING);
	//start the first task
	StartTask();
}

void CVCDlg::OnStopButton()
{
	if(m_nCurTaskState == TSE_RUNNING)
	{
		ChangeState(TSE_STOPPED);
		m_pProcExecutor->PostMsg(REQ_STOP_PROCESS);
	}
}

LRESULT CVCDlg::OnRspStart(WPARAM wParam, LPARAM lParam)
{
	bool bOk = (lParam == 0);
	if(!bOk)
	{
		ChangeState(TSE_ERROR);
		UpdateStatus(m_selRows.at(m_nCurRow), m_nCurTaskState);
	}
	//TODO: if start failed, stop to continue;
	/*
	CTaskInfo* pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(m_selRows.at(m_nCurRow));
	if(pTaskInfo == NULL)
	{
		return 0L;
	}

	cfl::tstring szBinName;
	if(m_nCurCmd < m_cmdInfos.size())
	{
		cfl::FileUtils::GetFileNameOnly(szBinName, m_cmdInfos.at(m_nCurCmd).m_szBinFQPath);
		pTaskInfo->m_szInfo.Format(_T("%s - %s"), szBinName.c_str(), (bOk ? _T("OK") : _T("Failed")));
	}
	else
	{
		pTaskInfo->m_szInfo.Format(_T("CmdInfo unknown - %s"), (bOk ? _T("OK") : _T("Failed")));
	}
	m_taskListCtrl.UpdateRow(m_selRows.at(m_nCurRow));
	*/
	return 0L;
}

void CVCDlg::StartProcess(const CmdInfo& cmdInfo, int nIndex)
{
	ExecArgument* pExecArg = new ExecArgument();
	pExecArg->szCmdLine = cmdInfo.m_szCmdLine;
	GetStreamDumpFile(pExecArg->szOutDumpFile, cmdInfo.m_szBinFQPath, _T("out"), nIndex);
	GetStreamDumpFile(pExecArg->szErrDumpFile, cmdInfo.m_szBinFQPath, _T("err"), nIndex);

	cfl::tstring szBinName;
	cfl::FileUtils::GetFileNameOnly(szBinName, cmdInfo.m_szBinFQPath);

	//TODO: parser depends on command line
	pExecArg->pOutParser = new SaveFileParser(pExecArg->szOutDumpFile.c_str());
	pExecArg->pErrParser = new SaveFileParser(pExecArg->szErrDumpFile.c_str());
	if(szBinName.compare(_T("mencoder")) == 0)
	{
		pExecArg->pOutParser = new MEncoderOutParser(GetSafeHwnd(), pExecArg->pOutParser);
	}

	m_pProcExecutor->PostMsg(REQ_START_PROCESS, 0, (LPARAM)pExecArg);
}

void CVCDlg::StartTask()
{
	//The validation check is done by the caller
	ASSERT(m_nCurRow >= 0 && m_nCurRow < m_selRows.size());

	//generate command line list for the current row
	CDefaultConverter cvter;

	CTaskInfo* pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(m_selRows.at(m_nCurRow));
	ASSERT(pTaskInfo);
	
	m_nProgressBase = 0;
	m_cmdInfos.clear();
	m_nCurCmd = -1;
	m_delList.clear();
	
	int nResult = cvter.Convert(pTaskInfo->m_szFileName);
	if(nResult != 0)
	{
		ChangeState(TSE_ERROR);

		CString szErrorText;
		szErrorText.Format(_T("Failed to start converting. result=%d"), nResult);
		AfxMessageBox(szErrorText);

		return;
	}
	
	//Execute now
	m_nCurCmd = 0;
	if(m_nCurCmd < m_cmdInfos.size())
	{
		UpdateStatus(m_selRows.at(m_nCurRow), TSE_RUNNING);
		StartProcess(m_cmdInfos.at(m_nCurCmd), m_nCurCmd);
	}
}

LRESULT CVCDlg::OnNotifyProcEnd(WPARAM wParam, LPARAM lParam)
{
	if(m_nCurTaskState != TSE_RUNNING)
	{
		//update status
		UpdateStatus(m_selRows.at(m_nCurRow), m_nCurTaskState);

		//clean up task list and temp files
		for(int i = 0; i < m_delList.size(); i++)
		{
			if(::PathFileExists(m_delList.at(i).c_str()))
			{
				::DeleteFile(m_delList.at(i).c_str());
			}
		}
		return 0L;
	}
	//The current process terminated, execute the next one
	m_nProgressBase += m_cmdInfos.at(m_nCurCmd).m_nWeight;
	m_nCurCmd++;
	if(m_nCurCmd < m_cmdInfos.size())
	{
		UpdateProgress(m_selRows.at(m_nCurRow), (double)m_nProgressBase / 100);

		//start next command
		StartProcess(m_cmdInfos.at(m_nCurCmd), m_nCurCmd);
	}
	else
	{
		//The current task done.
		CTaskInfo taskInfo;
		taskInfo.mask = TIF_STATUS | TIF_PROGRESS;
		taskInfo.m_nState = TSE_DONE;
		taskInfo.m_dProgress = 1.0f;
		m_taskListCtrl.UpdateRow(m_selRows.at(m_nCurRow), &taskInfo);

		//delete temp files
		for(int i = 0; i < m_delList.size(); i++)
		{
			if(::PathFileExists(m_delList.at(i).c_str()))
			{
				::DeleteFile(m_delList.at(i).c_str());
			}
		}

		//start the next task
		m_nCurRow++;
		//The last task done
		if(m_nCurRow >= m_selRows.size())
		{
			ChangeState(TSE_DONE);
		}
		else
		{
			StartTask();
		}
	}
	
	return 0L;
}

void CVCDlg::UpdateStatus(int nItem, int nState)
{
	CTaskInfo taskInfo;
	taskInfo.mask = TIF_STATUS;
	taskInfo.m_nState = nState;
	m_taskListCtrl.UpdateRow(nItem, &taskInfo);
}
void CVCDlg::UpdateProgress(int nItem, double dPercent)
{
	CTaskInfo taskInfo;
	taskInfo.mask = TIF_PROGRESS;
	taskInfo.m_dProgress = dPercent;
	m_taskListCtrl.UpdateRow(nItem, &taskInfo);
}

LRESULT CVCDlg::OnNotifyProcProgress(WPARAM wParam, LPARAM lParam)
{
	std::string* pVal = (std::string*)wParam;
	std::string* pPercent = (std::string*)lParam;
	
	double dPercent = 0;
	std::string val;

	if(pVal != NULL && m_taskListCtrl.GetMetaInfo(m_selRows.at(m_nCurRow), ID_LENGTH, val))
	{
		double dCurVal = atof(pVal->c_str());
		double dTotal = atof(val.c_str());
		if(dTotal > 0)
		{
			dPercent = (dCurVal / dTotal);
		}
	}
	else
	{
		dPercent = atof(pPercent->c_str());
	}

	if(dPercent < 0.0f)
	{
		dPercent = 0.0f;
	}
	else if(dPercent > 1.0f)
	{
		dPercent = 1.0f;
	}

	dPercent *= m_cmdInfos.at(m_nCurCmd).m_nWeight;
	dPercent += (double)m_nProgressBase;

	UpdateProgress(m_selRows.at(m_nCurRow), dPercent / 100);
	
	return 0L;
}

void CVCDlg::EnableButtons(BOOL bRunning)
{
	CMainDlg* pMainWnd = (CMainDlg*)AfxGetMainWnd();
	if(pMainWnd == NULL || pMainWnd->m_pMainToolBarCtrl == NULL)
	{
		return;
	}
	//ASSERT(pMainWnd != NULL && pMainWnd->m_pMainToolBarCtrl != NULL);
	
	pMainWnd->m_pMainToolBarCtrl->EnableButton(ID_MAIN_TBBTN_START, !bRunning);
	pMainWnd->m_pMainToolBarCtrl->EnableButton(ID_MAIN_TBBTN_STOP, bRunning);
}

void CVCDlg::ChangeState(int newState)
{
	if(m_nCurTaskState != newState)
	{
		m_nCurTaskState = newState;
		EnableButtons(m_nCurTaskState == TSE_RUNNING);
	}
}
