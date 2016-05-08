#!/bin/bash
#===============================================================================
#
#          FILE:  CreateBlog.sh
# 
#         USAGE:  ./CreateBlog.sh 
# 
#   DESCRIPTION:  
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR:   (), 
#       COMPANY:  
#       VERSION:  1.0
#       CREATED:  01/01/2016 08:39:53 PM PST
#      REVISION:  ---
#===============================================================================

rm -rf lizheblog
mkdir lizheblog
cd lizheblog

git init
hexo init lizheblog
sudo npm install
# cp ~/lizheblogtest/_config.yml ~/lizheblog/
# hexo generate
# hexo deploy
