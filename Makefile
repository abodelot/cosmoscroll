CC=g++
CFLAGS=-Wall -Wextra -Wwrite-strings -pedantic -ansi
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -ldumb
EXEC=cosmoscroll
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)

# tinyxml
TINYXML_OBJ= tinyxml/tinyxml.o tinyxml/tinyxmlparser.o tinyxml/tinyxmlerror.o

# static/dynamic linking
LINK=dynamic
ifeq ($(LINK), dynamic)
	CFLAGS += -DSFML_DYNAMIC
endif

# debug/release mode
DEBUG=yes
ifeq ($(DEBUG), yes)
	CFLAGS += -g -DDEBUG
else
	CFLAGS += -O2
endif

# disable audio
NO_AUDIO=no
ifeq ($(NO_AUDIO), yes)
	CFLAGS += -DNO_AUDIO
endif

# svn revision
SVNDEF= -DSVN_REV="\"$(shell svnversion -n .)\""
CFLAGS += $(SVNDEF)

$(EXEC): $(OBJ) $(TINYXML_OBJ)
	$(CC) $^ -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CC) $< -c $(CFLAGS)

tinyxml/tinyxml.o: tinyxml/tinyxml.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

tinyxml/tinyxmlparser.o: tinyxml/tinyxmlparser.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

tinyxml/tinyxmlerror.o: tinyxml/tinyxmlerror.cpp
	$(CC) $< -c -o $@ $(CFLAGS)


.PHONY: clean cleanxml mrproper

clean:
	-rm *.o

cleanxml:
	-rm tinyxml/*.o

mrproper: clean
	-rm $(EXEC)

all: mrproper $(EXEC)

