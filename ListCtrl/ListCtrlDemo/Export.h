#ifndef _EXPORT_H_
#define _EXPORT_H_

class IExporter
{
public:
	virtual BOOL DoExport(LPCTSTR lpFileName)   = 0;
};

//class CListCtrlDemoDlg;
class CResultListCtrl;
class CTotalInfo;

class CCSVExporter : public IExporter
{
public:
	CCSVExporter(CResultListCtrl* pListCtrl, CTotalInfo* pTotalInfo);
	virtual BOOL DoExport(LPCTSTR lpFileName);
	void LoadHeaderString(CString& str);
private:
	CResultListCtrl* m_pListCtrl;
	CTotalInfo* m_pTotalInfo;
};

class CXLSExporter : public IExporter
{
public:
	CXLSExporter(CResultListCtrl* pListCtrl, CTotalInfo* pTotalInfo);
	virtual BOOL DoExport(LPCTSTR lpFileName);
private:
	CResultListCtrl* m_pListCtrl;
	CTotalInfo* m_pTotalInfo;
};

class CXMLExporter : public IExporter
{
public:
	CXMLExporter(CResultListCtrl* pListCtrl, CTotalInfo* pTotalInfo);
	virtual BOOL DoExport(LPCTSTR lpFileName);
private:
	CResultListCtrl* m_pListCtrl;
	CTotalInfo* m_pTotalInfo;
};

class CHTMLExporter : public IExporter
{
public:
	CHTMLExporter(CResultListCtrl* pListCtrl, CTotalInfo* pTotalInfo);
	virtual BOOL DoExport(LPCTSTR lpFileName);
private:
	void FormatHtmlHeader(CString& s, LPCTSTR lpTitle, LPCTSTR lpVersionInfo);
	void FormatHtmlTail(CString& s, LPCTSTR lpCompany);
	void FormatContentHeader(CString& s, LPCTSTR lpContentHeader, LPCTSTR lpProduct);
	void FormatSummuryHeader(CString& s, LPCTSTR lpSummuryTitle, LPCTSTR lpSummaryInfo);
	void FormatSummuryTail(CString& s);
	void FormatSummaryItemInfo(CString& s, LPCTSTR lpType, int nCount, LPCTSTR lpPercent, COLORREF crBkColor);
	void FormatDetailHeader(CString& s, LPCTSTR lpTitle);
	void FormatDetailTail(CString& s);
	void FormatDetailLastItem(CString& s);
	CString GetHtmlColorStr(COLORREF cr = RGB(255, 255, 0));
private:
	CResultListCtrl* m_pListCtrl;
	CTotalInfo* m_pTotalInfo;
};


#endif