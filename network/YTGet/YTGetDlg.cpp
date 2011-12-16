// YTGetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YTGet.h"
#include "YTGetDlg.h"
#include "AddTaskDlg.h"
#include "CommonUtils.h"
#include "DownloaderMgr.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYTGetDlg dialog


CYTGetDlg::CYTGetDlg(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CYTGetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYTGetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CYTGetDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYTGetDlg)
	DDX_Control(pDX, IDC_TASK_LIST, m_taskListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYTGetDlg, CResizableDialog)
	//{{AFX_MSG_MAP(CYTGetDlg)
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_DOWNLOAD_STATUS, OnUpdateStatus)
	ON_MESSAGE(WM_DOWNLOAD_PROGRESS, OnUpdateProgress)
	ON_MESSAGE(WM_DOWNLOAD_COMPLETE, OnEnd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TASK_LIST, OnItemchangedTaskList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYTGetDlg message handlers

void CYTGetDlg::InitTaskListCtrl()
{
	m_taskListCtrl.Init();
}

void CYTGetDlg::InitResizableDlgAnchor()
{
	AddAnchor(m_taskListCtrl.m_hWnd, TOP_LEFT, BOTTOM_RIGHT);
}

BOOL CYTGetDlg::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitResizableDlgAnchor();

	InitTaskListCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CYTGetDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	
	pDC->FillRect(&rect, SYS_APP()->m_pSysBkBrush);
	
	return TRUE;
}

LRESULT CYTGetDlg::OnUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	int nTaskID = (int)wParam;
	CTaskInfo* pUpdateInfo = (CTaskInfo*)lParam;
	
	int nIndex = 0;
	CTaskInfo* pTaskInfo = NULL;
	if(!GetTaskInfo(nTaskID, &nIndex, &pTaskInfo))
	{
		CString szLog;
		szLog.Format("[OnUpdateStatus]: The task [%02d] is deleted from GUI.", nTaskID);
		LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	else
	{
		m_taskListCtrl.UpdateRow(nIndex, pUpdateInfo);

		ListCtrlSelectionChanged();
	}
	
	return 1L;
}

LRESULT CYTGetDlg::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	int nTaskID = (int)wParam;
	CTaskInfo* pUpdateInfo = (CTaskInfo*)lParam;
	
	int nIndex = 0;
	CTaskInfo* pTaskInfo = NULL;
	if(!GetTaskInfo(nTaskID, &nIndex, &pTaskInfo))
	{
		CString szLog;
		szLog.Format("[OnUpdateProgress]: The task [%02d] is deleted from GUI.", nTaskID);
		LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	else
	{
		m_taskListCtrl.UpdateRow(nIndex, pUpdateInfo);
	}
	
	return 1L;
}

