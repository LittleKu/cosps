#ifndef _LIST_CTRL_DEMO_UTIL_H_
#define _LIST_CTRL_DEMO_UTIL_H_

#define WM_START_COUNT          (WM_USER + 100)
#define WM_END_COUNT            (WM_USER + 101)
#define WM_UPDATE_PROGRESS      (WM_USER + 102)

typedef struct tagCountThreadParam
{
	HWND hwndMain;
	HWND hwndProgress;
}CountThreadParam, *lpCountThreadParam;



#endif