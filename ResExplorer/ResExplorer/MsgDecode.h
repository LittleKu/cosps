
#ifndef MsgDecode_h
#define MsgDecode_h

#ifdef _DEBUG       // entire file for debugging

// usage : NOTVERY( msg, TRACE("xxxxx %s xxx\n", decode(msg)) );
#define NOTVERY(msg, trace) if (!IsVeryFrequentlySent(msg)) trace
#define IFCOND(cond, trace) if (cond) trace

extern BOOL IsVeryFrequentlySent(UINT msg);
extern CString decode(UINT msg);
extern CString decodeRes(LPCTSTR resType);

#else

#define NOTVERY(msg, trace)
#define IFCOND(cond, trace)

#endif

#endif
