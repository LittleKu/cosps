#include "StdAfx.h"
#include "VersionUtil.h"

#include <shlwapi.h>

void CheckDllVersion(LPCTSTR lpstrDllName, ULONGLONG* pDllVersion)
{
	ASSERT(pDllVersion != NULL);

	HMODULE hMod = ::LoadLibrary(lpstrDllName);
	if (hMod != NULL)
	{
		// Get the version function
		DLLGETVERSIONPROC pfnDllGetVersion;
		pfnDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hMod, "DllGetVersion");
		
		if (pfnDllGetVersion != NULL)
		{
			// Obtain version information
			DLLVERSIONINFO dvi;
			::ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(DLLVERSIONINFO);
			if (SUCCEEDED(pfnDllGetVersion(&dvi)))
			{
				//Convert DLLVERSIONINFO to ULONGLONG
				*pDllVersion = MAKEDLLVERULL(dvi.dwMajorVersion, dvi.dwMinorVersion, 0, 0);
				::FreeLibrary(hMod);
				return;
			}
		}
		
		::FreeLibrary(hMod);
	}
	
	// Set values for the worst case
	*pDllVersion = MAKEDLLVERULL(4, 0, 0, 0);
}
