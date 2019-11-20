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

sed -i "/$Test/,/\!LOAD/{//p;d}" "$Rom"
sed -i "/$Test/d" "$Rom"
sed "/$Test/,/\!LOAD/!d" "$Src" | sed '$d' >> "$Rom"




