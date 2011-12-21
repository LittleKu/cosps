// PPgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "YTGet.h"
#include "PPgProxy.h"
#include "Options.h"
#include "CommonUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPPgProxy property page

IMPLEMENT_DYNCREATE(CPPgProxy, CPropertyPage)

CPPgProxy::CPPgProxy() : CPropertyPage(CPPgProxy::IDD)
{
	//{{AFX_DATA_INIT(CPPgProxy)
	//}}AFX_DATA_INIT
}

CPPgProxy::~CPPgProxy()
{
}

void CPPgProxy::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPPgProxy)
	DDX_Control(pDX, IDC_EDIT_PROXY_ADDRESS, m_editAddress);
	DDX_Control(pDX, IDC_EDIT_PROXY_PORT, m_editPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPgProxy, CPropertyPage)
	//{{AFX_MSG_MAP(CPPgProxy)
	ON_BN_CLICKED(IDC_RADIO_NO_PROXY, OnRadioNoProxy)
	ON_BN_CLICKED(IDC_RADIO_PROXY_MANUALLY, OnRadioProxyManually)
	ON_BN_CLICKED(IDC_RADIO_PROXY_FROM_IE, OnRadioProxyFromIe)
	ON_EN_CHANGE(IDC_EDIT_PROXY_ADDRESS, OnSettingsChange)
	ON_EN_CHANGE(IDC_EDIT_PROXY_PORT, OnSettingsChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPgProxy message handlers

BOOL CPPgProxy::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	//Proxy Mode
	if(SYS_OPTIONS()->m_nProxyMode < COptions::PME_NO_PROXY || SYS_OPTIONS()->m_nProxyMode > COptions::PME_SYS_PROXY)
	{
		ASSERT(FALSE);
		SYS_OPTIONS()->m_nProxyMode = COptions::PME_NO_PROXY;
	}
	int nIDCheckButton = IDC_RADIO_NO_PROXY + (SYS_OPTIONS()->m_nProxyMode - COptions::PME_NO_PROXY);
	CheckRadioButton(IDC_RADIO_NO_PROXY, IDC_RADIO_PROXY_FROM_IE, nIDCheckButton);
	
	//Proxy Server
	RadioSelChanged(nIDCheckButton);
	
	return TRUE;
}

void CPPgProxy::OnRadioNoProxy() 
{
	int nIDCheckButton = GetCheckedRadioButton(IDC_RADIO_NO_PROXY, IDC_RADIO_PROXY_FROM_IE);
	RadioSelChanged(nIDCheckButton);

	SetModified();
}

void CPPgProxy::OnRadioProxyManually() 
{
	int nIDCheckButton = GetCheckedRadioButton(IDC_RADIO_NO_PROXY, IDC_RADIO_PROXY_FROM_IE);
	RadioSelChanged(nIDCheckButton);

	SetModified();
}

void CPPgProxy::OnRadioProxyFromIe() 
{
	int nIDCheckButton = GetCheckedRadioButton(IDC_RADIO_NO_PROXY, IDC_RADIO_PROXY_FROM_IE);
	RadioSelChanged(nIDCheckButton);

	SetModified();
}

void CPPgProxy::RadioSelChanged(int nIDCheckButton)
{
	//Proxy Server
	if(nIDCheckButton == IDC_RADIO_NO_PROXY)
	{
		SetDlgItemText(IDC_EDIT_PROXY_ADDRESS, "");
		SetDlgItemText(IDC_EDIT_PROXY_PORT, "");
		
		m_editAddress.SetReadOnly(TRUE);
		m_editPort.SetReadOnly(TRUE);
	}
	else if(nIDCheckButton == IDC_RADIO_PROXY_MANUALLY)
	{
		CString szProxyServer;
		UINT nProxyPort;

		if(CCommonUtils::GetProxyServerPort(SYS_OPTIONS()->m_szProxy, szProxyServer, nProxyPort))
		{
			SetDlgItemText(IDC_EDIT_PROXY_ADDRESS, szProxyServer);
			SetDlgItemInt(IDC_EDIT_PROXY_PORT, nProxyPort, FALSE);
		}
		else
		{
			SetDlgItemText(IDC_EDIT_PROXY_ADDRESS, "");
			SetDlgItemText(IDC_EDIT_PROXY_PORT, "");
		}
		
		m_editAddress.SetReadOnly(FALSE);
		m_editPort.SetReadOnly(FALSE);
	}
	else if(nIDCheckButton == IDC_RADIO_PROXY_FROM_IE)
	{
		CString szProxy;
		CCommonUtils::GetProxyInfo(szProxy, CCommonUtils::PROXY_SERVER_HTTP);

		CString szProxyServer;
		UINT nProxyPort;
		
		if(CCommonUtils::GetProxyServerPort(szProxy, szProxyServer, nProxyPort))
		{
			SetDlgItemText(IDC_EDIT_PROXY_ADDRESS, szProxyServer);
			SetDlgItemInt(IDC_EDIT_PROXY_PORT, nProxyPort, FALSE);
		}
		else
		{
			SetDlgItemText(IDC_EDIT_PROXY_ADDRESS, "");
			SetDlgItemText(IDC_EDIT_PROXY_PORT, "");
		}
		
		m_editAddress.SetReadOnly(TRUE);
		m_editPort.SetReadOnly(TRUE);
	}
}


BOOL CPPgProxy::OnApply() 
{
	//Proxy Mode
	int nIDCheckButton = GetCheckedRadioButton(IDC_RADIO_NO_PROXY, IDC_RADIO_PROXY_FROM_IE);
	SYS_OPTIONS()->m_nProxyMode = COptions::PME_NO_PROXY + (nIDCheckButton - IDC_RADIO_NO_PROXY);

	//Proxy server
	if(nIDCheckButton == IDC_RADIO_PROXY_MANUALLY)
	{
		if(!SaveProxy())
		{
			SYS_OPTIONS()->m_szProxy = "";
			return FALSE;
		}
	}
	else if(nIDCheckButton == IDC_RADIO_PROXY_FROM_IE)
	{
		CString szProxyServer, szProxyPort;
		m_editAddress.GetWindowText(szProxyServer);
		m_editPort.GetWindowText(szProxyPort);

		if(!szProxyServer.IsEmpty())
		{
			if(szProxyPort.IsEmpty())
			{
				SYS_OPTIONS()->m_szProxy.Format("%s", szProxyServer);
			}
			else
			{
				SYS_OPTIONS()->m_szProxy.Format("%s:%s", szProxyServer, szProxyPort);
			}
		}
		else
		{
			SYS_OPTIONS()->m_szProxy = "";
		}
	}

	SetModified(FALSE);
	return CPropertyPage::OnApply();
}

BOOL CPPgProxy::SaveProxy()
{
	CString szProxyServer, szProxyPort;
	m_editAddress.GetWindowText(szProxyServer);
	m_editPort.GetWindowText(szProxyPort);

	BOOL bResult = FALSE;
	do 
	{
		//Address
		if(szProxyServer.IsEmpty())
		{
			AfxMessageBox(_T("Proxy server address can't be empty."));
			break;
		}

		//Port
		UINT nPort;
		if( (_stscanf(szProxyPort, "%u", &nPort) == 1) && nPort >= 1 && nPort <= 65535)
		{
			SYS_OPTIONS()->m_szProxy.Format("%s:%u", szProxyServer, nPort);
			bResult = TRUE;
		}
		else
		{
			AfxMessageBox(_T("Invalid proxy server port."));
			break;
		}

	} while (FALSE);
	
	return bResult;
}
