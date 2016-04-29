xcopy /s/y/i/f/exclude:uncopy.txt ".\lg\*.*" ".\hehe"

SET SLEEP=ping 127.0.0.1 -n
%SLEEP% 5 > nul
del .\hehe\*.* /q
echo test done
SET SLEEP=ping 127.0.0.1 -n
%SLEEP% 2 > nul

rem 那个listfile uncopy应该放到那个当前的目录下面。
