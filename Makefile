CPPFLAGS=-std=c++11
CC=g++

all: rogue_experiment

rogue_experiment: main.o playerManager.o player.o
	$(CC) -o rogue_experiment main.o playerManager.o player.o $(CPPFLAGS)

main.o: main.cpp playerManager.h player.h
	$(CC) $(CPPFLAGS) -c main.cpp

playerManager.o: playerManager.cpp playerManager.h
	$(CC) $(CPPFLAGS) -c playerManager.cpp

player.o: player.cpp player.h
	$(CC) $(CPPFLAGS) -c player.cpp

.PHONY: clean

clean:
	rm -f *.o
