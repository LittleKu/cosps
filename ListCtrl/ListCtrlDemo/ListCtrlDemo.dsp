# Microsoft Developer Studio Project File - Name="ListCtrlDemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ListCtrlDemo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ListCtrlDemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ListCtrlDemo.mak" CFG="ListCtrlDemo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ListCtrlDemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ListCtrlDemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ListCtrlDemo - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\ResizableLib\1.3\ResizableLib" /I ".\Include" /I "..\..\tinyxml_2_5_3" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ResizableLib.lib SCC.lib tinyxml.lib gtb.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\..\ResizableLib\1.3\ResizableLib\Release" /libpath:".\Lib\\" /libpath:"..\..\tinyxml_2_5_3\Release"

!ELSEIF  "$(CFG)" == "ListCtrlDemo - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\ResizableLib\1.3\ResizableLib" /I ".\Include" /I "..\..\tinyxml_2_5_3" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ResizableLib.lib SCCd.lib tinyxmld.lib gtbd.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\..\ResizableLib\1.3\ResizableLib\Debug" /libpath:".\Lib\\" /libpath:"..\..\tinyxml_2_5_3\Debug"

!ENDIF 

# Begin Target

# Name "ListCtrlDemo - Win32 Release"
# Name "ListCtrlDemo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\CheckHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\CheckListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Counter.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomLangDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Export.cpp
# End Source File
# Begin Source File

SOURCE=.\HistoryComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\LangGrammarMap.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlDemo.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlDemo.rc
# End Source File
# Begin Source File

SOURCE=.\ListCtrlDemoDlg.cpp
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

SOURCE=.\ThirdParty\MultiColumnSortListView.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\MultiSelTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PPgGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\Preferences.cpp
# End Source File
# Begin Source File

SOURCE=.\PreferencesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\TreePropSheet\PropPageFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\TreePropSheet\PropPageFrameDefault.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\SortableHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\SortClass.cpp
# End Source File
# Begin Source File

SOURCE=.\SourceDirListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\ToolBarCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\TreePropSheet\TreePropSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\Util.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\CheckHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\CheckListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Counter.h
# End Source File
# Begin Source File

SOURCE=.\CustomLangDlg.h
# End Source File
# Begin Source File

SOURCE=.\Export.h
# End Source File
# Begin Source File

SOURCE=.\HistoryComboBox.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\LangGrammarMap.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlDemo.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlDemoDlg.h
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

SOURCE=.\ThirdParty\MultiColumnSortListView.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\MultiSelTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PPgGeneral.h
# End Source File
# Begin Source File

SOURCE=.\Preferences.h
# End Source File
# Begin Source File

SOURCE=.\PreferencesDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\TreePropSheet\PropPageFrame.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\TreePropSheet\PropPageFrameDefault.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResultListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\SortableHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\SortClass.h
# End Source File
# Begin Source File

SOURCE=.\SourceDirListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\ToolBarCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\TreePropSheet\TreePropSheet.h
# End Source File
# Begin Source File

SOURCE=.\Util.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AppIcon.ico
# End Source File
# Begin Source File

SOURCE=.\res\HdrDownA.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HdrUpA.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\ListCtrlDemo.ico
# End Source File
# Begin Source File

SOURCE=.\res\ListCtrlDemo.rc2
# End Source File
# Begin Source File

SOURCE=.\res\MainToolBarBkg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MainToolBarHot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Options_General.ico
# End Source File
# Begin Source File

SOURCE=.\res\Preferences.ico
# End Source File
# Begin Source File

SOURCE=.\res\PurpleShell.ico
# End Source File
# Begin Source File

SOURCE=.\res\SplitterBkg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SplitterBkgH.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SplitterBkgV.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TreeCheckBoxState.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TreeItemSelState.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
