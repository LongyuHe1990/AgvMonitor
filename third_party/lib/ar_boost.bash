#!/bin/bash

OUTLIB=libLogger.a
FILE=`ls *.a`
for F in $FILE
do
    ar x $F
done

ar crU $OUTLIB *.o
ranlib $OUTLIB
rm -rf *.o


