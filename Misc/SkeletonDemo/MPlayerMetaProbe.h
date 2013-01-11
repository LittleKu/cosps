// MPlayerMetaProbe.h: interface for the MPlayerMetaProbe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPLAYERMETAPROBE_H__EDBAAA0C_0A25_44F0_AE44_002F6DFF83E2__INCLUDED_)
#define AFX_MPLAYERMETAPROBE_H__EDBAAA0C_0A25_44F0_AE44_002F6DFF83E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cflwin/SyncBuffer.h"
#include "MetaProbe.h"
#include <map>

enum MetaID
{
	ID_LENGTH = 0,
	ID_START_TIME,
	ID_VIDEO_FORMAT,
	ID_VIDEO_WIDTH,
	ID_VIDEO_HEIGHT,
	ID_VIDEO_FPS,
	ID_VIDEO_BITRATE,
	
	ID_AUDIO_BITRATE,
	ID_AUDIO_RATE,
	ID_AUDIO_NCH
};

class MPlayerMetaProbe : public MetaProbe  
{
public:
	MPlayerMetaProbe(MetaMap* pMap);
	virtual ~MPlayerMetaProbe();
	virtual int Probe(LPCTSTR lpszFileName);
	virtual bool GetMeta(int nMetaID, std::string& val);

private:
	typedef std::map<int, std::string> ismap;
	
	class MPlayerMetaOutParser : public ContentParser
	{
	public:
		MPlayerMetaOutParser(MetaMap* pMap, ContentParser* parser = NULL);
		virtual ~MPlayerMetaOutParser();
		virtual void ParseContent(std::string& szLine, int nLineCount);
		virtual bool Init();
		virtual bool DeInit();
	private:
		MetaMap*	m_pMap;
		ContentParser* m_pParser;
	};
	
	MetaMap*	m_pMap;
};

#endif // !defined(AFX_MPLAYERMETAPROBE_H__EDBAAA0C_0A25_44F0_AE44_002F6DFF83E2__INCLUDED_)
