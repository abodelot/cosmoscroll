#!/bin/sh

# Launcher in case dependencies are not avaible
# If sfml and libdumb are correctly installed, no need to use this script

DATA_DIR='./data'
LIBS_DIR=./lib32
BIN=./cosmoscroll

LD_LIBRARY_PATH="$LIBS_DIR":"$LD_LIBRARY_PATH" "$BIN" -d "$DATA_DIR"
