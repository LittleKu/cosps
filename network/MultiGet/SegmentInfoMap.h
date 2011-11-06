// SegmentInfoMap.h: interface for the CSegmentInfoMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEGMENTINFOMAP_H__27D29BE5_9949_467B_B4EE_B1F3FA9F22D5__INCLUDED_)
#define AFX_SEGMENTINFOMAP_H__27D29BE5_9949_467B_B4EE_B1F3FA9F22D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef CArray<CSegmentInfo*, CSegmentInfo*> CSegmentInfoArray;
typedef CMap<int, int, void*, void*> CMapIntToPtr;

class CSegmentInfoMap  
{
public:
	virtual ~CSegmentInfoMap();
	/*
	CSegmentInfoArray* GetSegmentInfoArray(LPCTSTR lpszUrl);
	void AddSegmentInfoArray(LPCTSTR lpszUrl, CSegmentInfoArray* pSegInfoArray);
	void RemoveSegmentInfoArray(LPCTSTR lpszUrl);
	*/

	CSegmentInfoArray* GetSegmentInfoArray(int nTaskID);
	void AddSegmentInfoArray(int nTaskID, CSegmentInfoArray* pSegInfoArray);
	void RemoveSegmentInfoArray(int nTaskID);
private:
	CSegmentInfoMap();	
//	CMapStringToPtr m_mapSegmentInfo;
	CMapIntToPtr m_mapSegmentInfo;
public:
	static CSegmentInfoMap* GetInstance();
};

#endif // !defined(AFX_SEGMENTINFOMAP_H__27D29BE5_9949_467B_B4EE_B1F3FA9F22D5__INCLUDED_)
