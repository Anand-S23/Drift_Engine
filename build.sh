#!/bin/sh

set -xe

mkdir -p build
cd build

CC=gcc
CFLAGS="-Wall -Wextra -Wno-address -std=c11 -ggdb"
LIBS="-lGL -lSDL2 -lm -I../third_party"

src=../src/drift_entrypoint.c
name=drift

$CC $CFLAGS -o $name $src ../third_party/GL/glew.c $LIBS

cd ..
