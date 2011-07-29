#include "StdAfx.h"
#include "Export.h"
#include "ResultListCtrl.h"
#include "FileParser.h"
#include "ListCtrlDemo.h"

CCSVExporter::CCSVExporter(CResultListCtrl* pListCtrl, CTotalInfo* pTotalInfo) : 
	m_pListCtrl(pListCtrl), m_pTotalInfo(pTotalInfo)
{
}

BOOL CCSVExporter::DoExport(LPCTSTR lpFileName)
{
	BOOL bResult = FALSE;

	CString szText; 
	int i, j;
	TRY
	{
		CStdioFile cFile(lpFileName, CFile::modeCreate | CFile::modeWrite);
		
		szText.Empty();
		LoadHeaderString(szText);
		cFile.WriteString(szText + "\n");
		
		
		int nColCount = m_pListCtrl->GetHeaderCtrl()->GetItemCount();
		int nRowCount = m_pListCtrl->GetItemCount();
		for(i = 0; i < nRowCount; i++)
		{
			szText.Empty();
			for(j = 0; j < nColCount; j++)
			{
				szText += m_pListCtrl->GetItemText(i, j);
				if(j < (nColCount - 1))
				{
					szText += ",";
				}
			}
			szText += "\n";
			
			cFile.WriteString(szText);
		}

		//Total Statistic
		szText = _T("\n\n\n");
		cFile.WriteString(szText);

		szText.Format("%s\n", _T("Total"));
		cFile.WriteString(szText);

		//Header
		szText.Format("%s,%s,%s\n", _T("Type"), _T("Count"), _T("Percentage(%)"));
		cFile.WriteString(szText);

		szText.Format("%s,%d,%s\n", _T("Total File Count:"), m_pTotalInfo->m_nTotalCount, _T("100%"));
		cFile.WriteString(szText);

		szText.Format("%s,%d,%s\n", _T("Total Lines:"), m_pTotalInfo->m_nTotalLines, _T("100%"));
		cFile.WriteString(szText);

		szText.Format("%s,%d,%s\n", _T("Total Code:"), m_pTotalInfo->m_nTotalCodeLines, 
			CommonUtils::GetPercentStr(m_pTotalInfo->m_nTotalCodeLines, m_pTotalInfo->m_nTotalLines));
		cFile.WriteString(szText);

		szText.Format("%s,%d,%s\n", _T("Total Comment:"), m_pTotalInfo->m_nTotalCommentLines, 
			CommonUtils::GetPercentStr(m_pTotalInfo->m_nTotalCommentLines, m_pTotalInfo->m_nTotalLines));
		cFile.WriteString(szText);

		szText.Format("%s,%d,%s\n", _T("Total Mixed:"), m_pTotalInfo->GetTotalMixedLines(), 
			CommonUtils::GetPercentStr(m_pTotalInfo->GetTotalMixedLines(), m_pTotalInfo->m_nTotalLines));
		cFile.WriteString(szText);

		szText.Format("%s,%d,%s\n", _T("Total Blank:"), m_pTotalInfo->m_nTotalBlankLines, 
			CommonUtils::GetPercentStr(m_pTotalInfo->m_nTotalBlankLines, m_pTotalInfo->m_nTotalLines));
		cFile.WriteString(szText);

		cFile.Close();
		
		bResult = TRUE;
	}
	CATCH(CFileException, e)
	{
		TCHAR szCause[255];
		CString msg;
		
		e->GetErrorMessage(szCause, 255);	   
		AfxMessageBox(szCause, MB_OK | MB_ICONEXCLAMATION);
	}
	AND_CATCH_ALL(e)
	{
		AfxMessageBox("Error writing to file!", 
			MB_OK | MB_ICONEXCLAMATION);
	}
	END_CATCH_ALL
		
	return bResult;    
}

void CCSVExporter::LoadHeaderString(CString& str)
{
	CHeaderCtrl* pHeaderCtrl = m_pListCtrl->GetHeaderCtrl();
	int nColCount = pHeaderCtrl->GetItemCount();

	TCHAR lpBuffer[256];

	HDITEM hdi;
	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256;

	for(int i = 0; i < nColCount; i++)
	{
		pHeaderCtrl->GetItem(i, &hdi);
		str += hdi.pszText;
		if(i < (nColCount - 1))
		{
			str += ",";
		}
	}
}



CXLSExporter::CXLSExporter(CResultListCtrl* pListCtrl, CTotalInfo* pTotalInfo) : m_pListCtrl(pListCtrl), m_pTotalInfo(pTotalInfo)
{

}

