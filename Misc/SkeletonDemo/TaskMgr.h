// TaskMgr.h: interface for the TaskMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKMGR_H__A797CE19_B56D_4623_91F9_ABF4CBE536A8__INCLUDED_)
#define AFX_TASKMGR_H__A797CE19_B56D_4623_91F9_ABF4CBE536A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>
#include "CmdBuilder.h"

class TaskMgr  
{
public:
	TaskMgr();
	virtual ~TaskMgr();

	void Reset();

	void StartTask();

private:
	void StartProcess();
public:
	typedef std::vector<int> TaskList;
	typedef std::vector<CmdInfo> CmdInfoList;

	TaskList			m_taskList;			/* Task list */
	int					m_nCurTaskIndex;	/* Current task index in the task list */
	int					m_nCurTaskPrgsBase;	/* Current task progress base */

	int					m_nCurTaskState;	/* Current task state */

	CmdInfoList			m_curCmdList;
	int					m_nCurCmdIndex;
	TStrVector			m_curDelList;
};

#endif // !defined(AFX_TASKMGR_H__A797CE19_B56D_4623_91F9_ABF4CBE536A8__INCLUDED_)
