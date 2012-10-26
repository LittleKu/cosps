@echo off

set SRC_APP=elw.exe
set TGT_DIR=.

if not "%~1"=="" set SRC_APP=%~1
if not "%~2"=="" set TGT_DIR=%~2

echo SRC_APP="%SRC_APP%"
echo TGT_DIR="%TGT_DIR%"

if not exist "%SRC_APP%" (
  echo SRC_APP=%SRC_APP% doesn't exist
  goto END
)

if not exist "%TGT_DIR%" (
  echo TGT_DIR=%TGT_DIR% doesn't exist
  goto END
)

touch trash
for /f "delims=" %%i in ('dir /b "%TGT_DIR%\*.exe"') do (
  copy /b /y "%SRC_APP%" "%TGT_DIR%\%%i" /b >>trash
  echo replaced %%i
)
del /f /q trash

:END

pause