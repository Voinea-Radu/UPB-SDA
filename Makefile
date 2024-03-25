# Copyright 2023 Echipa PCLP1

# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -g3

# define targets
TARGETS = sfl

build: $(TARGETS)

sfl: src/main.c
	$(CC) $(CFLAGS) src/api/state_manager.h src/api/string_utils.h src/api/linked_list.h src/api/utils.h src/api/heap.h src/state_manager.c src/string_utils.c src/linked_list.c src/utils.c src/heap.c src/main.c -lm -o sfl

pack:
	zip -FSr 315CA_VoineaRadu_TEMA3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean