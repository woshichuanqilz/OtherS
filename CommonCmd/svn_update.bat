rm -rf "G:\WorkCode\lele2-20160222\游戏"

rm -rf "G:\WorkCode\lele2-20160222\开发库"

rm -rf "G:\WorkCode\lele2-20160222\系统模块"

rm -rf "G:\WorkCode\lele2-20160222\配置文件\"

rm -rf "G:\WorkCode\lele2-20160222\数据库改动记录\"

svn checkout svn://192.168.1.189/lele2/Server/lele2-20160222 g:/WorkCode/lele2-20160222

"g:\WorkCode\lele2-20160222\系统模块\Platform.sln"

echo update complete

ping 127.0.0.1 -n 6 > nul
