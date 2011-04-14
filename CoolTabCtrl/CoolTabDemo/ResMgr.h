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
private:
	class CGarbo
	{
	public:
		~CGarbo()
		{
			if(CResMgr::m_instance != NULL)
			{
				delete CResMgr::m_instance;
				CResMgr::m_instance = NULL;
			}
			TRACE0("\n\nCResMgr destruction\n\n");
		}
	};
	friend class CGarbo;
	CResMgr();	
public:
	static CResMgr* GetInstance();
	virtual ~CResMgr();
	BOOL Load(LPCSTR lpcstrSkinFile);
	CBitmap* GetBitmap(LPCTSTR lpcstrImagePaht);
	CImgBtnGroup* GetImageButtonGroup(LPCTSTR lpstrImgBtnGroup);	
private:
	void ToPath(LPCSTR skinFolder, LPCSTR subFolder, LPCSTR imageName, CString& ret);
	void CResMgr::LoadImage(CString& imagePath);
	CImgBtn* Init(TiXmlElement *pElement);
private:
	CMapStringToPtr m_pathToBitmapMap;
	CMapStringToPtr m_btnGroupMap;		
	static CResMgr* m_instance;
	static CString BUTTON_STATUS_NAME[BS_MAX];
	static CGarbo m_garbo;
};

#endif // !defined(AFX_RESMGR_H__C786C9AB_FA68_4D06_B123_74141A600158__INCLUDED_)
