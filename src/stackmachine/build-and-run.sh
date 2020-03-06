#!/bin/bash -e

mkdir -p ../../build/stackmachine
gcc -o ../../build/stackmachine/main main.c
./generate-program.sh
../../build/stackmachine/main ../../build/stackmachine/program.bin
