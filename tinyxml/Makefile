CC=g++
CFLAGS=-Wall -Wextra -Wwrite-strings -O2

TinyXML: tinyxml.o tinyxmlparser.o tinyxmlerror.o

tinyxml.o: tinyxml.cpp tinyxml.h
	$(CC) $< -c $(CFLAGS)

tinyxmlparser.o: tinyxmlparser.cpp tinyxml.h
	$(CC) $< -c $(CFLAGS)

tinyxmlerror.o: tinyxmlerror.cpp tinyxml.h
	$(CC) $< -c $(CFLAGS)

clean:
	-rm *.o

