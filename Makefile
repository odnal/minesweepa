CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Wpedantic -O2 -g
LDFLAGS = -lraylib -lGL -lm -lc -lpthread -ldl
PATHS = -I./raylib
LDPATHS = -L./raylib

all: main

main: main.o
	$(CC) $(PATHS)  main.o -o main $(LDFLAGS) $(LDPATHS)

main.o: main.c
	$(CC) $(CFLAGS) $(PATHS) -c main.c -o main.o $(LDPATHS) $(LDFLAGS)

clean:
	rm *.o
	rm main
