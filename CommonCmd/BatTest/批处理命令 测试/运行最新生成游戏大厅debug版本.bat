@echo off

taskkill /f /t /im  GamePlaza.exe


copy /y  G:\����\����\����\Debug\Unicode "C:\Program Files (x86)\��������ƽ̨"
@rem ����(2 - 1)��
ping -n 2 127.1>nul  

copy /y  G:\����\����\����\Debug\Unicode "C:\Program Files (x86)\��������ƽ̨"


"C:\Program Files (x86)\��������ƽ̨\GamePlaza.exe"
ping -n 121 127.1>nul  

taskkill /f /t /im  GamePlaza.exe
taskkill /f /t /im  cmd.exe

pause