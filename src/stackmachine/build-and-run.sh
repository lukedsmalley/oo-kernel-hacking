#!/bin/bash -e

mkdir -p ../../build
gcc -o ../../build/main main.c
./generate-program.sh
../../build/main ../../build/program.bin
