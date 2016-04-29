for /f "tokens=1-4 delims=/ " %%a in ('date/t') do (
	echo %%a 
	echo %%b 
	echo %%c
	echo %%d)
	pause
	
	@rem /t 是命令分隔符也就是说直接打date是没有用的。 
	@rem /f 是知道下面的下面的分割方法。
	@rem %%a,b,c分别表示第几个字符串。
	@rem delims=/ 注意delims 包括两个符号, 一个/ 一个空格 。
	