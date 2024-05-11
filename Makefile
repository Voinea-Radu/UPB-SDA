CC=gcc
CFLAGS=-Wall -Wextra -Werror -g

.PHONY: build clean

all: build

build: friends posts feed

# Any other object file that is needed for the other files
# add it here, separated by space
UTILS = users.o

friends: $(UTILS) friends.o social_media_friends.o
	$(CC) $(CFLAGS) -o $@ $^

posts: $(UTILS) posts.o social_media_posts.o
	$(CC) $(CFLAGS) -o $@ $^
	
feed: $(UTILS) posts.o friends.o feed.o social_media_feed.o
	$(CC) $(CFLAGS) -o $@ $^

social_media_friends.o:
	$(CC) $(CFLAGS) -c -D TASK_1 -o $@ src/social_media.c

social_media_posts.o:
	$(CC) $(CFLAGS) -c -D TASK_2 -o $@ src/social_media.c

social_media_feed.o:
	$(CC) $(CFLAGS) -c -D TASK_1 -D TASK_2 -D TASK_3 -o $@ src/social_media.c

users.o:
	$(CC) $(CFLAGS) -c -o $@ src/utils/users.c

friends.o:
	$(CC) $(CFLAGS) -c -o $@ src/friends.c

posts.o:
	$(CC) $(CFLAGS) -c -o $@ src/posts.c

feed.o:
	$(CC) $(CFLAGS) -c -o $@ src/feed.c

clean:
	rm -rf *.o friends posts feed