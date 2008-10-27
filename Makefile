
# this is an auto-generated makefile
CC=g++
FLAGS=-Wall -Wextra -Wwrite-strings -pedantic -ansi
TINYXMLFILES= tinyxml/tinyxml.o tinyxml/tinyxmlerror.o tinyxml/tinyxmlparser.o tinyxml/tinystr.o
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
EXEC=cosmoscroll
DEBUG=yes
ifeq ($(DEBUG), yes)
	CFLAGS=$(FLAGS) -g -DDEBUG
else
	CFLAGS=$(FLAGS) -O3
endif



$(EXEC): Asteroid.o Blorb.o BulletManager.o ControlPanel.o Ennemy.o Entity.o Game.o  Main.o MediaManager.o Menu.o ParticleSystem.o PlayerShip.o Weapon.o Level.o Misc.o
	$(CC) $^ $(TINYXMLFILES) -o $(EXEC) $(LDFLAGS)


Asteroid.o: Asteroid.cpp Asteroid.hpp
	$(CC) $< -c $(CFLAGS)

Blorb.o: Blorb.cpp Blorb.hpp
	$(CC) $< -c $(CFLAGS)

BulletManager.o: BulletManager.cpp BulletManager.hpp
	$(CC) $< -c $(CFLAGS)

ControlPanel.o: ControlPanel.cpp ControlPanel.hpp
	$(CC) $< -c $(CFLAGS)

Ennemy.o: Ennemy.cpp Ennemy.hpp
	$(CC) $< -c $(CFLAGS)

Entity.o: Entity.cpp Entity.hpp
	$(CC) $< -c $(CFLAGS)

Game.o: Game.cpp Game.hpp
	$(CC) $< -c $(CFLAGS)

Main.o: Main.cpp
	$(CC) $< -c $(CFLAGS)

MediaManager.o: MediaManager.cpp MediaManager.hpp
	$(CC) $< -c $(CFLAGS)

Menu.o: Menu.cpp Menu.hpp
	$(CC) $< -c $(CFLAGS)

Misc.o: Misc.cpp Misc.hpp
	$(CC) $< -c $(CFLAGS)

ParticleSystem.o: ParticleSystem.cpp ParticleSystem.hpp
	$(CC) $< -c $(CFLAGS)

PlayerShip.o: PlayerShip.cpp PlayerShip.hpp
	$(CC) $< -c $(CFLAGS)

Weapon.o: Weapon.cpp Weapon.hpp
	$(CC) $< -c $(CFLAGS)

Level.o: Level.cpp Level.hpp
	$(CC) $< -c $(CFLAGS)


clean:
	-rm *.o

mrproper: clean
	-rm $(EXEC)

all: mrproper $(EXEC)

