#!/bin/bash
# mkdir of the directory
mkdir -p logfolder

# define the extension of the file
ext="_log.logr"
folder="./logfolder/"


awk  '{ a[$7]++ } END { for (b in a) { print b } }' *.log > UserID.txt
sed -ri "s/^/UserID /g" UserID.txt
while IFS='' read -r line || [[ -n "$line" ]]; do
    filename="UserID $folder$line$ext"
    echo "file name is: $filename"
    grep -E $line *.log  > $filename
done < UserID.txt
