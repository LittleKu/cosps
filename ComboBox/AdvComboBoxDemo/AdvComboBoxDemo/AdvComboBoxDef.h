#ifndef __ADV_COMBO_BOX_DEF_H__
#define __ADV_COMBO_BOX_DEF_H__

#pragma once

#define WM_ON_DROPDOWN_BUTTON	(WM_USER+101)

#define WM_SELECTED_ITEM		(WM_USER+201)
#define WM_DESTROY_DROPLIST		(WM_USER+202)
#define WM_VRC_SETCAPTURE		(WM_USER+203)
#define WM_VRC_RELEASECAPTURE	(WM_USER+204)

#define ACBS_FLAT			0x0001
#define ACBS_STANDARD		0x0002
#define ACBS_CHECKED		0x0004
#define ACBS_AUTOAPPEND		0x0008
#define ACBS_AUTOSUGGEST	0x0010

enum AdvComboBoxItemState
{
	ACBIS_NORMAL	= 0,
	ACBIS_DISABLED	= 0x0001,
	ACBIS_CHECKED	= 0x0002,
	ACBIS_CHILDREN	= 0x0004,
	ACBIS_COLLAPSED	= 0x0008
};

class AdvComboBoxItem
{
public:
	CString		strText;
	UINT		state;
	HBITMAP		hImage;
	void*		vpItemData;
public:
	AdvComboBoxItem()
	{
		strText.Empty();
		state = ACBIS_NORMAL;
		hImage = NULL;
		vpItemData = NULL;
	}

	bool operator < (const AdvComboBoxItem& other)
	{
		return strText < other.strText;
	}
};

typedef AdvComboBoxItem LIST_ITEM, *PLIST_ITEM;

#endif