BOOL CXLSExporter::DoExport(LPCTSTR lpFileName)
{
	if(::PathFileExists(lpFileName))
	{
		if(!::DeleteFile(lpFileName))
		{
			CString szCause;
			szCause.Format(_T("Failed to overwrite the file: %s"), lpFileName);
			AfxMessageBox(szCause, MB_OK | MB_ICONEXCLAMATION);
			return FALSE;
		}
	}
	BOOL bResult = FALSE;
	
	CDatabase database;
	//Excel driver
	CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)";
	CString sSql;
	
	TRY
	{
		//Specifies an ODBC connect string
		sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",
			sDriver, lpFileName, lpFileName);
		
		//Create Excel database
		if( database.OpenEx(sSql, CDatabase::noOdbcDialog) )
		{
			m_pListCtrl->ExportAsExcel(database);

			//Total
			sSql.Format("CREATE TABLE Total (Type TEXT, Count NUMBER, Percentage TEXT)");
			database.ExecuteSQL(sSql);

			//Total File Count
			sSql.Format("INSERT INTO Total (Type, Count, Percentage) VALUES ('%s', %d, '%s')",
				_T("Total File Count:"), m_pTotalInfo->m_nTotalCount, _T("100%"));
			database.ExecuteSQL(sSql);

			//Total Lines
			sSql.Format("INSERT INTO Total (Type, Count, Percentage) VALUES ('%s', %d, '%s')",
				_T("Total Lines:"), m_pTotalInfo->m_nTotalLines, _T("100%"));
			database.ExecuteSQL(sSql);

			//Total Code
			sSql.Format("INSERT INTO Total (Type, Count, Percentage) VALUES ('%s', %d, '%s')",
				_T("Total Code:"), m_pTotalInfo->m_nTotalCodeLines, 
				CommonUtils::GetPercentStr(m_pTotalInfo->m_nTotalCodeLines, m_pTotalInfo->m_nTotalLines));
			database.ExecuteSQL(sSql);

			//Total Comment
			sSql.Format("INSERT INTO Total (Type, Count, Percentage) VALUES ('%s', %d, '%s')",
				_T("Total Comment:"), m_pTotalInfo->m_nTotalCommentLines, 
				CommonUtils::GetPercentStr(m_pTotalInfo->m_nTotalCommentLines, m_pTotalInfo->m_nTotalLines));
			database.ExecuteSQL(sSql);

			//Total Mixed
			sSql.Format("INSERT INTO Total (Type, Count, Percentage) VALUES ('%s', %d, '%s')",
				_T("Total Mixed:"), m_pTotalInfo->GetTotalMixedLines(), 
				CommonUtils::GetPercentStr(m_pTotalInfo->GetTotalMixedLines(), m_pTotalInfo->m_nTotalLines));
			database.ExecuteSQL(sSql);

			//Total Blank
			sSql.Format("INSERT INTO Total (Type, Count, Percentage) VALUES ('%s', %d, '%s')",
				_T("Total Blank:"), m_pTotalInfo->m_nTotalBlankLines,
				CommonUtils::GetPercentStr(m_pTotalInfo->m_nTotalBlankLines, m_pTotalInfo->m_nTotalLines));
			database.ExecuteSQL(sSql);
		}      
		
		//Close Database
		database.Close();

		bResult = TRUE;
	}
	CATCH_ALL(e)
	{
		TCHAR szCause[4096];
		CString msg;
		
		e->GetErrorMessage(szCause, 4096);	   
		AfxMessageBox(szCause, MB_OK | MB_ICONEXCLAMATION);
	}
	END_CATCH_ALL
	
	return bResult;
}

CXMLExporter::CXMLExporter(CResultListCtrl* pListCtrl, CTotalInfo* pTotalInfo) : m_pListCtrl(pListCtrl), m_pTotalInfo(pTotalInfo)
{
}

