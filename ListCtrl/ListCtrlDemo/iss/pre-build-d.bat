@echo off

set SYSDLL_DIR=C:\WINDOWS\system32
set DST_DIR=.\sourced

if exist "constantd.in" copy /b /y "constantd.in" "constantd"
if exist "help_en.in.chm" copy /b /y "help_en.in.chm" "..\help\help_en.chm"
if not exist "%DST_DIR%" mkdir "%DST_DIR%"

copy /Y /B "%SYSDLL_DIR%\mfc42d.dll" "%DST_DIR%"
copy /Y /B "%SYSDLL_DIR%\msvcrtd.dll" "%DST_DIR%"
copy /Y /B "%SYSDLL_DIR%\msvcp60d.dll" "%DST_DIR%"
copy /Y /B "%SYSDLL_DIR%\mfco42d.dll" "%DST_DIR%"
copy /Y /B "%SYSDLL_DIR%\mfcd42d.dll" "%DST_DIR%"
copy /Y /B "%GTB4WIN_HOME%\bin\gtbd.dll" "%DST_DIR%"
copy /Y /B "%LOG4CPLUS_HOME%\bin\log4cplusd.dll" "%DST_DIR%"

pause