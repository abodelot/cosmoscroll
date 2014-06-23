TARGET   := bin/cosmoscroll
SRCDIR   := src
SRC      := $(shell find $(SRCDIR) -name "*.cpp" -type f)
OBJDIR   := obj
OBJ      := $(SRC:%.cpp=$(OBJDIR)/%.o)

CC       := g++
CFLAGS   := -I$(SRCDIR) -std=c++11 -pedantic -Wall -Wextra -Wwrite-strings -O2
UNAME    := $(shell uname)
# Linker requires a different syntax on MacOS
ifeq (UNAME, "Darwin")
	LDFLAGS := -framework sfml-graphics -framework sfml-window -framework sfml-system -framework sfml-audio -framework sfml-network -ldumb
else
	LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -ldumb
endif


$(TARGET): $(OBJ)
	@echo "\033[1;33mlinking\033[0m $@"
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "\033[1;32mDone!\033[0m"

$(OBJDIR)/%.o: %.cpp
	@echo "\033[1;33mcompiling\033[0m $<"
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "\033[1;33mremoving\033[0m $(OBJDIR)"
	-@rm -r $(OBJDIR)

mrproper: clean
	@echo "\033[1;33mremoving\033[0m $(TARGET)"
	-@rm $(TARGET)

all: mrproper $(TARGET)

