#!/bin/bash
## executes a single line from a file
## usage:
## runline.sh list line-number

if [ -f $1 ] ; then
    re='^[0-9]+$'
    if ! [[ $2 =~ $re ]] ; then
        echo "Error: argument 2 is not a number" >&2; exit 1
    fi
    line=`sed -n ${2}'p' ${1}`
    eval ${line}
else
    echo Error: file "$1" not found >&2; exit 2
fi
