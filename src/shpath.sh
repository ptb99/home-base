#!/bin/sh
IFS="${IFS}:"
mypath=$PATH
if test -n "$2"; then
   mypath=$2
fi
for cmddir in ${mypath}
do
    test -s $cmddir/$1 && test ! -d $cmddir/$1 && echo "	$cmddir/$1"
done
