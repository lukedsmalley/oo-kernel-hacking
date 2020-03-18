#!/bin/bash -e

mkdir -p ../../build/stackmachine
gcc -o ../../build/stackmachine/main main.c
./generate-program.sh
echo
echo "Build finished; running program."
echo
../../build/stackmachine/main ../../build/stackmachine/program.bin
