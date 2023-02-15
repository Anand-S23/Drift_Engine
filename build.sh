#!/bin/sh

set -xe

mkdir -p build
cd build

CC=gcc
CFLAGS="-Wall -Wextra -Wno-address -std=c11 -ggdb"
LIBS="-lGL -lSDL2 -lm -I../third_party"
ENGINE="-I../src"

name=app
engine_src=(../src/drift_entrypoint.c ../third_party/GL/glew.c)
game_src=../test_game/app.c

$CC $CFLAGS ${engine_src[*]} $LIBS -o $name
$CC $CFLAGS $game_src $LIBS $ENGINE -shared -o drift_app.dll

cd ..