BOOL CXMLExporter::DoExport(LPCTSTR lpFileName)
{
	CString s;
    BOOL bResult = FALSE;

    TRY
    {
        CStdioFile cFile(lpFileName, CFile::modeCreate | CFile::modeWrite);

		CString sXSLFileName = CommonUtils::GetConfFilePath(_T("report_stylesheet.xsl"), GCFP_AUTO, SYS_APP()->m_szWorkDir);

		CString sXSLT;
		sXSLT.Format("<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>\n", sXSLFileName);
        
        
        // the header
        s.Format(
            // header
            "<?xml version=\"1.0\"?>\n"
            "\n"
            "<!--\n"
            "\n"
			//Product name
            " Statistics Result generated by %s\n"
            "\n"
            "\tlines-total:     count of all lines in the file\n"
            "\tlines-code:      count of lines that only have code\n"
            "\tlines-comments:  count of lines that only have comments\n"
            "\tlines-mixed:     count of lines that have both code and comments\n"
            "\tlines-blank:     count of lines without code or comments\n"
            "\n"
            "-->\n\n"       

            // style sheet
            "%s"

            // root
            "<statistics orgnization=\"%s\" product=\"%s\" date=\"%s\" version=\"%s\">\n",
            SZ_PRODUCT_NAME, sXSLT, SZ_ORGNIZATION_NAME, SZ_PRODUCT_NAME, 
			CommonUtils::GetCurrentTime(), SZ_VERSION_NAME);
        cFile.WriteString(s);

		int nRowCount = m_pListCtrl->GetItemCount();
		for(int i = 0; i < nRowCount; i++)
		{
			CFileInfo* pFileInfo = (CFileInfo*)m_pListCtrl->GetItemData(i);
			s.Format(
				"\t\t<file id=\"%s\">\n"
				"\t\t\t<path>%s</path>\n"
				"\t\t\t<name>%s</name>\n"
				"\t\t\t<ext>%s</ext>\n"
				"\t\t\t<lines-total>%d</lines-total>\n"
				"\t\t\t<lines-code>%d</lines-code>\n"
				"\t\t\t<lines-comments>%d</lines-comments>\n"
				"\t\t\t<lines-mixed>%d</lines-mixed>\n"
				"\t\t\t<lines-blank>%d</lines-blank>\n"
				"\t\t</file>\n",
				pFileInfo->m_sFullFileName,
				pFileInfo->m_sFilePath,
				pFileInfo->m_sFileName,
				pFileInfo->m_sFileExt,
				pFileInfo->m_nTotalLines,
				pFileInfo->m_nCodeLines,
				pFileInfo->m_nCommentLines,
				pFileInfo->GetMixedLines(),
				pFileInfo->m_nBlankLines);
			
			cFile.WriteString(s);
		}
		cFile.WriteString("</statistics>\n");
		cFile.Close();
		bResult = TRUE;
    }
	CATCH_ALL(e)
	{
		TCHAR szCause[4096];
		CString msg;
		
		e->GetErrorMessage(szCause, 4096);	   
		AfxMessageBox(szCause, MB_OK | MB_ICONEXCLAMATION);
	}
	END_CATCH_ALL
		
	return bResult;    
}


CHTMLExporter::CHTMLExporter(CResultListCtrl* pListCtrl, CTotalInfo* pTotalInfo) : 
	m_pListCtrl(pListCtrl), m_pTotalInfo(pTotalInfo)
{
}

