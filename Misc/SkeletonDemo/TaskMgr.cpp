// TaskMgr.cpp: implementation of the TaskMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TaskMgr::TaskMgr()
{

}

TaskMgr::~TaskMgr()
{

}

void TaskMgr::Reset()
{
	m_taskList.clear();
	m_nCurTaskIndex = -1;
	m_nCurTaskPrgsBase = 0;
	m_curCmdList.clear();
	m_nCurCmdIndex = -1;
	m_curDelList.clear();
	m_nCurTaskState = TSE_READY;
}