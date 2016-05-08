#!/bin/bash
#===============================================================================
#
#          FILE:  RanamePic.sh
# 
#         USAGE:  ./RanamePic.sh 
# 
#   DESCRIPTION:  rename the pic file from input para in order 
# 
#       OPTIONS:  ---
#  REQUIREMENTS:  ---
#          BUGS:  ---
#         NOTES:  ---
#        AUTHOR:   (), 
#       COMPANY:  
#       VERSION:  1.0
#       CREATED:  05/06/2016 11:10:03 AM CST
#      REVISION:  ---
#===============================================================================


for file in *.png
do
  mv "$file" $1_$file
done

