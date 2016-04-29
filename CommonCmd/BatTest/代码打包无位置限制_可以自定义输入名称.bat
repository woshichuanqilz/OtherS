@echo off
::taskkill /f /t /im devenv.exe

echo plz input the name of the proj
set /p projname= >nul
set projpa=%cd%
set pa=%cd%\Platform.sln
set rarfile="G:\Code\Code\´úÂë°æ±¾\´úÂëÑ¹ËõTemp\%projname%"
echo %rarfile%
WinRAR a -y %rarfile% "%projpa%"
pause
devenv.exe "%pa%" /clean Release_Unicode
devenv.exe "%pa%" /clean Debug_Unicode
devenv.exe "%pa%" /clean Release
devenv.exe "%pa%" /clean Debug
 
del /f/q/s *.sdf 
del /f/q/s *.ipch

::for /f "delims=" %%a in ('dir /ad/s/b Release_Unicode') do rd /s/q "%%~a"
::for /f "delims=" %%a in ('dir /ad/s/b Release') do rd /s/q "%%~a"
::for /f "delims=" %%a in ('dir /ad/s/b Debug_Unicode') do rd /s/q "%%~a"
::for /f "delims=" %%a in ('dir /ad/s/b Debug') do rd /s/q "%%~a"
for /f "delims=" %%a in ('dir /ad/s/b ipch') do rd /s/q "%%~a"

WinRAR a -y "rarfile" "%projpa%"

pause