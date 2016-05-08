#!/bin/bash
#===============================================================================
#
#          FILE:  Array.sh
# 
#         USAGE:  ./Array.sh 
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
#       CREATED:  02/04/2016 10:28:39 AM CST
#      REVISION:  ---
#===============================================================================
find . -name '*.md' | while read line; do
    echo "Copying '$line' to /tmp"
    sed -i "1s/^/hehe/" $line
done
