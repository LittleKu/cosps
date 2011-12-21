// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B8816D45_D21B_4FC5_AA89_0EA1D2E5E673__INCLUDED_)
#define AFX_STDAFX_H__B8816D45_D21B_4FC5_AA89_0EA1D2E5E673__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxtempl.h>
#include <afxmt.h>
#include <Shlwapi.h>


#ifdef _DEBUG
#ifndef ENABLE_LOG4CPLUS
#define ENABLE_LOG4CPLUS
#endif
#else
#undef ENABLE_LOG4CPLUS
#endif

#include "log4cplus_config.h"
#include "Constants.h"
#include "common.h"
#include "Options.h"

#include "ResizableDialog.h"

#define CURL_STATICLIB

class CThreadMonitor;
CThreadMonitor* SYS_THREAD_MONITOR();

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__B8816D45_D21B_4FC5_AA89_0EA1D2E5E673__INCLUDED_)
