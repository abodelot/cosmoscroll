CC=g++
LD=ld
TINYXML_DIR=tinyxml
TINYXML_OBJ=$(TINYXML_DIR)/tinyxml.o $(TINYXML_DIR)/tinyxmlerror.o $(TINYXML_DIR)/tinyxmlparser.o

CFLAGS=-Wall -Wextra -Wwrite-strings -pedantic -ansi -DSFML_DYNAMIC
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -ldumb
EXEC=cosmoscroll
SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)

# static/dynamic linking
LINK=dynamic
ifeq ($(LINK), dynamic)
	CFLAGS += -DSFML_DYNAMIC
else
	
endif

# debug/release mode
DEBUG=yes
ifeq ($(DEBUG), yes)
	CFLAGS += -g -DDEBUG
else
	CFLAGS += -O2
endif

# enable/disable joystick
JOY_ENABLED=yes
ifeq ($(JOY_ENABLED), yes)
	CFLAGS += -DJOY_ENABLED
endif

# disable audio
NO_AUDIO=yes
ifeq ($(NO_AUDIO), yes)
	CFLAGS += -DNO_AUDIO
endif

#disanble music
NO_MUSIC=no
ifeq ($(NO_MUSIC), yes)
	CFLAGS += -DNO_MUSIC
endif

# svn revision
SVNDEF= -DSVN_REV="\"$(shell svnversion -n .)\""
CFLAGS += $(SVNDEF)

$(EXEC): $(TINYXML_OBJ) $(OBJ)
	$(CC) $(TINYXML_OBJ) $(OBJ) -o $(EXEC) $(LDFLAGS)

$(TINYXML_OBJ):
	@(cd $(TINYXML_DIR) && $(MAKE))

%.o: %.cpp
	$(CC) $< -c -o $@ $(CFLAGS)


.PHONY: clean mrproper

clean:
	-rm *.o

mrproper: clean
	-rm $(EXEC)

all: mrproper $(EXEC)

