CC=gcc
CFLAGS=-Wall -Wextra -Werror -g

.PHONY: build clean

all: build

build: friends posts feed

friends: clean
	$(CC) $(CFLAGS) -D TASK_1 -O0  src/*.c src/*.h src/**/*.c src/**/*.h -o $@

posts: clean
	$(CC) $(CFLAGS) -D TASK_2 -O0 src/*.c src/*.h src/**/*.c src/**/*.h -o $@

feed: clean
	$(CC) $(CFLAGS) -D TASK_1 -D TASK_2 -D TASK_3 -O0 src/*.c src/*.h src/**/*.c src/**/*.h -o $@

clean:
	rm -rf *.o friends posts feed