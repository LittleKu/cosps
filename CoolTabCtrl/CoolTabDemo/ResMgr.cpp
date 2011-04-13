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
CResMgr* CResMgr::m_instance = NULL;

CResMgr* CResMgr::GetInstance()
{
	if(m_instance == NULL)
	{
		m_instance = new CResMgr;
	}
	return m_instance;
}

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

CBitmap* CResMgr::GetBitmap(LPCTSTR lpcstrImagePath)
{
	void* value = NULL;
	if(m_pathToBitmapMap.Lookup(lpcstrImagePath, value))
	{
		return (CBitmap*)value;
	}
	return NULL;
}

ImageButtonGroup* CResMgr::GetImageButtonGroup(LPCTSTR lpstrImgBtnGroup)
{
	void* value = NULL;
	if(m_btnGroupMap.Lookup(lpstrImgBtnGroup, value))
	{
		return (ImageButtonGroup*)value;
	}
	return NULL;
}
