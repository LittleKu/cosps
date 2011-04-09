@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by RESEXPLORER.HPJ. >"hlp\ResExplorer.hm"
echo. >>"hlp\ResExplorer.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\ResExplorer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\ResExplorer.hm"
echo. >>"hlp\ResExplorer.hm"
echo // Prompts (IDP_*) >>"hlp\ResExplorer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\ResExplorer.hm"
echo. >>"hlp\ResExplorer.hm"
echo // Resources (IDR_*) >>"hlp\ResExplorer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\ResExplorer.hm"
echo. >>"hlp\ResExplorer.hm"
echo // Dialogs (IDD_*) >>"hlp\ResExplorer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\ResExplorer.hm"
echo. >>"hlp\ResExplorer.hm"
echo // Frame Controls (IDW_*) >>"hlp\ResExplorer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\ResExplorer.hm"
REM -- Make help for Project RESEXPLORER


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\ResExplorer.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\ResExplorer.hlp" goto :Error
if not exist "hlp\ResExplorer.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\ResExplorer.hlp" Debug
if exist Debug\nul copy "hlp\ResExplorer.cnt" Debug
if exist Release\nul copy "hlp\ResExplorer.hlp" Release
if exist Release\nul copy "hlp\ResExplorer.cnt" Release
echo.
goto :done

:Error
echo hlp\ResExplorer.hpj(1) : error: Problem encountered creating help file

:done
echo.
