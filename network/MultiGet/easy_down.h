#ifndef __EASY_DOWN_H__
#define __EASY_DOWN_H__

class CEasyDown  
{
public:
	CEasyDown();
	virtual ~CEasyDown();
	
	int download(const char* url, HWND hwnd, CControlInfo* pControlInfo);
public:
	CControlInfo* m_pControlInfo;
};

#endif