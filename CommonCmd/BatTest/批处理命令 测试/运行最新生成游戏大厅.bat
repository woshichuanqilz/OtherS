@echo off

taskkill /f /t /im  GamePlaza.exe


copy /y  G:\����\����\Release\Unicode "D:\Program Files\��������ƽ̨"
@rem ����(2 - 1)��
ping -n 2 127.1>nul  

copy /y G:\����\����\Release\Unicode "D:\Program Files\��������ƽ̨"


"D:\Program Files\��������ƽ̨\GamePlaza.exe"
ping -n 361 127.1>nul  

taskkill /f /t /im  GamePlaza.exe
taskkill /f /t /im  cmd.exe

pause