CC = g++
CFLAGS = -std=c++2b -Wno-parentheses -Iinclude/ -Idoc/

SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst src/%.cpp, obj/%.o, $(SOURCES))

All: build

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

build: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o bin/is$(CC)