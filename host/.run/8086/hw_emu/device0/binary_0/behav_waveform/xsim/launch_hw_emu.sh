#!/bin/bash -f
export XTLM_LOG_STATE=NONE
echo " running simulate.sh $*"
./simulate.sh $* >output.txt 2>&1
