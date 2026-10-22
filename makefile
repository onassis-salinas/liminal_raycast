CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c17 $(shell pkg-config --cflags sdl3)
LDLIBS = $(shell pkg-config --libs sdl3) -lm

BIN = build/game
SRC = src/main.c src/render.c src/walls.c src/maps.c src/textures.c

$(BIN): $(SRC) src/game.h | build
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LDLIBS)

build:
	mkdir -p build

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)

.PHONY: run clean
