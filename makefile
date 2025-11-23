CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: server client

server: src/server.c
	$(CC) $(CFLAGS) -o server src/server.c

client: src/client.c
	$(CC) $(CFLAGS) -o client src/client.c

clean:
	rm -f server client

#format: dont work with clamg-format in materials i use standart format download in arch yay (perezapisivay v ruchnuy)

.PHONY: all clean format