BOOL CHTMLExporter::DoExport(LPCTSTR lpFileName)
{
	CString s;
    BOOL bResult = FALSE;
	
    TRY
    {
        CStdioFile cFile(lpFileName, CFile::modeCreate | CFile::modeWrite);     
        
        // the header: title
		FormatHtmlHeader(s, "Statistics Result", SZ_VERSION_NAME);
        cFile.WriteString(s);
		
		//the content header
		FormatContentHeader(s, "Statistics Result", SZ_PRODUCT_NAME);
		cFile.WriteString(s);

		//Summary Header
		CString szSummaryInfo;
		szSummaryInfo.Format(_T("Totally %d files"), m_pTotalInfo->m_nTotalCount);
		FormatSummuryHeader(s, _T("Summary:"), szSummaryInfo);
		cFile.WriteString(s);

		//Summary info
		FormatSummaryItemInfo(s, "Total Lines:", m_pTotalInfo->m_nTotalLines, "100%", RGB(127, 0, 85));
		cFile.WriteString(s);

		FormatSummaryItemInfo(s, "Total Code:", m_pTotalInfo->m_nTotalCodeLines, 
			CommonUtils::GetPercentStr(m_pTotalInfo->m_nTotalCodeLines, m_pTotalInfo->m_nTotalLines), RGB(255, 128, 0));
		cFile.WriteString(s);

		FormatSummaryItemInfo(s, "Total Comment:", m_pTotalInfo->m_nTotalCommentLines, 
			CommonUtils::GetPercentStr(m_pTotalInfo->m_nTotalCommentLines, m_pTotalInfo->m_nTotalLines), RGB(0, 128, 0));
		cFile.WriteString(s);

		FormatSummaryItemInfo(s, "Total Mixed:", m_pTotalInfo->GetTotalMixedLines(), 
			CommonUtils::GetPercentStr(m_pTotalInfo->GetTotalMixedLines(), m_pTotalInfo->m_nTotalLines), RGB(0, 128, 144));
		cFile.WriteString(s);

		FormatSummaryItemInfo(s, "Total Blank:", m_pTotalInfo->m_nTotalBlankLines,
			CommonUtils::GetPercentStr(m_pTotalInfo->m_nTotalBlankLines, m_pTotalInfo->m_nTotalLines), RGB(128, 255, 255));
		cFile.WriteString(s);

		//Summary tail
		FormatSummuryTail(s);
		cFile.WriteString(s);

		//Detail header
		FormatDetailHeader(s, _T("Detailed:"));
		cFile.WriteString(s);

		//Detail Info
		int nRowCount = m_pListCtrl->GetItemCount();
		for(int i = 0; i < nRowCount; i++)
		{
			CFileInfo* pFileInfo = (CFileInfo*)m_pListCtrl->GetItemData(i);
			
			s.Format(
				"\t\t<TR>\n\t\t\t<TD>%s</TD>\n\t\t\t<TD>%d</TD>\n\t\t\t<TD>%d(%s)</TD>\n\t\t\t<TD>%d(%s)</TD>\n"
				"\t\t\t<TD>%d(%s)</TD>\n\t\t\t<TD>%d(%s)</TD>\n\t\t\t<TD>%s</TD>\n\t\t</TR>\n",
				pFileInfo->m_sFileName, pFileInfo->m_nTotalLines, pFileInfo->m_nCodeLines,
				CommonUtils::GetPercentStr(pFileInfo->m_nCodeLines, pFileInfo->m_nTotalLines),
				pFileInfo->m_nCommentLines, CommonUtils::GetPercentStr(pFileInfo->m_nCommentLines, pFileInfo->m_nTotalLines),
				pFileInfo->GetMixedLines(), CommonUtils::GetPercentStr(pFileInfo->GetMixedLines(), pFileInfo->m_nTotalLines),
				pFileInfo->m_nBlankLines, CommonUtils::GetPercentStr(pFileInfo->m_nBlankLines, pFileInfo->m_nTotalLines),
				pFileInfo->m_sFilePath
				);
			cFile.WriteString(s);
		}
		FormatDetailLastItem(s);
		cFile.WriteString(s);

		//Detailed Table tail
		FormatDetailTail(s);
		cFile.WriteString(s);

		//Content Tail
		FormatHtmlTail(s, SZ_ORGNIZATION_NAME);
		cFile.WriteString(s);

		cFile.Close();
		bResult = TRUE;
    }
	CATCH_ALL(e)
	{
		TCHAR szCause[4096];
		CString msg;
		
		e->GetErrorMessage(szCause, 4096);	   
		AfxMessageBox(szCause, MB_OK | MB_ICONEXCLAMATION);
	}
	END_CATCH_ALL
		
	return bResult;
}

CString CHTMLExporter::GetHtmlColorStr(COLORREF cr)
{
	CString str;
	str.Format("#%02X%02X%02X", GetRValue(cr), GetGValue(cr), GetBValue(cr));
	return str;
}


void CHTMLExporter::FormatHtmlHeader(CString& s, LPCTSTR lpTitle, LPCTSTR lpVersionInfo)
{
	// the header
	s.Format(
		"<HTML>\n"
		"\t<HEAD>\n"
		"\t\t<META HTTP-EQUIV=\"CONTENT-TYPE\" CONTENT=\"TEXT/HTML; CHARSET=UTF-8\" />\n"
		"\t\t<TITLE>%s - %s</TITLE>\n"
		"\t<style type=\"text/css\">\n"
		"\t\t.content-header {\n"
		"\t\t\tcolor: #000000;\n"
		"\t\t\tfont-size: 24pt;\n"
		"\t\t\tfont-weight: bold;\n"
		"\t\t}\n"
		"\t\t.detail-item { \n"
		"\t\t\tcolor: #000090;\n"
		"\t\t}\n"
		"\t\t.detail-header { \n"
		"\t\t\tfont-family: Helvetica;\n"
		"\t\t\tfont-size: 12pt;\n"
		"\t\t\tfont-weight: bold;\n"
		"\t\t\ttext-align: center;\n"
		"\t\t\tcolor: #000040;\n"
		"\t\t\tbackground-color: #A8B4D8;\n"
		"\t\t}\n"
		"\t\t.detail-total { \n"
		"\t\t\tfont-weight: bold;\n"
		"\t\t\tcolor: #002020;\n"
		"\t\t\tbackground-color: #90B498;\n"
		"\t\t}\n"
		"\t\t.summary-title {\n"
		"\t\t\tcolor: #900000;\n"
		"\t\t\tfont-size: 14pt;\n"
		"\t\t\tfont-weight: bold;\n"
		"\t\t}\n"
		"\t\t.summary-info {\n"
		"\t\t\tfont-size: 12pt;\n"
		"\t\t\tfont-weight: bold;\n"
		"\t\t}\n"
		"\t</style>\n"
		"\t</HEAD>\n"
		"\n"
		,
		lpTitle, lpVersionInfo
		);
}

