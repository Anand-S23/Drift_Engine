#!/bin/sh

set -xe

mkdir -p build
cd build

CC=gcc
CFLAGS="-Wall -Wextra -Wno-address -Wno-implicit-function-declaration -std=c11 -std=gnu11 -ggdb"
LIBS="-lGL -lSDL2 -lm -ldl -I../third_party"
ENGINE="-I../src"

$CC $CFLAGS ../src/drift_entrypoint.c $LIBS -o app
$CC $CFLAGS ../test_game/app.c $LIBS $ENGINE -shared -fPIC -o drift_app.dll

cd ..
