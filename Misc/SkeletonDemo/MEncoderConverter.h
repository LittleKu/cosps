// MEncoderConverter.h: interface for the MEncoderConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENCODERCONVERTER_H__8AFE03E6_C811_4D4C_9212_90E160B6C151__INCLUDED_)
#define AFX_MENCODERCONVERTER_H__8AFE03E6_C811_4D4C_9212_90E160B6C151__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cflbase/tstring.h"
#include "cflwin/WinProcess.h"
#include "cflwin/SyncBuffer.h"
#include "cflwin/MsgQueue.h"

class MEncoderConverter : public cfl::MsgHandler  
{
public:
	MEncoderConverter();
	virtual ~MEncoderConverter();
	
	/* Set the Window Handle to which the response message should send */
	void SetResponseWnd(HWND hWnd);
	
	/* Create the message queue. 
	* If the function succeeds, the return value is 0. otherwise nonzero. 
	*/
	virtual int Create();
	
	/* Destroy the message queue.
	* If the function succeeds, the return value is 0. otherwise nonzero. 
	*/
	virtual int Destroy();
	
	/* Send message to the queue, and wait until the message to be processed */
	virtual LRESULT SendMsg(UINT uMsg, WPARAM wParam = NULL, LPARAM lParam = NULL);
	
	/* Post message to the queue, returns without waiting for the thread to process the message */
	virtual BOOL PostMsg(UINT uMsg, WPARAM wParam = NULL, LPARAM lParam = NULL);
	
	/* Implement the MsgHandler to process messages in the MsgQueue */
	virtual LRESULT ProcessMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	enum ErrorCode
	{
		OK = 0,
		ALREADY_EXIST,
		CREATE_THREAD_FAILED,
		BUILD_COMMAND_LINE_FAILED
	};
	
private:
	int Convert(ConvertParam* lpParam);
	
private:
	class OutParser : public ContentParser
	{
	public:
		OutParser(HWND hRspWnd) { this->m_hRspWnd = hRspWnd; }
		virtual void ParseContent(std::string& szLine, int nLineCount) {}
	private:
		HWND m_hRspWnd;
	};
	
	class ErrParser : public ContentParser
	{
	public:
		ErrParser(HWND hRspWnd);
		virtual void Reset();
		virtual void ParseContent(std::string& szLine, int nLineCount);
	private:
		bool GetDuration(std::string& szLine, std::string& szDuration);
		bool GetProgress(std::string& szLine, std::string& szProgress);
		bool GetRegexVal(const std::string& str, const std::string szPattern, int groupCount, int groupIndex, std::string& szVal);
	private:
		enum
		{
			EP_INIT = 0,
			EP_DURATION_DONE = 1
		};
		HWND m_hRspWnd;
		int	 m_nState;
	};
	
private:
	static DWORD WINAPI PlayerThreadProc(LPVOID lpParameter);
	static DWORD WINAPI StdOutThreadProc(LPVOID lpParameter);
	static DWORD WINAPI StdErrThreadProc(LPVOID lpParameter);
	
	DWORD PlayerProc();
	DWORD StdOutProc();
	DWORD StdErrProc();
	
	DWORD OutStreamProc(cfl::SyncBuffer* pBuffer, ContentParser* pParser, LPCTSTR szName);
	
private:
	HANDLE				m_hConverterThread;
	HANDLE				m_hOutThread;
	HANDLE				m_hErrThread;
	
	cfl::MsgQueue*		m_pMsgQueue;
	cfl::WinProcess*	m_pProcess;
	ContentParser*		m_pOutParser;
	ContentParser*		m_pErrParser;
	
	cfl::tstring		m_szCmdLine;
	cfl::SyncBuffer		m_outReceiver;
	cfl::SyncBuffer		m_errReceiver;
		
	HWND				m_hRspWnd;
};

#endif // !defined(AFX_MENCODERCONVERTER_H__8AFE03E6_C811_4D4C_9212_90E160B6C151__INCLUDED_)
