CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude

all: server client

server: src/server.c include/errproc.c
	$(CC) $(CFLAGS) -o server src/server.c include/errproc.c

client: src/client.c
	$(CC) $(CFLAGS) -o client src/client.c include/errproc.c

clean:
	rm -f server client

format:
	find src include -name "*.c" -o -name "*.h" | xargs clang-format -i -style=file:materials/.clang-format

.PHONY: all clean format