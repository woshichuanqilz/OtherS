for /f "tokens=1-4 delims=/ " %%a in ('date/t') do (
	echo %%a 
	echo %%b 
	echo %%c
	echo %%d)
	pause
	
	@rem /t ������ָ���Ҳ����˵ֱ�Ӵ�date��û���õġ� 
	@rem /f ��֪�����������ķָ����
	@rem %%a,b,c�ֱ��ʾ�ڼ����ַ�����
	@rem delims=/ ע��delims ������������, һ��/ һ���ո� ��
	