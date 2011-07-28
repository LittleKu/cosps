@echo off

set SYSDLL_DIR=C:\WINDOWS\system32
set DST_DIR=.\sourced

if not exist "%DST_DIR%" mkdir "%DST_DIR%"

copy /Y /B "%SYSDLL_DIR%\mfc42d.dll" "%DST_DIR%"
copy /Y /B "%SYSDLL_DIR%\msvcrtd.dll" "%DST_DIR%"
copy /Y /B "%SYSDLL_DIR%\msvcp60d.dll" "%DST_DIR%"
copy /Y /B "%SYSDLL_DIR%\mfco42d.dll" "%DST_DIR%"
copy /Y /B "%SYSDLL_DIR%\mfcd42d.dll" "%DST_DIR%"
copy /Y /B "%GTB4WIN_HOME%\bin\gtbd.dll" "%DST_DIR%"
copy /Y /B "%LOG4CPLUS_HOME%\bin\log4cplusd.dll" "%DST_DIR%"

pause