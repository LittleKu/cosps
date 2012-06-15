# Microsoft Developer Studio Project File - Name="mydissolver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=mydissolver - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mydissolver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mydissolver.mak" CFG="mydissolver - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mydissolver - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "mydissolver - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mydissolver - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "C:\diskf\stdlibx\allegro\apeg\include" /I "C:\diskf\stdlibx\allegro\jpgalleg-2.5\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib ole32.lib dinput.lib ddraw.lib dxguid.lib winmm.lib dsound.lib alleg.lib libjpgalleg.lib apeg.lib revel.lib xvidcore.lib ogg.lib libvorbis.lib libtheora.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "mydissolver - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\diskf\stdlibx\allegro\apeg\include" /I "C:\diskf\stdlibx\allegro\jpgalleg-2.5\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib ole32.lib dinput.lib ddraw.lib dxguid.lib winmm.lib dsound.lib alld.lib libjpgalld.lib apegd.lib reveld.lib xvidcore_d.lib ogg_d.lib libvorbis_d.lib libtheora_d.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "mydissolver - Win32 Release"
# Name "mydissolver - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\b64.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\conv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\crossfadecol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\display.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\dissolver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\effect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\enc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\fadecol.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\getopt.c
# End Source File
# Begin Source File

SOURCE=..\..\src\jpg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\utils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\water.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\b64.h
# End Source File
# Begin Source File

SOURCE=..\..\src\conv.h
# End Source File
# Begin Source File

SOURCE=..\..\src\crossfade.h
# End Source File
# Begin Source File

SOURCE=..\..\src\crossfadecol.h
# End Source File
# Begin Source File

SOURCE=..\..\src\display.h
# End Source File
# Begin Source File

SOURCE=..\..\src\dissolver.h
# End Source File
# Begin Source File

SOURCE=..\..\src\effect.h
# End Source File
# Begin Source File

SOURCE=..\..\src\enc.h
# End Source File
# Begin Source File

SOURCE=..\..\src\fade.h
# End Source File
# Begin Source File

SOURCE=..\..\src\fadecol.h
# End Source File
# Begin Source File

SOURCE=..\..\src\getopt.h
# End Source File
# Begin Source File

SOURCE=..\..\src\jpg.h
# End Source File
# Begin Source File

SOURCE=..\..\src\revel.h
# End Source File
# Begin Source File

SOURCE=..\..\src\utils.h
# End Source File
# Begin Source File

SOURCE=..\..\src\water.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
