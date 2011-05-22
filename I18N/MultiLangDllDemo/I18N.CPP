#include "StdAfx.h"
#include "I18N.H"

CString GetResString(RESSTRIDTYPE StringID)
{
	return CI18N::GetInstance()->GetString(StringID);
}

CI18N* CI18N::m_singletonInst = NULL;

CI18N* CI18N::GetInstance()
{
	if(m_singletonInst == NULL)
	{
		m_singletonInst = new CI18N();
	}
	return m_singletonInst;
}

CI18N::CI18N()
{
	LoadLangDlls();
}

CI18N::~CI18N()
{
	for(int i = 0; i < m_hLangDlls.GetSize(); i++)
	{
		HINSTANCE hInst = m_hLangDlls.GetAt(i);
		if(hInst != NULL)
		{
			::FreeLibrary(hInst);
		}
	}
	m_hLangDlls.RemoveAll();
}

void CI18N::LoadLangDlls()
{
	HINSTANCE hInst = NULL;
	hInst = ::LoadLibrary(_T("zh_CN.dll"));
	m_hLangDlls.Add(hInst);

	m_hCurLang = hInst;

	hInst = ::LoadLibrary(_T("zh_TW.dll"));
	m_hLangDlls.Add(hInst);
}

void CI18N::SetCurLang(WORD langID)
{
	m_hCurLang = m_hLangDlls.GetAt(langID);
}

#ifdef _UNICODE
#define CHAR_FUDGE 1    // one TCHAR unused is good enough
#else
#define CHAR_FUDGE 2    // two BYTES unused for case of DBC last char
#endif
CString CI18N::GetString(UINT uID)
{
	CString str;

	// try fixed buffer first (to avoid wasting space in the heap)
	TCHAR szTemp[256];
	int nLen = ::LoadString(m_hCurLang, uID, szTemp, 256);
	if (256 - nLen > CHAR_FUDGE)
	{
		if(nLen == 0)
		{
			::LoadString(GetModuleHandle(NULL), uID, szTemp, 256);
		}
		str = szTemp;
		return str;
	}
	
	// try buffer size of 512, then larger size until entire string is retrieved
	int nSize = 256;
	do
	{
		nSize += 256;
		nLen = ::LoadString(m_hCurLang, uID, str.GetBuffer(nSize-1), nSize);
	} while (nSize - nLen <= CHAR_FUDGE);
	str.ReleaseBuffer();
	if (nLen == 0)
		str = "";
	return str;
}