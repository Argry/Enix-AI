#!/bin/bash

# GPL [OvO]wl, saving time 2015. Part of ENiX test utilities.

Nargs="$#"
FileName="$0"

if [ $Nargs -ne 3 ]
then 
    echo ""
    echo "Usage: $FileName <test file> <test rom file> <test>"
    echo ""
    exit
fi

Src="$1"
Rom="$2"
Test="$3"

sed "/$Test/,/\!LOAD/!d; /SYSTEM/d" "$Src" > /tmp/file1.txt
sed "/$Test/,/\!LOAD/!d; /SYSTEM/d" "$Rom" > /tmp/file2.txt

vimdiff /tmp/file1.txt /tmp/file2.txt


