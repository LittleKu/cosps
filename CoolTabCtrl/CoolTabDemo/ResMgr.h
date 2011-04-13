// ResMgr.h: interface for the ResMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESMGR_H__C786C9AB_FA68_4D06_B123_74141A600158__INCLUDED_)
#define AFX_RESMGR_H__C786C9AB_FA68_4D06_B123_74141A600158__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TiXmlElement;

class CResMgr  
{
public:
	CResMgr();
	virtual ~CResMgr();
	BOOL LoadImage(LPCSTR lpcstrImagePath, CBitmap &cBitmap);
	BOOL Load(LPCSTR lpcstrSkinFile);
public:
	CBitmap* GetBitmap(LPCTSTR lpcstrImagePaht);
	ImageButtonGroup* GetImageButtonGroup(LPCTSTR lpstrImgBtnGroup);
	static CResMgr* GetInstance();
private:
	void ToPath(LPCSTR skinFolder, LPCSTR subFolder, LPCSTR imageName, CString& ret);
	void CResMgr::LoadImage(CString& imagePath);
	IMAGE_BUTTON* Init(TiXmlElement *pElement);

private:
	CMapStringToPtr m_pathToBitmapMap;
	CMapStringToPtr m_btnGroupMap;
	static CString BUTTON_STATUS_NAME[BS_MAX];
	static CResMgr* m_instance;
};

#endif // !defined(AFX_RESMGR_H__C786C9AB_FA68_4D06_B123_74141A600158__INCLUDED_)
