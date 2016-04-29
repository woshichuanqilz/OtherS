@echo off
ipconfig | find "IPv4"
SET SLEEP=ping 127.0.0.1 -n
%SLEEP% 5 > nul