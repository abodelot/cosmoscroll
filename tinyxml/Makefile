
# this is an auto-generated makefile
CC=g++
FLAGS=-Wall -Wextra -Wwrite-strings -pedantic
LDFLAGS=
DEBUG=yes
ifeq ($(DEBUG), yes)
	CFLAGS=$(FLAGS) -g -DDEBUG
else
	CFLAGS=$(FLAGS) -O3
endif


tinyxml.o: tinyxml.cpp tinyxml.h
	$(CC) $< -c $(CFLAGS)

tinyxmlparser.o: tinyxmlparser.cpp
	$(CC) $< -c $(CFLAGS)

tinyxmlerror.o: tinyxmlerror.cpp
	$(CC) $< -c $(CFLAGS)

tinystr.o: tinystr.cpp
	$(CC) $< -c $(CFLAGS)


clean:
	-rm *.o

mrproper: clean

all: mrproper tinyxml.o tinyxmlparser.o tinyxmlerror.o tinystr.o

