@echo off

call svn.exe revert .\constant
call svn.exe revert .\constantd
call svn.exe revert ..\help\help_en.chm

pause
