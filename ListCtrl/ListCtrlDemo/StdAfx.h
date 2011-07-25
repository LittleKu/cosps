// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8960EFE3_C11E_4A7C_B50A_C4AB4F3504E1__INCLUDED_)
#define AFX_STDAFX_H__8960EFE3_C11E_4A7C_B50A_C4AB4F3504E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#pragma warning( disable : 4786 )

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxtempl.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#include <afxdb.h>

#pragma comment(lib, "htmlhelp.lib")

#ifdef _DEBUG

#ifndef ENABLE_LOG4CPLUS
#define ENABLE_LOG4CPLUS
#endif

#else

#ifdef ENABLE_LOG4CPLUS
#undef ENABLE_LOG4CPLUS
#endif

#endif

#include "log4cplus_config.h"

#include "Version.h"
#include "Util.h"
#include "ResizableDialog.h"
#include "gtb.h"
#include "Preferences.h"
#include "BkWnd.h"

#endif // !defined(AFX_STDAFX_H__8960EFE3_C11E_4A7C_B50A_C4AB4F3504E1__INCLUDED_)
