CC = g++
CXXFLAGS = -Wall -std=c++11 -I.

SOURCE = ./src/*.cc

all: Proteus

Proteus: $(SOURCE)
	$(CC) $(SOURCE) $(CXXFLAGS) -o $@

run:
	./Proteus

clean:
	rm Proteus
	