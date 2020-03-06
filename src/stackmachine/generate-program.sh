#!/bin/bash -e

F=../../build/stackmachine/program.bin

# Function table length
printf '\x01\x00\x00\x00\x00\x00\x00\x00' > $F

# Function 0
# | Length
printf '\x02\x00\x00\x00\x00\x00\x00\x00' >> $F
# | Body
printf '\x00\x01' >> $F
