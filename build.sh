#!/bin/sh

set -xe

CFLAGS="-Wall -Werror -std=c11 -pedantic -ggdb `pkg-config --cflags sdl2`"
LIBS="`pkg-config --libs sdl2` -lm"

cc $CLAGS -o sound_exp main.c $LIBS
