#!/usr/bin/env python
#-*- coding: utf-8 -*-

import os

target = "."
makefile_name = "Makefile"
sources = []

for source in sorted(os.listdir(target)):
    if os.path.isfile(source) and source.endswith(".cpp"):
        sources.append(os.path.splitext(source)[0])

makefile = open(makefile_name, "w")

makefile.write("""
# this is an auto-generated makefile
CC=g++
FLAGS=-Wall -Wextra -Wwrite-strings -pedantic -ansi
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
EXEC=cosmoscroll
DEBUG=yes
ifeq ($(DEBUG), yes)
	CFLAGS=$(FLAGS) -g -DDEBUG
else
	CFLAGS=$(FLAGS) -O3
endif

""")

makefile.write("""
$(EXEC): %s
	$(CC) $^ -o $(EXEC) $(LDFLAGS)

""" % " ".join([s + ".o" for s in sources])) # lol :)

for s in sources:
    header = s + ".hpp"
    if os.path.isfile(header):
        makefile.write("%s.o: %s.cpp %s\n" % (s, s, header))
    else:
        makefile.write("%s.o: %s.cpp\n" % (s, s))
    
    makefile.write("\t$(CC) $< -c $(CFLAGS)\n\n")

makefile.write("""
clean:
	-rm *.o

mrproper: clean
	-rm $(EXEC)

all: mrproper $(EXEC)

""")

makefile.close();

ret = os.system("make -f %s" % makefile_name)
if ret == 0:
    print "done :)"
else:
   "le compilo s'est vautré comme une loutre bourrée"