void CHTMLExporter::FormatHtmlTail(CString& s, LPCTSTR lpCompany)
{
	s.Format(
		"<HR/>\n"
		"<FONT SIZE=\"3\">Note 1: Mixed line means Code line with Comment.</FONT>\n"
		"<BR/><BR/>"
		"<TABLE NOSAVE=\"\" BORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"0\" COLS=\"1\" WIDTH=\"100%%\">\n"
		"\t<TR ALIGN=\"RIGHT\"><TD><FONT SIZE=\"2\">(C) %s, Since 2011. All Rights Reserved.</FONT></TD></TR>\n"
		"</TABLE>\n\n"
		"\t</BODY>\n"
		"</HTML>\n",
		lpCompany);
}

void CHTMLExporter::FormatContentHeader(CString& s, LPCTSTR lpContentHeader, LPCTSTR lpProduct)
{
	s.Format(
		"\t<BODY>\n\n"
		"\t\t<TABLE NOSAVE=\"\"  BORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"0\" WIDTH=\"100%%\">\n"
		"\t\t<TBODY><TR NOSAVE=\"\">\n"
		"\t\t\t<TD COLSPAN=\"2\" ROWSPAN=\"2\" NOSAVE=\"\" CLASS=\"content-header\"><B>%s</B></TD>\n"
		"\t\t\t<TD ALIGN=\"RIGHT\">Genrated by <b><i>%s v%s</i></b><BR><b>%s</b></TD>\n"
		"\t\t</TR></TBODY>\n"
		"\t\t</TABLE><HR/>\n\n"
		,
		lpContentHeader, lpProduct, SZ_VERSION_NAME,
		CommonUtils::GetCurrentTime()
		);
}

void CHTMLExporter::FormatSummuryHeader(CString& s, LPCTSTR lpSummuryTitle, LPCTSTR lpSummaryInfo)
{
	s.Format(
		"<TABLE border=\"0\" WIDTH=\"100%%\">\n"
		"\t<TR><TD>\n"
		"\t\t<TABLE border=\"0\">\n"
		"\t\t\t<TR><TD class=\"summary-title\">%s&nbsp;&nbsp;</TD><TD class=\"summary-info\">%s</TD></TR>\n"
		"\t\t</TABLE>\n"
		"\t</TD></TR>\n"
		"\t<TR><TD>\n"
		"\t\t<TABLE BORDER=\"1\" CELLSPACING=\"0\" COLS=\"3\" WIDTH=\"100%%\">\n"
		"\t\t\t<TR CLASS=\"detail-header\">\n"
		"\t\t\t\t<TD WIDTH=\"20%%\">Type</TD>\n"
		"\t\t\t\t<TD WIDTH=\"10%%\">Count</TD>\n"
		"\t\t\t\t<TD WIDTH=\"70%%\">Percentage(%%)</TD>\n"
		"\t\t\t</TR>\n"
		,
		lpSummuryTitle, lpSummaryInfo);
}
void CHTMLExporter::FormatSummaryItemInfo(CString& s, LPCTSTR lpType, int nCount, LPCTSTR lpPercent, COLORREF crBkColor)
{
	s.Format(
		"\t\t<TR CLASS=\"detail-total\">\n"
		"\t\t\t<TD>%s</TD>\n"
		"\t\t\t<TD ALIGN=\"RIGHT\">%d</TD>\n"
		"\t\t\t<TD ALIGN=\"RIGHT\">\t\t\n"
		"\t\t\t\t<TABLE border=\"0\" WIDTH=\"100%%\" CLASS=\"detail-total\">\n"
		"\t\t\t\t\t<TR>\n"
		"\t\t\t\t\t\t<TD WIDTH=80 ALIGN=\"RIGHT\">%s</TD>\n"
		"\t\t\t\t\t\t<TD ALIGN=\"LEFT\">\n"
		"\t\t\t\t\t\t\t<TABLE BORDER=\"0\" CELLSPACING=\"0\" CELLPADDING=\"0\" WIDTH=\"%s\"><TR BGCOLOR=\"%s\"><TD>&nbsp;</TD></TR>\n"
		"\t\t\t\t\t\t\t</TABLE>\n"
		"\t\t\t\t\t\t</TD>\n"
		"\t\t\t\t\t</TR>\n"
		"\t\t\t\t</TABLE>\n"
		"\t\t\t</TD>\n"
		"\t\t</TR>\n",
		lpType, nCount, lpPercent, lpPercent, GetHtmlColorStr(crBkColor)
		);
}

