@echo off

taskkill /f /t /im  GamePlaza.exe


copy /y  G:\代码\运行\Release\Unicode "D:\Program Files\九五娱乐平台"
@rem 休眠(2 - 1)秒
ping -n 2 127.1>nul  

copy /y G:\代码\运行\Release\Unicode "D:\Program Files\九五娱乐平台"


"D:\Program Files\九五娱乐平台\GamePlaza.exe"
ping -n 361 127.1>nul  

taskkill /f /t /im  GamePlaza.exe
taskkill /f /t /im  cmd.exe

pause