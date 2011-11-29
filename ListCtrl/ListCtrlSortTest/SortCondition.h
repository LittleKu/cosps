// SortCondition.h: interface for the CSortCondition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTCONDITION_H__1CFB453E_74C2_47B1_9BCA_83C3CC1281E8__INCLUDED_)
#define AFX_SORTCONDITION_H__1CFB453E_74C2_47B1_9BCA_83C3CC1281E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSortCondition  
{
public:
	CSortCondition();
	virtual ~CSortCondition();

public:
	int m_nCol;
    int m_nDir;
};

#endif // !defined(AFX_SORTCONDITION_H__1CFB453E_74C2_47B1_9BCA_83C3CC1281E8__INCLUDED_)
