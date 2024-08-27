#!/bin/bash

set -e

echo "compiling malloc.c file into object file"
gcc -c malloc.c -o malloc.o

echo "link test.c file with malloc.o object file"
gcc test.c malloc.o -o test -pthread