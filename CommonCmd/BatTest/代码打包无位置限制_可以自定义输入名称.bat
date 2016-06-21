@echo off
del /f/q/s *.sdf 
del /f/q/s *.ipch

for /f "delims=" %%a in ('dir /ad/s/b Release_Unicode') do rd /s/q "%%~a"
for /f "delims=" %%a in ('dir /ad/s/b Release') do rd /s/q "%%~a"
for /f "delims=" %%a in ('dir /ad/s/b Debug_Unicode') do rd /s/q "%%~a"
for /f "delims=" %%a in ('dir /ad/s/b Debug') do rd /s/q "%%~a"
for /f "delims=" %%a in ('dir /ad/s/b ipch') do rd /s/q "%%~a"

pause
