// ProcessEnv.h: interface for the CProcessEnv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESSENV_H__73109B96_DBCB_4CA4_BA34_2E7C3714A738__INCLUDED_)
#define AFX_PROCESSENV_H__73109B96_DBCB_4CA4_BA34_2E7C3714A738__INCLUDED_

#include "Environment.h"

class CProcessEnv : public CEnvironment 
{
public:
	CProcessEnv();
	virtual ~CProcessEnv();

	virtual BOOL SetEnv(LPCWSTR lpName,	const std::wstring& strValue);
	virtual BOOL GetEnv(LPCWSTR lpName, std::wstring& strValue );
};

#endif // !defined(AFX_PROCESSENV_H__73109B96_DBCB_4CA4_BA34_2E7C3714A738__INCLUDED_)
