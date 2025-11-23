# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Цели
all: server client

# Сборка сервера
server: src/server.c
	$(CC) $(CFLAGS) -o server src/server.c

# Сборка клиента
client: src/client.c
	$(CC) $(CFLAGS) -o client src/client.c
	
# Пересборка
rebuild: clean all

# Очистка
clean:
	rm -f server client

.PHONY: all task rebuild clean