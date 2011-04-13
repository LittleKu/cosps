#ifndef _COMMON_COOLTAB_H_
#define _COMMON_COOLTAB_H_

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

#endif
