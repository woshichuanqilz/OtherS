copy "MFYueYuan_Noncommercial-Regular.otf" "%WINDIR%\Fonts"
reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Fonts" /v "MFYueYuan_Noncommercial-Regular (TrueType)" /t REG_SZ /d MFYueYuan_Noncommercial-Regular.otf /f

pause
