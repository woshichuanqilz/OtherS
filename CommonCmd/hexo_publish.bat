h:
cd "h:\GitCode\lizheblog\"
hexo d -g
echo "blog publish over quit in 6 secs"
ping 127.0.0.1 -n 6 > nul
