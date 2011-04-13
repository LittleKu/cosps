// ResMgr.cpp: implementation of the ResMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoolTabDemo.h"
#include "ResMgr.h"
#include "tinyxml.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString CResMgr::BUTTON_STATUS_NAME[BS_MAX] = {"normal", "highlighted", "clicked", "disabled"};

CResMgr::CResMgr()
{

}

CResMgr::~CResMgr()
{
	POSITION pos = NULL;
	CString key;
	void* pValue;

	CBitmap* pBitmap = NULL;
	pos = m_pathToBitmapMap.GetStartPosition();		
	while (pos != NULL)
	{
		m_pathToBitmapMap.GetNextAssoc( pos, key, pValue );
		
		if(pValue != NULL)
		{
			pBitmap = (CBitmap*)pValue;
			delete pBitmap;
		}
	}
	m_pathToBitmapMap.RemoveAll();

	ImageButtonGroup* pImgBtnGroup;
	pos = m_btnGroupMap.GetStartPosition();
	while (pos != NULL)
	{
		m_btnGroupMap.GetNextAssoc( pos, key, pValue );
		
		if(pValue != NULL)
		{
			pImgBtnGroup = (ImageButtonGroup*)pValue;
			delete pImgBtnGroup;
		}
	}
	m_btnGroupMap.RemoveAll();
}

BOOL CResMgr::LoadImage(LPCSTR lpcstrImagePath, CBitmap &cBitmap)
{
	HBITMAP hb = (HBITMAP)::LoadImage(/*AfxGetInstanceHandle()*/NULL, lpcstrImagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);	
	if(hb != NULL)
	{
		return cBitmap.Attach(hb);
	}
	return FALSE;
}

void CResMgr::ToPath(LPCSTR skinFolder, LPCSTR subFolder, LPCSTR imageName, CString& ret)
{
	CString imagePath;
	if(skinFolder != NULL && skinFolder[0] != '\0')
	{
		imagePath += skinFolder;
	}
	if(subFolder != NULL && subFolder[0] != '\0')
	{
		imagePath += "\\";
		imagePath += subFolder;
	}
	imagePath += "\\";
	imagePath += imageName;

	ret = imagePath;
}

void CResMgr::LoadImage(CString& imagePath)
{
	HBITMAP hb = (HBITMAP)::LoadImage(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if(hb == NULL)
	{
		TRACE1("Failed to load image file %s\n", imagePath);
		return;
	}
	CBitmap* pBitmap = new CBitmap();
	pBitmap->Attach(hb);
	
	m_pathToBitmapMap.SetAt(imagePath, pBitmap);
}

IMAGE_BUTTON* CResMgr::Init(TiXmlElement *pElement)
{
	if(pElement == NULL)
	{
		return NULL;
	}

	const char* lpcsValue = 0;
	int nValue;

	IMAGE_BUTTON* pImageBtn = new IMAGE_BUTTON;

	lpcsValue = pElement->Attribute("id", &nValue);
	pImageBtn->id = nValue;

	lpcsValue = pElement->Attribute("x", &nValue);
	pImageBtn->rect.left = nValue;

	lpcsValue = pElement->Attribute("y", &nValue);
	pImageBtn->rect.top = nValue;

	lpcsValue = pElement->Attribute("width", &nValue);
	pImageBtn->rect.right = pImageBtn->rect.left + nValue;

	lpcsValue = pElement->Attribute("height", &nValue);
	pImageBtn->rect.bottom = pImageBtn->rect.top + nValue;

	lpcsValue = pElement->Attribute("tips");
	pImageBtn->sTips = lpcsValue;
	
	return pImageBtn;
}

BOOL CResMgr::Load(LPCSTR lpcstrSkinFile)
{
	TiXmlDocument doc( lpcstrSkinFile );

	bool loadOkay = doc.LoadFile();	
	if ( !loadOkay )
	{
		TRACE1("Could not load test file 'demotest.xml'. Error='%s'. Exiting.\n", doc.ErrorDesc());
		return FALSE;
	}

	TiXmlNode *node = 0, *sub_node = 0;
	TiXmlElement *pElement = 0, *sub_element = 0;
	const char* lpcsValue = 0;

	node = doc.FirstChild( "skin" );
	ASSERT( node );
	pElement = node->ToElement();
	ASSERT(pElement);

	CString skinFolder = "skins\\";
	skinFolder += pElement->Attribute("folder");

	CString subFolder;
	CBitmap* pBitmap = NULL;
	CString imagePath;

	int i;
	for(node = node->FirstChild(); node != NULL; node = node->NextSibling())
	{
		pElement = node->ToElement();
		ASSERT(pElement);

		subFolder = pElement->Attribute("folder");


		ImageButtonGroup* pImgBtnGroup = new ImageButtonGroup;
		for(i = BS_MIN; i < BS_MAX; i++)
		{
			ToPath(skinFolder, subFolder, pElement->Attribute(BUTTON_STATUS_NAME[i]), imagePath);
			pImgBtnGroup->btnBitmapPath[i] = imagePath;
			LoadImage(imagePath);
		}

		for(sub_node = node->FirstChild(); sub_node != NULL; sub_node = sub_node->NextSibling())
		{
			sub_element = sub_node->ToElement();
			ASSERT(sub_element);

			lpcsValue = sub_element->Value();
			if(strcmp(lpcsValue, "button") != 0)
			{
				TRACE1("sub_element value = %s\n", lpcsValue);
				continue;
			}

			IMAGE_BUTTON* pImageBtn = Init(sub_element);
			pImgBtnGroup->imgBtnPtrList.AddTail(pImageBtn);
		}

		m_btnGroupMap.SetAt(pElement->Value(), pImgBtnGroup);
	}
	
	return TRUE;
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