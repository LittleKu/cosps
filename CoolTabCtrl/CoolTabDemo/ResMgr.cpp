// ResMgr.cpp: implementation of the ResMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoolTabDemo.h"
#include "ResMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResMgr::CResMgr()
{

}

CResMgr::~CResMgr()
{

}

BOOL CResMgr::LoadImage(LPCSTR lpcstrImagePath, CBitmap &cBitmap)
{
	HBITMAP hb = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), lpcstrImagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);	
	if(hb != NULL)
	{
		return cBitmap.Attach(hb);
	}
	return FALSE;
}
BOOL CResMgr::Init(LPCSTR lpcBaseDir)
{
	//Main Tool Bar
	CString tempPath = lpcBaseDir;
	BOOL ret = FALSE;
	do 
	{
		if( !(ret = LoadImage(tempPath + "\\main_button_bar\\background.bmp", m_mainBkg)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\main_button_bar\\normal.bmp", m_mainNormal)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\main_button_bar\\highlight.bmp", m_mainHighlight)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\main_button_bar\\one_clicked.bmp", m_mainClicked1)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\main_button_bar\\two_clicked.bmp", m_mainClicked2)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\main_button_bar\\three_clicked.bmp", m_mainClicked3)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\tabcontrol\\background.bmp", m_tiBkg)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\tabcontrol\\left.bmp", m_tiLeft)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\tabcontrol\\middle.bmp", m_tiMid)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\tabcontrol\\right.bmp", m_tiRight)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\tabcontrol\\seperator.BMP", m_tiSep)) )
		{
			break;
		}

		if( !(ret = LoadImage(tempPath + "\\main_buttonbar_dps\\main_buttonbar_normal.bmp", m_mainBkgDps)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\main_buttonbar_dps\\main_buttonbar_normal.bmp", m_mainNormalDps)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\main_buttonbar_dps\\main_buttonbar_highlight.bmp", m_mainHighlightDps)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\main_buttonbar_dps\\main_buttonbar_clicked.bmp", m_mainClickedDps)) )
		{
			break;
		}
		if( !(ret = LoadImage(tempPath + "\\main_buttonbar_dps\\main_buttonbar_disabled.bmp", m_mainDisabledDps)) )
		{
			break;
		}
		CBitmap m_mainBkgDps;
		CBitmap m_mainNormalDps;
		CBitmap m_mainHighlightDps;
		CBitmap m_mainClickedDps;
		CBitmap m_mainDisabledDps;

	} while ( FALSE );
	
	return ret;
}