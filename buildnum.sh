#!/bin/bash
buildfile="buildnum"
[[ -e $buildfile ]] || echo 0>$buildfile
B=$(cat $buildfile)
((B+=1))
echo $B>$buildfile
echo $B
