#!/bin/sh

# Launcher in case dependencies are not avaible on the user system
# If dependencies are already installed, no need to use this script

RES_DIR=./resources
LIBS_DIR=./lib
BIN=./cosmoscroll

LD_LIBRARY_PATH="$LIBS_DIR":"$LD_LIBRARY_PATH" "$BIN" -d "$RES_DIR"

