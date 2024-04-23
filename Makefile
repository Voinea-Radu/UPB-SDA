# Copyright 2023 Echipa PCLP1

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -g3

# targets
TARGETS=tema2

all: $(TARGETS)

build: $(TARGETS)

tema2: src/main.c
	$(CC) $(CFLAGS) src/**/**/*.c src/**/*.c src/*.c src/**/*.h src/*.h -lm -o tema2

pack:
	zip -FSr 315CA_VoineaRadu_TEMA1.zip README Makefile src/

clean:
	rm -f $(TARGETS)

.PHONY: all clean run build pack