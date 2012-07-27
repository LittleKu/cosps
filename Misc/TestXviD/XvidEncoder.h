// XvidEncoder.h: interface for the CXvidEncoder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XVIDENCODER_H__1FEBED4B_D0CD_4C42_A196_1AAC4E71F517__INCLUDED_)
#define AFX_XVIDENCODER_H__1FEBED4B_D0CD_4C42_A196_1AAC4E71F517__INCLUDED_

#pragma once

#include "xvid.h"

class CXvidEncoder  
{
public:
	CXvidEncoder();
	virtual ~CXvidEncoder();
	virtual bool IsEncoderCreated() const { return m_xvidEncoderHandle != 0; }
	virtual int CreateEncoder(int width, int height);
	virtual int EncodeFrame(void* image, int stride, int csp, int quality);
	virtual char* GetEncodedFrame() const { return m_frameBuffer; }
	virtual void Reset();

	enum
	{
		MAX_QUALITY = 6
	};

private:
	// Handle to the XviD encoder.
    void *m_xvidEncoderHandle;

	char *m_frameBuffer; /**< Buffer containing one frame's worth of encoded data. */
	int m_nBufferLength;

	static const int motion_presets[MAX_QUALITY + 1];
	static const int vop_presets[MAX_QUALITY + 1];
};

#endif // !defined(AFX_XVIDENCODER_H__1FEBED4B_D0CD_4C42_A196_1AAC4E71F517__INCLUDED_)
