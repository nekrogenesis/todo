CC         = gcc
SRC        = $(wildcard src/*.c)
BIN        = todo
FLAGS      = -Wall
LIBS 	   =
DEBUG      = -g3
DFLAGS     = -Wall -Wextra -Wpedantic

build:
	$(CC) $(SRC) -o $(BIN) $(DEBUG) $(FLAGS) $(LIBS)

debug:
	$(CC) $(SRC) -o $(BIN) $(DEBUG) $(DFLAGS) $(LIBS)