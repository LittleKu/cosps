// ResMgr.cpp: implementation of the CResMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static BmpResInfo brInfos[] = 
{
	{BR_ACB_BKG_NORMAL, _T("combobox_bkg_normal.bmp"), NULL},
	{BR_ACB_BKG_HOT, _T("combobox_bkg_hot.bmp"), NULL}
};

static BmpResInfo stateBri[] = 
{
	{BR_EXPAND, _T("expanded.bmp"), NULL},
	{BR_COLLAPSE, _T("collapsed.bmp"), NULL}
};

static BmpResInfo normalBri[] = 
{
	{BR_TEMP1, _T("temp1.bmp"), NULL},
	{BR_TEMP2, _T("temp2.bmp"), NULL},
	{BR_TEMP3, _T("temp3.bmp"), NULL},
	{BR_TEMP4, _T("temp4.bmp"), NULL},
	{BR_TEMP5, _T("temp5.bmp"), NULL}
};

CResMgr::CResMgr() : m_bLoaded(FALSE)
{
}

CResMgr::~CResMgr()
{
	UnLoad();
}

int CResMgr::Load(LPCTSTR lpszResDir)
{
	if(m_bLoaded)
	{
		return 0;
	}

	CString szBmpPath;

	int nCount = sizeof(brInfos) / sizeof(brInfos[0]);
	for(int i = 0; i < nCount; i++)
	{
		if(brInfos[i].nID < 0 || brInfos[i].lpszPath == NULL)
		{
			continue;
		}

		szBmpPath.Format(_T("%s\\%s"), ((lpszResDir == NULL) ? _T(".") : lpszResDir), brInfos[i].lpszPath);
		
		brInfos[i].hBitmap = (HBITMAP)::LoadImage(NULL, szBmpPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if(brInfos[i].hBitmap == NULL)
		{
			AfxTrace(_T("Failed to load image file %s: errorcode=%d\n"), (LPCTSTR)szBmpPath, GetLastError());
		}
	}

	LoadToImageList(&m_stateIL, stateBri, sizeof(stateBri)/sizeof(stateBri[0]), lpszResDir);
	LoadToImageList(&m_normalIL, normalBri, sizeof(normalBri)/sizeof(normalBri[0]), lpszResDir);

	m_bLoaded = TRUE;

	return 0;
}

int CResMgr::UnLoad()
{
	if(m_bLoaded)
	{
		int nCount = sizeof(brInfos) / sizeof(brInfos[0]);
		for(int i = 0; i < nCount; i++)
		{
			if(brInfos[i].hBitmap != NULL)
			{
				DeleteObject(brInfos[i].hBitmap);
				brInfos[i].hBitmap = NULL;
			}
		}
		
		m_stateIL.DeleteImageList();
		m_normalIL.DeleteImageList();

		m_bLoaded = FALSE;
	}
	return 0;
}

HBITMAP CResMgr::GetBitmap(int nID)
{
	int nCount = sizeof(brInfos) / sizeof(brInfos[0]);
	if(nID < 0 || nID >= nCount)
	{
		return NULL;
	}
	return brInfos[nID].hBitmap;
}

int CResMgr::LoadToImageList(CImageList* pImageList, BmpResInfo* pBri, int nCount, LPCTSTR lpszResDir)
{
	BOOL bCreated = FALSE;

	CString szBmpPath;
	for(int i = 0; i < nCount; i++)
	{
		if(pBri[i].lpszPath == NULL)
		{
			continue;
		}
		
		szBmpPath.Format(_T("%s\\%s"), ((lpszResDir == NULL) ? _T(".") : lpszResDir), pBri[i].lpszPath);
		
		HBITMAP hbmp = (HBITMAP)::LoadImage(NULL, szBmpPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if(hbmp == NULL)
		{
			AfxTrace(_T("Failed to load image file %s: errorcode=%d\n"), (LPCTSTR)szBmpPath, GetLastError());
		}

		if(!bCreated)
		{
			BITMAP bmp;
			::GetObject(hbmp, sizeof(bmp), &bmp);

			pImageList->Create(bmp.bmWidth, bmp.bmHeight, ILC_COLOR24 | ILC_MASK, nCount, nCount);
			pImageList->SetBkColor(CLR_NONE);
			bCreated = TRUE;
		}

		pImageList->Add(CBitmap::FromHandle(hbmp), RGB(255, 255, 255));
	}

	return 0;
}

CImageList* CResMgr::GetImageList(int nID)
{
	if(nID == ILID_STATE)
	{
		return &m_stateIL;
	}
	else if(nID == ILID_NORMAL)
	{
		return &m_normalIL;
	}
	else
	{
		return NULL;
	}
}
