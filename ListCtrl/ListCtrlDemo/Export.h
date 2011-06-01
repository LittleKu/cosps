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
	void FormatHtmlHeader(CString& s, LPCTSTR lpTitle, LPCTSTR lpVersionInfo);
	void FormatHtmlTail(CString& s, LPCTSTR lpCompany);
	void FormatContentHeader(CString& s, LPCTSTR lpContentHeader, LPCTSTR lpProduct);
	void FormatSummuryHeader(CString& s, LPCTSTR lpSummuryTitle);
	void FormatSummuryTail(CString& s);
	void FormatDetailHeader(CString& s, LPCTSTR lpTitle);
	void FormatDetailTail(CString& s);

	CResultListCtrl* m_pListCtrl;
	CString GetHtmlColorStr(COLORREF cr = RGB(255, 255, 0));
};


#endif