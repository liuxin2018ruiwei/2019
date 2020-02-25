#!/bin/bash
pwd
rm ./lrzsz/src/lsz
rm ./build/lsz
pwd
cd lrzsz
make distclean
CC=/opt/hisi-linux/x86-arm/arm-hisiv300-linux/target/bin/arm-hisiv300-linux-gcc CFLAGS=-O2 ./configure  --host= arm-linux  --target=arm-linux  
make

pwd
cp ./src/lsz ../build/


