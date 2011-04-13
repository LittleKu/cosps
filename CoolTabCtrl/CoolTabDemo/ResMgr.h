// ResMgr.h: interface for the ResMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESMGR_H__C786C9AB_FA68_4D06_B123_74141A600158__INCLUDED_)
#define AFX_RESMGR_H__C786C9AB_FA68_4D06_B123_74141A600158__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum EButtonStatus
{
	BS_NORMAL = 0,
	BS_HIGHLIGHT,
	BS_CLICKED,
	BS_DISABLED,
	BS_MAX
};

#define BS_MIN BS_NORMAL

typedef struct tagIMAGE_BUTTON
{
	UINT id;
	CRect rect;
	CString sTips;
}IMAGE_BUTTON;


typedef CList<IMAGE_BUTTON*, IMAGE_BUTTON*> CImgBtnPtrList;

class ImageButtonGroup
{
public:
	ImageButtonGroup() {}
	virtual ~ImageButtonGroup() 
	{
		POSITION pos = imgBtnPtrList.GetHeadPosition();
		IMAGE_BUTTON* pImageButton;
		while(pos != NULL)
		{
			pImageButton = (IMAGE_BUTTON*)imgBtnPtrList.GetNext(pos);
			ASSERT(pImageButton);
			delete pImageButton;
		}
		imgBtnPtrList.RemoveAll();
	}
public:
	CString btnBitmapPath[BS_MAX];
	CImgBtnPtrList imgBtnPtrList;
};

class TiXmlElement;

class CResMgr  
{
	friend class CCoolTabDemoDlg;
public:
	CResMgr();
	virtual ~CResMgr();
	BOOL Init(LPCSTR lpcBaseDir);
	BOOL LoadImage(LPCSTR lpcstrImagePath, CBitmap &cBitmap);
	BOOL Load(LPCSTR lpcstrSkinFile);
private:
	void ToPath(LPCSTR skinFolder, LPCSTR subFolder, LPCSTR imageName, CString& ret);
	void CResMgr::LoadImage(CString& imagePath);
	IMAGE_BUTTON* Init(TiXmlElement *pElement);
	
	CBitmap m_mainBkg;
	CBitmap m_mainNormal;
	CBitmap m_mainHighlight;
	CBitmap m_mainClicked1;
	CBitmap m_mainClicked2;
	CBitmap m_mainClicked3;

	CBitmap m_tiBkg;
	CBitmap m_tiLeft;
	CBitmap m_tiMid;
	CBitmap m_tiRight;
	CBitmap m_tiSep;

	CBitmap m_mainBkgDps;
	CBitmap m_mainNormalDps;
	CBitmap m_mainHighlightDps;
	CBitmap m_mainClickedDps;
	CBitmap m_mainDisabledDps;

	CMapStringToPtr m_pathToBitmapMap;
	CMapStringToPtr m_btnGroupMap;
	static CString BUTTON_STATUS_NAME[BS_MAX];
};

#endif // !defined(AFX_RESMGR_H__C786C9AB_FA68_4D06_B123_74141A600158__INCLUDED_)
