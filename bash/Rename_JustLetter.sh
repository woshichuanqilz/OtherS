#!/bin/bash
#===============================================================================
#
#          FILE:  loop.sh
# 
#         USAGE:  ./loop.sh 
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
#       CREATED:  05/07/2016 01:10:42 PM CST
#      REVISION:  ---
#===============================================================================
clear
i=1
for x in `ls *.png| sort -f`
do
  mv $x $1$i.png
  let i=i+1
done
