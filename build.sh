#!/bin/bash

set -e

echo "compiling malloc.c file into object file"
gcc -c ./src/malloc.c -o ./src/malloc.o

echo "link test.c file with malloc.o object file"
gcc test.c ./src/malloc.o -o test -pthread