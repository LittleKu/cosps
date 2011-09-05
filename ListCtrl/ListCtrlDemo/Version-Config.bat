@echo off

set MODE=1
if not "%1"=="" set MODE=%1

if "%MODE%"=="1" (
  if not exist "Version.h.in" goto END
  copy /Y /B "Version.h" "Version.h.bak" >nul
  echo copy /Y /B "Version.h" "Version.h.bak"
  
  rem copy /Y /B "Version.h.in" "Version.h" >nul
  rem echo copy /Y /B "Version.h.in" "Version.h"
  SubWCRev.exe . "Version.h.in" "Version.h"
  echo SubWCRev.exe . "Version.h.in" "Version.h"
  
  rem for debug
  copy /Y /B "Version.h" "Version.h.build" >nul
  echo copy /Y /B "Version.h" "Version.h.build"
  
  if exist "res\AppIcon.in.ico" copy /Y /B "res\AppIcon.in.ico" "res\AppIcon.ico"
) else ( 
  if not exist "Version.h.bak" goto END
  copy /Y /B "Version.h.bak" "Version.h" >nul  
  echo copy /Y /B "Version.h.bak" "Version.h"
  
  del /F /Q "Version.h.bak" >nul
  echo del /F /Q "Version.h.bak"
  
  call svn.exe revert -R .\res
  
  if exist "Debug/ListCtrlDemo.res" touch "Debug/ListCtrlDemo.res"
  if exist "Debug/acc.exe" touch "Debug/acc.exe"
  if exist "Release/ListCtrlDemo.res" touch "Release/ListCtrlDemo.res"
  if exist "Release/acc.exe" touch "Release/acc.exe"
)

:END
