# Microsoft Developer Studio Project File - Name="YTGet" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=YTGet - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "YTGet.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "YTGet.mak" CFG="YTGet - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "YTGet - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "YTGet - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "YTGet - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\ResizableLib\1.3\ResizableLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 libcurl.lib ws2_32.lib wldap32.lib shell32.lib Shlwapi.lib wininet.lib ResizableLib.lib gtb.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\..\ResizableLib\1.3\ResizableLib\Release"

!ELSEIF  "$(CFG)" == "YTGet - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\ResizableLib\1.3\ResizableLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libcurld.lib ws2_32.lib wldap32.lib shell32.lib Shlwapi.lib wininet.lib ResizableLib.lib gtbd.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\..\ResizableLib\1.3\ResizableLib\Debug"

!ENDIF 

# Begin Target

# Name "YTGet - Win32 Release"
# Name "YTGet - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddTaskDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CNETDownloaderCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\common.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\CreatorFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Downloader.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloaderContext.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloaderCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloaderMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\EasyDownloader.cpp
# End Source File
# Begin Source File

SOURCE=.\HeaderDownloader.cpp
# End Source File
# Begin Source File

SOURCE=.\MainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainReBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MainToolBarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Options.cpp
# End Source File
# Begin Source File

SOURCE=.\ParserDownloader.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressMeter.cpp
# End Source File
# Begin Source File

SOURCE=.\SegmentDownloader.cpp
# End Source File
# Begin Source File

SOURCE=.\SegmentInfoMap.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\SHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\SListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\Sorter.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\SortSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TaskListComparator.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\ToolBarCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\YTDownloaderCreator.cpp
# End Source File
# Begin Source File

SOURCE=.\YTGet.cpp
# End Source File
# Begin Source File

SOURCE=.\YTGet.rc
# End Source File
# Begin Source File

SOURCE=.\YTGetDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddTaskDlg.h
# End Source File
# Begin Source File

SOURCE=.\CNETDownloaderCreator.h
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\CommonUtils.h
# End Source File
# Begin Source File

SOURCE=.\Constants.h
# End Source File
# Begin Source File

SOURCE=.\CreatorFactory.h
# End Source File
# Begin Source File

SOURCE=.\Downloader.h
# End Source File
# Begin Source File

SOURCE=.\DownloaderContext.h
# End Source File
# Begin Source File

SOURCE=.\DownloaderCreator.h
# End Source File
# Begin Source File

SOURCE=.\DownloaderMgr.h
# End Source File
# Begin Source File

SOURCE=.\EasyDownloader.h
# End Source File
# Begin Source File

SOURCE=.\HeaderDownloader.h
# End Source File
# Begin Source File

SOURCE=.\MainDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainReBar.h
# End Source File
# Begin Source File

SOURCE=.\MainToolBarCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Options.h
# End Source File
# Begin Source File

SOURCE=.\ParserDownloader.h
# End Source File
# Begin Source File

SOURCE=.\ProgressMeter.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SegmentDownloader.h
# End Source File
# Begin Source File

SOURCE=.\SegmentInfoMap.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\SHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\SListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\Sorter.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\SortSupport.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TaskListComparator.h
# End Source File
# Begin Source File

SOURCE=.\TaskListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ThreadMonitor.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\ToolBarCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\YTDownloaderCreator.h
# End Source File
# Begin Source File

SOURCE=.\YTGet.h
# End Source File
# Begin Source File

SOURCE=.\YTGetDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\HeadSort.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MainToolBarBkg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MainToolBarHot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TaskStatus.bmp
# End Source File
# Begin Source File

SOURCE=.\res\YTGet.ico
# End Source File
# Begin Source File

SOURCE=.\res\YTGet.rc2
# End Source File
# End Group
# End Target
# End Project
