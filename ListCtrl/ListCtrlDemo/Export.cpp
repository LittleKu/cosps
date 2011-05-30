#include "StdAfx.h"
#include "Export.h"
#include "ResultListCtrl.h"

CCSVExporter::CCSVExporter(CListCtrl* pListCtrl) : m_pListCtrl(pListCtrl)
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



CXLSExporter::CXLSExporter(CResultListCtrl* pListCtrl) : m_pListCtrl(pListCtrl)
{

}

BOOL CXLSExporter::DoExport(LPCTSTR lpFileName)
{
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


