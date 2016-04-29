@echo off

chcp 936
xcopy "d:\Program Files\Vim" "g:\下载temp\使用软件备份\Vim" /d /e /r /k /y
xcopy "d:\Program Files\FirefoxPortable" "g:\下载temp\使用软件备份\FirefoxPortable" /d /e /r /k /y

REM  xcopy "D:\Program Files\FirefoxPortable" "E:\快盘\Program files\FirefoxPortable" /d /e /r /k /y
REM  xcopy "D:\Program Files\ALTRun" "E:\快盘\Program files\ALTRun" /d /e /r /k /y

echo backup success

SET SLEEP=ping 127.0.0.1 -n
%SLEEP% 3 > nul

exit
