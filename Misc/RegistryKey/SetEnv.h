// SetEnv.h: interface for the CSetEnv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETENV_H__C3A7306F_392D_47AC_B6CE_D3283B60672D__INCLUDED_)
#define AFX_SETENV_H__C3A7306F_392D_47AC_B6CE_D3283B60672D__INCLUDED_

#include <windows.h>
#include <string>
#include <vector>

class CEnvironment;

class CSetEnv  
{
public:
	class VariableValue
	{
	public:
		std::string value;
		bool expand;
	};
	class VariableEntry
	{
	public:
		std::string name;
		std::string type;
		std::string scope;
		std::string separator;

		std::vector<VariableValue> values;
	};

	CSetEnv();
	virtual ~CSetEnv();

	bool Set(const char * lpXMLFile);
	bool Unset(const char * lpXMLFile);
private:
	static bool LoadFile(const char* lpXMLFile, std::vector<VariableEntry>& entrys);
	static bool ToWide(const std::string& src, std::wstring& dst, UINT nCodePage);
	static bool ExpandVariable(const std::wstring& src, std::wstring& dst);

	CEnvironment* m_pSystemEnv;
	CEnvironment* m_pProcessEnv;
};

#endif // !defined(AFX_SETENV_H__C3A7306F_392D_47AC_B6CE_D3283B60672D__INCLUDED_)
