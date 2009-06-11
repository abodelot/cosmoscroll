CC=g++
CFLAGS= -Wall -Wextra -Wwrite-strings -ansi -pedantic
LDFLAGS= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -ldumb
EXEC=bin/cosmoscroll
SRC= $(wildcard src/*/*.cpp)
OBJ= $(SRC:.cpp=.o)

# debug/release mode
DEBUG=yes
ifeq ($(DEBUG), yes)
	CFLAGS += -g -DDEBUG
else
	CFLAGS += -O2
endif

# static/dynamic linking
LINK=dynamic
ifeq ($(LINK), dynamic)
	CFLAGS += -DSFML_DYNAMIC
endif

# disable audio
NO_AUDIO=no
ifeq ($(NO_AUDIO), yes)
	CFLAGS += -DNO_AUDIO
endif

# svn revision
SVNDEF= -DSVN_REV="\"$(shell svnversion -n .)\""
CFLAGS += $(SVNDEF)


$(EXEC): $(OBJ) 
	$(CC) $^ -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CC) $< -c -o $@ $(CFLAGS)

.PHONY: clean cleanxml mrproper

clean:
	-rm src/core/*.o
	-rm src/utils/*.o
	-rm src/menu/*.o
	-rm src/entities/*.o

cleanxml:
	-rm src/tinyxml/*.o
	
mrproper: clean
	-rm $(EXEC)

all: mrproper $(EXEC)

