CC=gcc
CFLAGS=-Wall -Wextra

LOAD=load_balancer
SERVER=server
CACHE=lru_cache
UTILS=utils

# Add new source file names here:
EXTRA=replication

.PHONY: build clean

build: tema2

tema2: main.o $(LOAD).o $(SERVER).o $(CACHE).o $(UTILS).o # $(EXTRA).o
	$(CC) $^ -o $@

main.o: src/main.c
	$(CC) $(CFLAGS) $^ -c

$(LOAD).o: src/$(LOAD).c src/api/$(LOAD).h
	$(CC) $(CFLAGS) $^ -c

$(SERVER).o: src/$(SERVER).c src/api/$(SERVER).h
	$(CC) $(CFLAGS) $^ -c

$(CACHE).o: src/$(CACHE).c src/api/$(CACHE).h
	$(CC) $(CFLAGS) $^ -c

$(UTILS).o: src/$(UTILS).c src/api$(UTILS).h
	$(CC) $(CFLAGS) $^ -c

$(EXTRA).o: src/$(EXTRA).c src/api/$(EXTRA).h
	$(CC) $(CFLAGS) $^ -c

clean:
	rm -f *.o tema2 *.h.gch
