@echo off
call:dir "%cd%"
call:name "%dir:~,-1%"
pause
goto :eof
:dir
set dir=%~dp1
goto :eof
:name
set dirul=%~nx1
echo %dirul%
pause