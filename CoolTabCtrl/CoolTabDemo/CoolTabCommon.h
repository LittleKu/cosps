#ifndef _COOL_TAB_COMMON_H_
#define _COOL_TAB_COMMON_H_

#pragma once

enum EButtonStatus
{
	BS_NORMAL = 0,
	BS_HIGHLIGHT,
	BS_CLICKED,
	BS_DISABLED,
	BS_MAX
};

#define BS_MIN BS_NORMAL


class CImgBtn
{
public:
	UINT id;
	CRect rect;
	CString sTips;	
};

typedef CArray<CImgBtn*, CImgBtn*> CImgBtnArray;

class CImgBtnGroup
{
public:
	CImgBtnGroup() {}
	virtual ~CImgBtnGroup() 
	{
 		CImgBtn* pImgBtn;
		int i, size = m_imgBtnArray.GetSize();
		for(i = 0; i < size; i++)
		{
			pImgBtn = (CImgBtn*)m_imgBtnArray.GetAt(i);
			ASSERT(pImgBtn);
			delete pImgBtn;
		}
		m_imgBtnArray.RemoveAll();
	}
public:
	CBitmap* m_pBitmaps[BS_MAX];
	CImgBtnArray m_imgBtnArray;
};

enum EPostion
{
	PS_LEFT = 0,
	PS_MID,
	PS_RIGHT,
	PS_MAX
};

enum ETabBtnPartType
{
	TB_PT_BACKGROUND = 0,
	TB_PT_SEPERATOR,
	TB_PT_MAX
};

#define PS_MIN PS_LEFT
#define TB_PT_MIN TB_PT_BACKGROUND

class CImgTabBtn
{
public:
	CImgTabBtn();
public:
	CBitmap* m_pTabPart[TB_PT_MAX];
	CBitmap* m_pBitmaps[BS_MAX][PS_MAX];
};

extern CString BUTTON_STATUS_NAME[BS_MAX];
extern CString TAB_BTN_PART_TYPE_NAME[TB_PT_MAX];

#endif
