CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iinclude
LDFLAGS = -lreadline -lncurses  # link bibliotek potomuchto podrugomu dont work()

all: server client

server: src/server.c include/errproc.c
	$(CC) $(CFLAGS) -o server src/server.c include/errproc.c $(LDFLAGS) 

server-ignore: src/server.c include/errproc.c
	$(CC) -o server src/server.c include/errproc.c -Iinclude $(LDFLAGS) 

client: src/client.c include/repl.c include/errproc.c  
	$(CC) $(CFLAGS) -o client src/client.c include/repl.c include/errproc.c $(LDFLAGS)  

client-ignore: src/client.c include/repl.c include/errproc.c  
	$(CC) -o client src/client.c include/repl.c include/errproc.c -Iinclude $(LDFLAGS)  
clean:
	rm -f server client

rebuild: clean all

.PHONY: all clean rebuild