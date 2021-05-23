CFLAGS = -std=c99 -g -Wall -O3
CC=clang

default: all

all: card_shuffling

card_shuffling: card_shuffling.c
	$(CC) $(CFLAGS) -o card_shuffling $< 

clean:
	rm card_shuffling
