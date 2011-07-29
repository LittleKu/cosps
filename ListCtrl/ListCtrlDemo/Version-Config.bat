@echo off

set MODE=1
if not "%1"=="" set MODE=%1

if "%MODE%"=="1" (
  if not exist "Version.h.in" goto END
  copy /Y /B "Version.h" "Version.h.bak" >nul
  echo copy /Y /B "Version.h" "Version.h.bak"
  
  copy /Y /B "Version.h.in" "Version.h" >nul
  echo copy /Y /B "Version.h.in" "Version.h"
) else ( 
  if not exist "Version.h.bak" goto END
  copy /Y /B "Version.h.bak" "Version.h" >nul  
  echo copy /Y /B "Version.h.bak" "Version.h"
  
  del /F /Q "Version.h.bak" >nul
  echo del /F /Q "Version.h.bak"
)

:END
