#ifndef __VERSION_H__
#define __VERSION_H__

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _T
#define _T(x)	x
#endif

#define _chSTR(x)		_T(#x)
#define chSTR(x)		_chSTR(x)

// *) Specify the version only here with the following defines.
// *) When changing any of those version nr. defines you also have to rebuild the language DLLs.
//
// General format:
//	<major>.<minor>.<update>.<build>
//
// Fields:
//	<major>		major number (e.g. 0)
//	<minor>		minor number (e.g. 30)
//	<update>	update number (e.g. 0='a'  1='b'  2='c'  3='d'  4='e'  5='f' ...)
//	<build>		build number; currently not used
//
// Currently used:
//  <major>.<minor>.<update> is used for the displayed version (GUI) and the version check number
//	<major>.<minor>			 is used for the protocol(!) version
//
#define VERSION_MJR		1
#define VERSION_MIN		0
#define VERSION_UPDATE	1
#define VERSION_BUILD	2

// NOTE: This version string is also used by the language DLLs!
#define	SZ_VERSION_NAME				chSTR(VERSION_MJR) _T(".") chSTR(VERSION_MIN) _T(".") chSTR(VERSION_UPDATE) _T(".") chSTR(VERSION_BUILD)
#define SZ_SHORT_VERSION_NAME		chSTR(VERSION_MJR) _T(".") chSTR(VERSION_MIN) _T(".") chSTR(VERSION_UPDATE)

#define SZ_PRODUCT_NAME				_T("Demo")
#define SZ_EXE_NAME					_T("ecc.exe")
#define SZ_ORGNIZATION_NAME			_T("www.easydemo77.com")
#define SZ_PRODUCT_WEBSITE			_T("http://www.easydemo77.com")
#define SZ_PRODUCT_SUPPORT_EMAIL	_T("support@easydemo77.com")

#define SZ_PRODUCT_WEBSITE_ORDER	SZ_PRODUCT_WEBSITE _T("/buynow.html")
#define SZ_PRODUCT_UPGRADE			SZ_PRODUCT_WEBSITE _T("/upgrade.html")
#define SZ_COPYRIGHT				_T("Copyright (c) 2011-2016") _T(" ") SZ_ORGNIZATION_NAME

#endif /* !__VERSION_H__ */
