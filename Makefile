# Copyright 2023 Echipa PCLP1

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -g3 -O3

# targets
TARGETS=tema2

all: $(TARGETS)

build: $(TARGETS)

tema2: src/main.c
	$(CC) $(CFLAGS) src/api/*.h src/*.c -lm -o tema2

pack:
	zip -FSr 315CA_VoineaRadu_TEMA1.zip README Makefile src/

clean:
	rm -f $(TARGETS)

.PHONY: all clean run build pack