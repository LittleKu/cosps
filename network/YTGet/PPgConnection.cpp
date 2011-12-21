// PPgConnection.cpp : implementation file
//

#include "stdafx.h"
#include "YTGet.h"
#include "PPgConnection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPgConnection property page

IMPLEMENT_DYNCREATE(CPPgConnection, CPropertyPage)

CPPgConnection::CPPgConnection() : CPropertyPage(CPPgConnection::IDD)
{
	//{{AFX_DATA_INIT(CPPgConnection)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPPgConnection::~CPPgConnection()
{
}

void CPPgConnection::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPgConnection)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPgConnection, CPropertyPage)
	//{{AFX_MSG_MAP(CPPgConnection)
	ON_EN_CHANGE(IDC_EDIT_MAX_TASK_COUNT, OnSettingsChange)
	ON_EN_CHANGE(IDC_EDIT_MAX_CONNECTION_COUNT, OnSettingsChange)
	ON_EN_CHANGE(IDC_EDIT_MIN_SEGMENT_SIZE, OnSettingsChange)
	ON_EN_CHANGE(IDC_EDIT_MAX_RETRY_TIMES, OnSettingsChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPgConnection message handlers

BOOL CPPgConnection::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	SetDlgItemInt(IDC_EDIT_MAX_TASK_COUNT, SYS_OPTIONS()->m_nMaxTaskCount, FALSE);
	SetDlgItemInt(IDC_EDIT_MAX_CONNECTION_COUNT, SYS_OPTIONS()->m_nMaxConnectionCount, FALSE);

	UINT nMinSegSizeInMB = SYS_OPTIONS()->m_nMinSegmentSize / (1024 * 1024);
	SetDlgItemInt(IDC_EDIT_MIN_SEGMENT_SIZE, nMinSegSizeInMB, FALSE);
	SetDlgItemInt(IDC_EDIT_MAX_RETRY_TIMES, SYS_OPTIONS()->m_nMaxRetryTimes, FALSE);
	
	return TRUE;
}

BOOL CPPgConnection::OnApply() 
{
	BOOL bResult = FALSE;
	do 
	{
		bResult = SaveParameter(IDC_EDIT_MAX_TASK_COUNT, SYS_OPTIONS()->m_nMaxTaskCount, 1, 10, 
			"maximum task count");
		if(!bResult)
		{
			break;
		}

		bResult = SaveParameter(IDC_EDIT_MAX_CONNECTION_COUNT, SYS_OPTIONS()->m_nMaxConnectionCount, 1, 16, 
			"maximum connection count");
		if(!bResult)
		{
			break;
		}

		bResult = SaveParameter(IDC_EDIT_MIN_SEGMENT_SIZE, SYS_OPTIONS()->m_nMinSegmentSize, 1, 10, 
			"minimum segment size", 1024 * 1024);
		if(!bResult)
		{
			break;
		}

		bResult = SaveParameter(IDC_EDIT_MAX_RETRY_TIMES, SYS_OPTIONS()->m_nMaxRetryTimes, 1, 100, 
			"maximum retry times");
		if(!bResult)
		{
			break;
		}

		bResult = TRUE;

	} while (FALSE);

	if(!bResult)
	{
		return FALSE;
	}
	
	return CPropertyPage::OnApply();
}

BOOL CPPgConnection::SaveParameter(UINT nCtrlID, UINT& nParam, UINT nMin, UINT nMax, LPCTSTR lpszParamName, UINT nMultiple)
{
	CString szValue;
	GetDlgItemText(nCtrlID, szValue);
	
	UINT nValue;
	if( (_stscanf(szValue, "%u", &nValue) == 1) && nValue >= nMin && nValue <= nMax )
	{
		nParam = nValue * nMultiple;
		return TRUE;
	}
	else
	{
		CString szPrompt;
		szPrompt.Format("Invalid value for %s: value = %s, range = (%u-%u).", lpszParamName, szValue, nMin, nMax);
		AfxMessageBox(szPrompt);

		return FALSE;
	}
}
