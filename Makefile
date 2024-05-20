CC=gcc
CFLAGS=-Wall -Wextra -g3

.PHONY: build clean

all: build

build: friends

friends: clean
	$(CC) $(CFLAGS) -D TASK_1 src/*.c src/*.h src/**/*.c src/**/*.h src/**/**/*.c src/**/**/*.h -o $@

clean:
	rm -rf *.o friends