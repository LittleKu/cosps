#include "StdAfx.h"
#include "CoolTabCommon.h"

CString BUTTON_STATUS_NAME[BS_MAX] = {"normal", "highlighted", "clicked", "disabled"}; 
CString TAB_BTN_PART_TYPE_NAME[TB_PT_MAX] = {"background", "seperator"};

CImgTabBtn::CImgTabBtn()
{
	TRACE1("sizeof(m_pTabPart)=%d\n", sizeof(m_pTabPart));
	TRACE1("sizeof(m_pBitmaps)=%d\n", sizeof(m_pBitmaps));
	ZeroMemory(m_pTabPart, sizeof(m_pTabPart));
	ZeroMemory(m_pBitmaps, sizeof(m_pBitmaps));
}
