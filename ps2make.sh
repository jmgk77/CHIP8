#!/bin/sh
docker  run  -v `pwd`:`pwd` -w `pwd` -i -t  ps2dev/jmgk77 make -f Makefile.ps2 $1 $2 $3
