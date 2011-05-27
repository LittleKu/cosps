#ifndef _EXPORT_H_
#define _EXPORT_H_

class IExporter
{
public:
	virtual BOOL PreExport()  {}
	virtual BOOL DoExport(LPCTSTR lpFileName)   = 0;
	virtual BOOL PostExport() {}
};

class CCSVExporter
{
public:
	CCSVExporter(CListCtrl* pListCtrl);
	virtual BOOL DoExport(LPCTSTR lpFileName);
	void LoadHeaderString(CString& str);
private:
	CListCtrl* m_pListCtrl;
};

#endif