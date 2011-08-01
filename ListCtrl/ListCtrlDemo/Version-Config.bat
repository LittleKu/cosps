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
) else ( 
  if not exist "Version.h.bak" goto END
  copy /Y /B "Version.h.bak" "Version.h" >nul  
  echo copy /Y /B "Version.h.bak" "Version.h"
  
  del /F /Q "Version.h.bak" >nul
  echo del /F /Q "Version.h.bak"
)

:END
