#!/bin/bash

cd ./lib
#extract auxiliary libraries
tar zxvf lib.tar.gz 
#go back and compile all
cd ..
make all

