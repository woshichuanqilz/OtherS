@echo off

set _task=GameServer_100.exe
set _svr=.\GameServer_100.exe
set _des=start.bat

:checkstart
for /f "tokens=1" %%n in ('tasklist ^| findstr "%_task%" ') do (
 if %%n==%_task% (goto checkag) else goto startsvr
)

 

:startsvr
echo %date%%time% 
echo ********����ʼ����********
echo �������������� %date%%time% ,����ϵͳ��־ >> restart_service.txt
echo start %_svr% > %_des%
echo exit >> %_des%
start %_des%
set/p=.<nul
for /L %%i in (1 1 10) do set /p a=.<nul&ping.exe /n 2 127.0.0.1>nul
echo .
echo Wscript.Sleep WScript.Arguments(0) >%tmp%\delay.vbs 
cscript //b //nologo %tmp%\delay.vbs 10000 
del %_des% /Q
echo ********�����������********
goto checkstart


:checkag
echo %date%%time% ������������,10���������.. 
echo Wscript.Sleep WScript.Arguments(0) >%tmp%\delay.vbs 
cscript //b //nologo %tmp%\delay.vbs 10000 
goto checkstart
