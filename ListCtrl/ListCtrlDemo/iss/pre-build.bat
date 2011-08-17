@echo off

set SYSDLL_DIR=C:\WINDOWS\system32
set DST_DIR=.\source

if exist "constant.in" copy /b /y "constant.in" "constant"
if not exist "%DST_DIR%" mkdir "%DST_DIR%"

copy /Y /B "%SYSDLL_DIR%\mfc42.dll" "%DST_DIR%"
copy /Y /B "%SYSDLL_DIR%\msvcrt.dll" "%DST_DIR%"
copy /Y /B "%SYSDLL_DIR%\msvcp60.dll" "%DST_DIR%"
copy /Y /B "%GTB4WIN_HOME%\bin\gtb.dll" "%DST_DIR%"
copy /Y /B "%LOG4CPLUS_HOME%\bin\log4cplus.dll" "%DST_DIR%"

pause
