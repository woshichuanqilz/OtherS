@echo off

chcp 936
xcopy "d:\Program Files\Vim" "g:\����temp\ʹ���������\Vim" /d /e /r /k /y
xcopy "d:\Program Files\FirefoxPortable" "g:\����temp\ʹ���������\FirefoxPortable" /d /e /r /k /y

REM  xcopy "D:\Program Files\FirefoxPortable" "E:\����\Program files\FirefoxPortable" /d /e /r /k /y
REM  xcopy "D:\Program Files\ALTRun" "E:\����\Program files\ALTRun" /d /e /r /k /y

echo backup success

SET SLEEP=ping 127.0.0.1 -n
%SLEEP% 3 > nul

exit
