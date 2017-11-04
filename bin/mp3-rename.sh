#! /bin/sh

awk -F/ '{++i; printf "mv %s %02d_%s\n", $2, i, $2}' |
sh -x -s