LRESULT CYTGetDlg::OnEnd(WPARAM wParam, LPARAM lParam)
{
	int nTaskID = (int)wParam;
	CTaskInfo* pUpdateInfo = (CTaskInfo*)lParam;
	
	int nIndex = 0;
	CTaskInfo* pTaskInfo = NULL;
	if(!GetTaskInfo(nTaskID, &nIndex, &pTaskInfo))
	{
		CString szLog;
		szLog.Format("[OnEnd]: The task [%02d] is deleted from GUI.", nTaskID);
		LOG4CPLUS_INFO_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	else
	{
		m_taskListCtrl.UpdateRow(nIndex, pUpdateInfo);

		ListCtrlSelectionChanged();
	}
	
	return 1L;
}

void CYTGetDlg::AddTask()
{
	CAddTaskDlg dlg(this);
	dlg.DoModal();
}

void CYTGetDlg::AddBatchTask()
{
	static const TCHAR* lpszUrlList[] = {
		"http://download.httpwatch.com/httpwatch.exe",
		"http://cn2.php.net/distributions/manual/php_enhanced_en.chm",
		"http://wiresharkdownloads.riverbed.com/wireshark/win32/wireshark-win32-1.6.2.exe",
		"http://download.wondershare.com/cbs_down/pdf-converter-pro_full839.exe",
		"http://70.duote.org:8080/rmdemo.zip",
		"http://desktop.youku.com/iku2/iku2.1_setup.exe",
		"http://sc.down.chinaz.com/201108/Mp3ABCut_2.1.0.zip",
		"http://63.duote.org/winmpgvideoconvert.exe",
		"http://download.skype.com/SkypeSetupFull.exe",
		"http://ncu.dl.sourceforge.net/project/aresgalaxy/aresgalaxy/AresRegular217_10272010/aresregular217_installer.exe",
		"http://labs.renren.com/apache-mirror//httpd/httpd-2.2.20-win32-src.zip",
		"http://www.youtubeget.com/down/yg.exe",
		"http://www.any-code-counter.com",
		"fasfasdfafffffffffffffffffffwefwef",
		"f43fifasdkfiefkds"};
		
		int nCount = sizeof(lpszUrlList)/sizeof(lpszUrlList[0]);
		for(int i = 0; i < nCount; i++)
		{
			AddTask(lpszUrlList[i]);
		}
}

void CYTGetDlg::Start()
{
	CTaskInfo* pTaskInfo = NULL;
	int nItem = -1;

	//Get all selected items
	POSITION pos = m_taskListCtrl.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		nItem = m_taskListCtrl.GetNextSelectedItem(pos);
		
		pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(nItem);
		ASSERT(pTaskInfo != NULL);
		
		if(pTaskInfo == NULL)
		{
			continue;
		}
		
		CDownloadParam param;
		param.m_hWnd = GetSafeHwnd();
		param.m_nTaskID = pTaskInfo->m_nTaskID;
		param.m_szUrl = pTaskInfo->m_szUrl;
		param.m_szFileName = pTaskInfo->m_szFileName;
		param.m_nFileSize = 0;
		param.m_nDlType = CCommonUtils::GetDownloadType(param.m_szUrl);
		
		if(pTaskInfo->m_lpDownloaderMgr == NULL)
		{
			pTaskInfo->m_lpDownloaderMgr = new CDownloaderMgr();
		}
		pTaskInfo->m_lpDownloaderMgr->Init(param);
		
		pTaskInfo->m_lpDownloaderMgr->Start();
	}
}

void CYTGetDlg::Pause()
{
	int nItem = -1;
	CTaskInfo* pTaskInfo = NULL;

	//Get all selected items
	POSITION pos = m_taskListCtrl.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		nItem = m_taskListCtrl.GetNextSelectedItem(pos);
		
		pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(nItem);
		ASSERT(pTaskInfo != NULL);
		
		if(pTaskInfo == NULL || pTaskInfo->m_lpDownloaderMgr == NULL)
		{
			continue;
		}
		
		pTaskInfo->m_lpDownloaderMgr->Pause();
	}
}

void CYTGetDlg::Remove()
{
	int nItem = -1;
	CTaskInfo* pTaskInfo = NULL;
	
	POSITION pos = m_taskListCtrl.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		nItem = m_taskListCtrl.GetNextSelectedItem(pos);
		
		pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(nItem);
		ASSERT(pTaskInfo != NULL);
		
		if(pTaskInfo->m_lpDownloaderMgr != NULL)
		{
			if(pTaskInfo->m_lpDownloaderMgr->Destroy() == CFinalizer::DELETE_BY_EXTERNAL)
			{
				delete pTaskInfo->m_lpDownloaderMgr;
			}
		}
	}
	m_taskListCtrl.RemoveSelectedItems();
}

void CYTGetDlg::Restart()
{
	int nItem = -1;
	CTaskInfo* pTaskInfo = NULL;

	//Get all selected items
	POSITION pos = m_taskListCtrl.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		nItem = m_taskListCtrl.GetNextSelectedItem(pos);
		
		pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(nItem);
		ASSERT(pTaskInfo != NULL);
		
		if(pTaskInfo == NULL || pTaskInfo->m_lpDownloaderMgr == NULL)
		{
			continue;
		}
		
		pTaskInfo->m_lpDownloaderMgr->ReStart();
	}
}