void CHTMLExporter::FormatSummuryTail(CString& s)
{
	s.Format(
		"\t\t</TABLE>\n"
		"\t</TD></TR>\n"
		"</TABLE>\n\n"
		"<BR/><HR/>\n\n");
}

void CHTMLExporter::FormatDetailHeader(CString& s, LPCTSTR lpTitle)
{
	s.Format(
		"<TABLE border=\"0\" WIDTH=\"100%%\">\n\n"
		"\t<TR><TD>\n"
		"\t\t<TABLE border=\"0\" WIDTH=\"100%%\">\n"
		"\t\t\t<TR><TD class=\"summary-title\">%s</TD><TD class=\"summary-info\"></TD></TR></TABLE>\n"
		"\t</TD></TR>\n"
		"\t<TR><TD>\t\n\n"
		"\t<TABLE BORDER=\"0\" WIDTH=100%%>\n"
		"\t\t<TR><TD>\n\n"
		"\t\t<TABLE BORDER=\"1\" CELLPADDING=\"2\" CELLSPACING=\"0\" CLASS=\"detail-item\" WIDTH=\"100%%\">\n"
		"\t\t<TR CLASS=\"detail-header\">\n"
		"\t\t\t<TD ROWSPAN=\"2\">File Name</TD>\n"
		"\t\t\t<TD COLSPAN=\"5\">Lines</TD>\n"
		"\t\t\t<TD ROWSPAN=\"2\">Path</TD>\n"
		"\t\t</TR>\n\n"
		"\t\t<TR CLASS=\"detail-header\">\n"
		"\t\t\t<TD>Total</TD>\n"
		"\t\t\t<TD>Code</TD>\n"
		"\t\t\t<TD>Comment</TD>\n"
		"\t\t\t<TD>Mixed</TD>\n"
		"\t\t\t<TD>Blank</TD>\n"
		"\t\t</TR>\n"
		,
		lpTitle);
}
void CHTMLExporter::FormatDetailTail(CString& s)
{
	s.Format("\t\t</TABLE>\n\n\t\t</TD></TR>\n\t</TABLE>\n<BR/>\n</TABLE>\n\n");
}

void CHTMLExporter::FormatDetailLastItem(CString& s)
{
	s.Format(
		"\t\t<TR CLASS=\"detail-total\">\n"
		"\t\t\t<TD>Total:&nbsp;</TD>\n"
		"\t\t\t<TD>%d</TD>\n"
		"\t\t\t<TD>%d(%s)</TD>\n"
		"\t\t\t<TD>%d(%s)</TD>\n"
		"\t\t\t<TD>%d(%s)</TD>\n"
		"\t\t\t<TD>%d(%s)</TD>\n"
		"\t\t\t<TD>&nbsp;</TD>\n"
		"\t\t</TR>\n",
		m_pTotalInfo->m_nTotalLines, 
		m_pTotalInfo->m_nTotalCodeLines, 
		CommonUtils::GetPercentStr(m_pTotalInfo->m_nTotalCodeLines, m_pTotalInfo->m_nTotalLines),
		m_pTotalInfo->m_nTotalCommentLines, 
		CommonUtils::GetPercentStr(m_pTotalInfo->m_nTotalCommentLines, m_pTotalInfo->m_nTotalLines),
		m_pTotalInfo->GetTotalMixedLines(), 
		CommonUtils::GetPercentStr(m_pTotalInfo->GetTotalMixedLines(), m_pTotalInfo->m_nTotalLines),
		m_pTotalInfo->m_nTotalBlankLines,
		CommonUtils::GetPercentStr(m_pTotalInfo->m_nTotalBlankLines, m_pTotalInfo->m_nTotalLines)
		);
}
