// ResMgr.h: interface for the CResMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESMGR_H__68083A86_7BDE_49AC_A57F_2A40CC8A7B82__INCLUDED_)
#define AFX_RESMGR_H__68083A86_7BDE_49AC_A57F_2A40CC8A7B82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct
{
	int nID;
	LPCTSTR lpszPath;
	HBITMAP hBitmap;
} BmpResInfo;

enum BmpResID
{
	BR_ACB_BKG_NORMAL,
	BR_ACB_BKG_HOT,
	BR_EXPAND,
	BR_COLLAPSE,
	BR_TEMP1,
	BR_TEMP2,
	BR_TEMP3,
	BR_TEMP4,
	BR_TEMP5
};

enum ImageListID
{
	ILID_FIRST = 0,
	ILID_STATE = 0,
	ILID_NORMAL,
	ILID_COUNT
};

class CResMgr  
{
public:
	CResMgr();
	virtual ~CResMgr();

	virtual int Load(LPCTSTR lpszResDir);
	virtual int UnLoad();

	virtual HBITMAP GetBitmap(int nID);
	virtual CImageList* GetImageList(int nID);

private:
	int LoadToImageList(CImageList* pImageList, BmpResInfo* pBri, int nCount, LPCTSTR lpszResDir);

private:
	BOOL m_bLoaded;
	CImageList m_stateIL;
	CImageList m_normalIL;
};

#endif // !defined(AFX_RESMGR_H__68083A86_7BDE_49AC_A57F_2A40CC8A7B82__INCLUDED_)