void CYTGetDlg::AddTask(LPCTSTR lpszAddress)
{
	CTaskInfo* pTaskInfo = new CTaskInfo();
	pTaskInfo->m_szUrl = lpszAddress;
	pTaskInfo->m_nTaskID = CCommonUtils::GetUniqueID();

	//File name
	BOOL bResult = CCommonUtils::ExtractFileName(lpszAddress, pTaskInfo->m_szFileName);
	if(!bResult || pTaskInfo->m_szFileName.GetLength() >= 64 || pTaskInfo->m_szFileName.IsEmpty())
	{
		pTaskInfo->m_szFileName = "index.html";
	}

	m_taskListCtrl.AddRow(pTaskInfo);
}

BOOL CYTGetDlg::GetTaskInfo(int nTaskID, int* pIndex, CTaskInfo** ppTaskInfo)
{
	ASSERT(pIndex != NULL && ppTaskInfo != NULL);

	int i, nRowCount;
	CTaskInfo* ptr = NULL;
	for(i = 0, nRowCount = m_taskListCtrl.GetItemCount(); i < nRowCount; i++)
	{
		ptr = (CTaskInfo*)m_taskListCtrl.GetItemData(i);
		ASSERT(ptr != NULL);
		
		if(ptr->m_nTaskID == nTaskID)
		{
			*pIndex = i;
			*ppTaskInfo = ptr;			
			return TRUE;
		}
	}
	
	return FALSE;
}

void CYTGetDlg::OnItemchangedTaskList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	CString szLog;
	if(IS_LOG_ENABLED(ROOT_LOGGER, log4cplus::TRACE_LOG_LEVEL))
	{
		szLog.Format("uChanged = %04x, uNewState = %04x, uOldState = %04x, iItem = %d. ",
			pNMListView->uChanged, pNMListView->uNewState, pNMListView->uOldState, pNMListView->iItem);
		LOG4CPLUS_TRACE_STR(ROOT_LOGGER, (LPCTSTR)szLog)
	}
	if(pNMListView->uChanged == LVIF_STATE)
    {
		//1. uNewState & LVIS_SELECTED, there's new items selected
		//2. uOldState & LVIS_SELECTED, there's old items deselected
		//we care about both events
		if((pNMListView->uNewState & LVIS_SELECTED) || (pNMListView->uOldState & LVIS_SELECTED))
		{
			ListCtrlSelectionChanged();
		}
    }
	
	*pResult = 0;
}

void CYTGetDlg::ListCtrlSelectionChanged()
{
	DWORD dwStatus = DL_OPER_FLAG_NONE;
	
	POSITION pos = m_taskListCtrl.GetFirstSelectedItemPosition();
	
	int nItem = -1;
	CTaskInfo* pTaskInfo = NULL;
	while (pos != NULL)
	{
		nItem = m_taskListCtrl.GetNextSelectedItem(pos);
		
		pTaskInfo = (CTaskInfo*)m_taskListCtrl.GetItemData(nItem);
		ASSERT(pTaskInfo != NULL);
		
		dwStatus |= pTaskInfo->m_dlState.GetAccess();
	}
	
	EnableButtons(dwStatus);
}

void CYTGetDlg::EnableButtons(DWORD dwStatus)
{
	CMainDlg* pMainWnd = (CMainDlg*)AfxGetMainWnd();
	ASSERT(pMainWnd != NULL && pMainWnd->m_pMainToolBarCtrl != NULL);

	pMainWnd->m_pMainToolBarCtrl->EnableButton(ID_TBBTN_START, dwStatus & DL_OPER_FLAG_START);
	pMainWnd->m_pMainToolBarCtrl->EnableButton(ID_TBBTN_PAUSE, dwStatus & DL_OPER_FLAG_PAUSE);
	pMainWnd->m_pMainToolBarCtrl->EnableButton(ID_TBBTN_REMOVE, dwStatus & DL_OPER_FLAG_REMOVE);
	pMainWnd->m_pMainToolBarCtrl->EnableButton(ID_TBBTN_RESTART, dwStatus & DL_OPER_FLAG_REDOWNLOAD);
}
