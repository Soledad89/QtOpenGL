#!/bin/sh
#Made By Jiangcaiyang at 17时25分16秒
 
if [ -n "." ] ; then
if [ "${LD_LIBRARY_PATH+set}" = "set" ] ; then
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:."
else
export LD_LIBRARY_PATH="."
fi
fi
chmod +x GLWidget
exec ./GLWidget
