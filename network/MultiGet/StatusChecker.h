// StatusChecker.h: interface for the CStatusChecker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUSCHECKER_H__40A34DE0_3D12_445D_84AA_8F01B5280B88__INCLUDED_)
#define AFX_STATUSCHECKER_H__40A34DE0_3D12_445D_84AA_8F01B5280B88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStatusChecker  
{
public:
	CStatusChecker(UINT nInitStatus = TSE_READY);
	virtual ~CStatusChecker();
	UINT GetCurrentStatus();
	void SetCurrentStatus(UINT nCurStatus);

	BOOL IsStartable();
	BOOL IsResumable();
	BOOL IsPausable();
	BOOL IsStopable();
protected:
	UINT m_nCurStatus;
};

#endif // !defined(AFX_STATUSCHECKER_H__40A34DE0_3D12_445D_84AA_8F01B5280B88__INCLUDED_)
