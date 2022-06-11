CC = g++
CFLAGS = -std=c++2b -Wno-parentheses -Iinclude/ -Idoc/

SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst src/%.cpp, bin/%.o, $(SOURCES))

All: build

bin/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

build: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o bin/is$(CC)