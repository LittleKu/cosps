# Microsoft Developer Studio Project File - Name="SkeletonDemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SkeletonDemo - Win32 UniDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SkeletonDemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SkeletonDemo.mak" CFG="SkeletonDemo - Win32 UniDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SkeletonDemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SkeletonDemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "SkeletonDemo - Win32 UniRelease" (based on "Win32 (x86) Application")
!MESSAGE "SkeletonDemo - Win32 UniDebug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SkeletonDemo - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "obj\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\include" /I ".\ThirdParty" /I ".\AdvComboBox" /I "..\..\ResizableLib\1.3\ResizableLib" /I "..\..\tinyxml_2_5_3" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Shlwapi.lib ResizableLib.lib cflbase.lib cflwin.lib cflmfc.lib tinyxml.lib OptionExpS.lib /nologo /subsystem:windows /machine:I386 /libpath:".\lib" /libpath:"..\..\ResizableLib\1.3\ResizableLib\Release" /libpath:"..\..\tinyxml_2_5_3\Release"

!ELSEIF  "$(CFG)" == "SkeletonDemo - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "obj\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\include" /I ".\ThirdParty" /I ".\AdvComboBox" /I "..\..\ResizableLib\1.3\ResizableLib" /I "..\..\tinyxml_2_5_3" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 MFC42D.lib Shlwapi.lib ResizableLib.lib cflbaseD.lib cflwinD.lib cflmfcD.lib tinyxmld.lib OptionExpSD.lib /nologo /subsystem:windows /debug /machine:I386 /out:"./SkeletonDemoD.exe" /pdbtype:sept /libpath:".\lib" /libpath:"..\..\ResizableLib\1.3\ResizableLib\Debug" /libpath:"..\..\tinyxml_2_5_3\Debug"

!ELSEIF  "$(CFG)" == "SkeletonDemo - Win32 UniRelease"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SkeletonDemo___Win32_UniRelease"
# PROP BASE Intermediate_Dir "SkeletonDemo___Win32_UniRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "obj\UniRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I ".\ThirdParty" /I ".\AdvComboBox" /I "..\..\ResizableLib\1.3\ResizableLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\include" /I ".\ThirdParty" /I ".\AdvComboBox" /I "..\..\ResizableLib\1.3\ResizableLib" /I "..\..\tinyxml_2_5_3" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 MFC42.lib Shlwapi.lib ResizableLib.lib cflbase.lib cflmfc.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\..\ResizableLib\1.3\ResizableLib\Release"
# ADD LINK32 Shlwapi.lib ResizableLib.lib cflbaseU.lib cflwinU.lib cflmfcU.lib tinyxml.lib OptionExpSU.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /out:"./SkeletonDemoU.exe" /libpath:".\lib" /libpath:"..\..\ResizableLib\1.3\ResizableLib\UniRelease" /libpath:"..\..\tinyxml_2_5_3\Release"

!ELSEIF  "$(CFG)" == "SkeletonDemo - Win32 UniDebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SkeletonDemo___Win32_UniDebug"
# PROP BASE Intermediate_Dir "SkeletonDemo___Win32_UniDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "."
# PROP Intermediate_Dir "obj\UniDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\ThirdParty" /I ".\AdvComboBox" /I "..\..\ResizableLib\1.3\ResizableLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\include" /I ".\ThirdParty" /I ".\AdvComboBox" /I "..\..\ResizableLib\1.3\ResizableLib" /I "..\..\tinyxml_2_5_3" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 MFC42D.lib Shlwapi.lib ResizableLib.lib cflbaseD.lib cflmfcD.lib /nologo /subsystem:windows /debug /machine:I386 /out:"./SkeletonDemoD.exe" /pdbtype:sept /libpath:"..\..\ResizableLib\1.3\ResizableLib\Debug"
# ADD LINK32 MFC42UD.lib Shlwapi.lib ResizableLib.lib cflbaseUD.lib cflwinUD.lib cflmfcUD.lib tinyxmld.lib OptionExpSUD.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /out:"./SkeletonDemoUD.exe" /pdbtype:sept /libpath:".\lib" /libpath:"..\..\ResizableLib\1.3\ResizableLib\UniDebug" /libpath:"..\..\tinyxml_2_5_3\Debug"

!ENDIF 

# Begin Target

# Name "SkeletonDemo - Win32 Release"
# Name "SkeletonDemo - Win32 Debug"
# Name "SkeletonDemo - Win32 UniRelease"
# Name "SkeletonDemo - Win32 UniDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Converter.cpp
# End Source File
# Begin Source File

SOURCE=.\MainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainReBar.cpp
# End Source File
# Begin Source File

SOURCE=.\MainToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Preferences.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessExecutor.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\PropListMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SkeletonDemo.cpp
# End Source File
# Begin Source File

SOURCE=.\SkeletonDemo.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdStreamParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SysUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\ToolBarCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\VCDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\VisualStylesXP.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CommonCmds.h
# End Source File
# Begin Source File

SOURCE=.\CommonDef.h
# End Source File
# Begin Source File

SOURCE=.\Converter.h
# End Source File
# Begin Source File

SOURCE=.\MainDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainReBar.h
# End Source File
# Begin Source File

SOURCE=.\MainToolBar.h
# End Source File
# Begin Source File

SOURCE=.\Preferences.h
# End Source File
# Begin Source File

SOURCE=.\ProcessExecutor.h
# End Source File
# Begin Source File

SOURCE=.\ProfileLoader.h
# End Source File
# Begin Source File

SOURCE=.\PropListMgr.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SkeletonDemo.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StdStreamParser.h
# End Source File
# Begin Source File

SOURCE=.\SysUtils.h
# End Source File
# Begin Source File

SOURCE=.\TaskListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TaskMgr.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\ToolBarCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\VCDlg.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\VisualStylesXP.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\MainToolBarBkg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SkeletonDemo.ico
# End Source File
# Begin Source File

SOURCE=.\res\SkeletonDemo.rc2
# End Source File
# Begin Source File

SOURCE=.\res\SplitterBkgH.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SplitterBkgV.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_add.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_option.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_start.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_stop.bmp
# End Source File
# End Group
# Begin Group "AdvComboBox"

# PROP Default_Filter "cpp;c;cxx;h"
# Begin Source File

SOURCE=.\AdvComboBox\AdvComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\AdvComboBox\AdvComboBox.h
# End Source File
# Begin Source File

SOURCE=.\AdvComboBox\AdvComboBoxDef.h
# End Source File
# Begin Source File

SOURCE=.\AdvComboBox\DropListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\AdvComboBox\DropListBox.h
# End Source File
# Begin Source File

SOURCE=.\AdvComboBox\DropScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\AdvComboBox\DropScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\AdvComboBox\DropWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\AdvComboBox\DropWnd.h
# End Source File
# End Group
# End Target
# End Project
