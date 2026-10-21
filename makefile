CC = gcc
CFLAGS = -Wall -Wextra -std=c17 $(shell pkg-config --cflags sdl3)
LDLIBS = $(shell pkg-config --libs sdl3) -lm

BIN = build/game
# main.c #includes the other .c files, so only it is compiled,
# but every source must be a dependency or make skips the rebuild.
SRC = src/main.c
DEPS = $(wildcard src/*.c)

$(BIN): $(DEPS) | build
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LDLIBS)

build:
	mkdir -p build

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)

.PHONY: run clean
