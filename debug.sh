#!/bin/bash

CC=gdb
LDFLAGS=`pkg-config --libs glew`
LDFLAGS="$LDFLAGS -lglut"

$CC main.cpp $LDFLAGS -o main -g