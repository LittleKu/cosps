#ifndef _I18N_H_
#define _I18N_H_

#pragma once

#ifdef USE_STRING_IDS
#define	RESSTRIDTYPE		LPCTSTR
#define	IDS2RESIDTYPE(id)	#id
#define GetResString(id)	_GetResString(#id)
CString _GetResString(RESSTRIDTYPE StringID);
#else//USE_STRING_IDS
#define	RESSTRIDTYPE		UINT
#define	IDS2RESIDTYPE(id)	id
CString GetResString(RESSTRIDTYPE StringID);
#define _GetResString(id)	GetResString(id)
#endif//!USE_STRING_IDS

class CI18N
{
public:
	void LoadLangDlls();
	void SetCurLang(WORD langID);
	CString GetString(UINT uID);
	virtual ~CI18N();
	static CI18N* GetInstance();
private:
	CI18N();
	HINSTANCE m_hCurLang;
	CArray<HINSTANCE, HINSTANCE&> m_hLangDlls;

	static CI18N* m_singletonInst;
};

#endif