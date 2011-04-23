//  ProgressDlg.cpp : implementation file
// CG: This file was added by the Progress Dialog component

#include "stdafx.h"
#include "resource.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog
clock_t CProgressDlg::m_clockLast = 0;
clock_t CProgressDlg::m_clockCurr = 0;

CProgressDlg::CProgressDlg() : m_bCancel(FALSE), m_bParentDisabled(FALSE)
{
}

CProgressDlg::~CProgressDlg()
{
    if(m_hWnd != NULL)
	{
		DestroyWindow();
		m_hWnd = NULL;
	}
}

BOOL CProgressDlg::DestroyWindow()
{
    ReEnableParent();
    return CDialog::DestroyWindow();
}

void CProgressDlg::ReEnableParent()
{
    if(m_bParentDisabled && (m_pParentWnd!=NULL))
      m_pParentWnd->EnableWindow(TRUE);
    m_bParentDisabled=FALSE;
}

BOOL CProgressDlg::Create(CWnd *pParent)
{
    // Get the true parent of the dialog
    m_pParentWnd = CWnd::GetSafeOwner(pParent);

    // m_bParentDisabled is used to re-enable the parent window
    // when the dialog is destroyed. So we don't want to set
    // it to TRUE unless the parent was already enabled.

    if((m_pParentWnd!=NULL) && m_pParentWnd->IsWindowEnabled())
    {
      m_pParentWnd->EnableWindow(FALSE);
      m_bParentDisabled = TRUE;
    }

    if(!CDialog::Create(CProgressDlg::IDD,pParent))
    {
      ReEnableParent();
      return FALSE;
    }

    return TRUE;
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CProgressDlg)
    DDX_Control(pDX, CG_IDC_PROGDLG_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
    //{{AFX_MSG_MAP(CProgressDlg)
	ON_MESSAGE(WM_UPDATE_PROGRESS, OnUpdateProgress)
	ON_WM_SYSCOMMAND()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CProgressDlg::SetStatus(LPCTSTR lpszMessage)
{
    ASSERT(m_hWnd); // Don't call this _before_ the dialog has
                    // been created. Can be called from OnInitDialog
    CWnd *pWndStatus = GetDlgItem(CG_IDC_PROGDLG_STATUS);

    // Verify that the static text control exists
    ASSERT(pWndStatus!=NULL);

	pWndStatus->SetWindowText(lpszMessage);
}

void CProgressDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		OnCancel();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CProgressDlg::OnCancel()
{
    m_bCancel = TRUE;
}

BOOL CProgressDlg::CheckCancelButton()
{
    return m_bCancel;
}

CProgressCtrl* CProgressDlg::GetProgressCtrl()
{
	return &m_Progress;
}

void CProgressDlg::UpdatePercent(int nNewPos)
{
    CWnd *pWndPercent = GetDlgItem(CG_IDC_PROGDLG_PERCENT);
    int nPercent, nLower, nUpper;

	m_Progress.GetRange(nLower, nUpper);

    int nDivisor = nUpper - nLower;
    ASSERT(nDivisor>0);  // nLower should be smaller than m_nUpper

    int nDividend = (nNewPos - nLower);
    ASSERT(nDividend>=0);   // Current position should be greater than nLower

    nPercent = nDividend * 100 / nDivisor;

    // Since the Progress Control wraps, we will wrap the percentage
    // along with it. However, don't reset 100% back to 0%
    if(nPercent!=100)
      nPercent %= 100;

    // Display the percentage
    CString strBuf;
    strBuf.Format(_T("%d%c"),nPercent,_T('%'));

	CString strCur; // get current percentage
    pWndPercent->GetWindowText(strCur);

	if (strCur != strBuf)
		pWndPercent->SetWindowText(strBuf);
}
    
/////////////////////////////////////////////////////////////////////////////
// CProgressDlg message handlers

BOOL CProgressDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
	m_Progress.SetRange(0, 100);
	m_Progress.SetStep(1);
	m_Progress.SetPos(0);

	CWnd *pWndStatus = GetDlgItem(CG_IDC_PROGDLG_STATUS);
	
    // Verify that the static text control exists
    ASSERT(pWndStatus!=NULL);
	pWndStatus->SetFont(this->GetParent()->GetFont());

    return TRUE;  
}

LRESULT CProgressDlg::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	m_clockCurr = clock();
	int nPos, nLower, nUpper;

	m_Progress.GetRange(nLower, nUpper);
	nPos = m_Progress.GetPos();

	if(CheckCancelButton () || nPos >= nUpper)
	{
		return (LRESULT)0;
	}
	
	nPos = (int)wParam;

	//SetPos
	m_Progress.SetPos(nPos);

	//The time difference in milliseconds;
	long diff = m_clockCurr - m_clockLast;
	if(CLOCKS_PER_SEC != 1000)
	{
		diff = diff * 1000 / CLOCKS_PER_SEC;
	}
	//This is used to reduce the flicker if the refresh speed is too fast (less than 1ms)
	//Only the time difference is greater or equal to 1 ms, refresh the static text
	if( nPos >= nUpper || diff >= 10)
	{
		//Status Text
		CString str;
		str.Format("Currect Status = %d", nPos);
		SetStatus(str);
		
		//Percent
		UpdatePercent(nPos);

		m_clockLast = clock();
	}

	return (LRESULT)1;
}
