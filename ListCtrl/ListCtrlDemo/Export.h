#ifndef _EXPORT_H_
#define _EXPORT_H_

class IExporter
{
public:
	virtual BOOL DoExport(LPCTSTR lpFileName)   = 0;
};

class CResultListCtrl;

class CCSVExporter : public IExporter
{
public:
	CCSVExporter(CListCtrl* pListCtrl);
	virtual BOOL DoExport(LPCTSTR lpFileName);
	void LoadHeaderString(CString& str);
private:
	CListCtrl* m_pListCtrl;
};

class CXLSExporter : public IExporter
{
public:
	CXLSExporter(CResultListCtrl* pListCtrl);
	virtual BOOL DoExport(LPCTSTR lpFileName);
private:
	CResultListCtrl* m_pListCtrl;
};

class CXMLExporter : public IExporter
{
public:
	CXMLExporter(CResultListCtrl* pListCtrl);
	virtual BOOL DoExport(LPCTSTR lpFileName);
private:
	CResultListCtrl* m_pListCtrl;
};

class CHTMLExporter : public IExporter
{
public:
	CHTMLExporter(CResultListCtrl* pListCtrl);
	virtual BOOL DoExport(LPCTSTR lpFileName);
private:
	CResultListCtrl* m_pListCtrl;
	CString GetHtmlColorStr(COLORREF cr = RGB(255, 255, 0));
};


#endif