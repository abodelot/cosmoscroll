CC=g++ #-DNO_AUDIO
TINYXML_DIR=tinyxml
TINYXML_OBJ=$(TINYXML_DIR)/tinyxml.o $(TINYXML_DIR)/tinyxmlerror.o $(TINYXML_DIR)/tinyxmlparser.o

FLAGS=-Wall -Wextra -Wwrite-strings -pedantic -ansi
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
EXEC=cosmoscroll
DEBUG=yes

SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)

ifeq ($(DEBUG), yes)
	CFLAGS=$(FLAGS) -g -DDEBUG
else
	CFLAGS=$(FLAGS) -O3
endif


$(EXEC): $(TINYXML_OBJ) $(OBJ)
	$(CC) $^ -o $(EXEC) $(LDFLAGS)

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

