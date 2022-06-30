#!/bin/sh
docker  run  -v `pwd`:`pwd` -w `pwd` -i -t  ps3dev/ps3dev make -f Makefile $1 $2 $3
