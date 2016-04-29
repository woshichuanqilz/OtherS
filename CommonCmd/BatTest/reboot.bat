@echo off
echo Do you want reboot?
set /p op=""
if "%op%" == "yes" (
    echo the reboot will begin
    shutdown -r -t 10
) 
if not "%op%" == "yes" (
    echo reboot abort !
    )

ping 127.0.0.1 -n 3 > nul 
