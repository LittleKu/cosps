// HistoryComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlDemo.h"
#include "HistoryComboBox.h"
#include "./ThirdParty/IniFileReadWrite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboBox

CHistoryComboBox::CHistoryComboBox()
{
	m_nMaxHistoryItems = 10;
}

CHistoryComboBox::~CHistoryComboBox()
{
}


BEGIN_MESSAGE_MAP(CHistoryComboBox, CComboBox)
	//{{AFX_MSG_MAP(CHistoryComboBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistoryComboBox message handlers
void CHistoryComboBox::OnDestroy()
{
	SaveHistory();
	CComboBox::OnDestroy();
}

void CHistoryComboBox::SetIniKeyNames(LPCTSTR lpFileName, LPCTSTR lpSectionName, LPCTSTR lpKeyBaseName)
{
	m_sIniFile = lpFileName;
	m_sSectionName = lpSectionName;
	m_sKeyBaseName = lpKeyBaseName;
}
void CHistoryComboBox::Init()
{
	SetIniKeyNames(SYS_PREF_INI_FILE(), _T("SRC_FILE_FILTER_LIST"), _T("Filter"));
	LoadHistory();
}

int CHistoryComboBox::AddString( LPCTSTR lpszString )
{
	CString combostring = lpszString;
	combostring.TrimLeft();
	combostring.TrimRight();

	if(combostring.IsEmpty())
	{
		return -1;
	}
	//search the Combo for another string like this
	//and delete it if one is found
	int nIndex = FindStringExact(0, combostring);
	if (nIndex >= 0)
	{
		DeleteString(nIndex);
	}

	nIndex = CComboBox::AddString(combostring);
	return nIndex;
}

void CHistoryComboBox::LoadHistory()
{
	if(!CommonUtils::IsFileExist(m_sIniFile))
	{
		return;
	}
	//Load history items
	CString sKey;
	CString sValue;
	int n = 0;
	do
	{
		sKey.Format(_T("%s%d"), m_sKeyBaseName, n);
		sValue = IniFile::GetKeyValue(SYS_PREF_INI_FILE(), m_sSectionName, sKey);
		if (!sValue.IsEmpty())
		{
			AddString(sValue);
		}
		n++;
	}while(!sValue.IsEmpty());
}

void CHistoryComboBox::SaveHistory()
{
	//Save all the items

	//Clear the old items
	::WritePrivateProfileSection(m_sSectionName, "", m_sIniFile);
	
	int nMax = min(GetCount(), m_nMaxHistoryItems);

	CString sKey;
	CString sValue;
	int i;
	for(i = 0; i < nMax; i++)
	{
		GetLBText(i, sValue);
		sKey.Format(_T("%s%d"), m_sKeyBaseName, i);
		::WritePrivateProfileString(m_sSectionName, sKey, sValue, m_sIniFile);
	}
}