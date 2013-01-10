// MetaProbe.h: interface for the MetaProbe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METAPROBE_H__0828B037_F130_414C_82E4_98FA903327D6__INCLUDED_)
#define AFX_METAPROBE_H__0828B037_F130_414C_82E4_98FA903327D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class MetaProbe  
{
public:
	MetaProbe();
	virtual ~MetaProbe();

	virtual int Probe(LPCTSTR lpszFileName) = 0;
	virtual bool GetMeta(int nMetaID, std::string& val) = 0;
};

#endif // !defined(AFX_METAPROBE_H__0828B037_F130_414C_82E4_98FA903327D6__INCLUDED_)
