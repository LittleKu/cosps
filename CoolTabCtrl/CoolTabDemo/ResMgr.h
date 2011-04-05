// ResMgr.h: interface for the ResMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESMGR_H__C786C9AB_FA68_4D06_B123_74141A600158__INCLUDED_)
#define AFX_RESMGR_H__C786C9AB_FA68_4D06_B123_74141A600158__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResMgr  
{
	friend class CCoolTabDemoDlg;
public:
	CResMgr();
	virtual ~CResMgr();
	BOOL Init(LPCSTR lpcBaseDir);
	BOOL LoadImage(LPCSTR lpcstrImagePath, CBitmap &cBitmap);
private:
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
};

#endif // !defined(AFX_RESMGR_H__C786C9AB_FA68_4D06_B123_74141A600158__INCLUDED_)
