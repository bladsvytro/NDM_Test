#include "errproc.h"
#include "repl.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUFFER_SIZE 256

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(34521);
    Inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);
    
    Connect(fd, (struct sockaddr *)&adr, sizeof(adr));
    client_commands_init();

    while (1) {

        char* input = readline("> ");
        
        if (input == NULL) {
            printf("\nЗавершение работы...\n");
            break;
        }

        if (strlen(input) == 0) {
            free(input);
            continue;
        }
        
        // Добавляем в историю
        add_history(input);
        

        if (client_handle_command(input)) {
            if (strcmp(input, "/exit") == 0) {
                write(fd, "EXIT", 4);
                free(input);
                break;
            }
            else if (strcmp(input, "/time") == 0) {
                write(fd, "TIME", 4);
            }
            else if (strcmp(input, "/stats") == 0) {
                write(fd, "STATS", 5);
            }
            else if (strcmp(input, "/shutdown") == 0) {
                write(fd, "SHUTDOWN", 8);
            }
            // /help обрабатывается локально
            else {
                free(input);
                continue;
            }
        } else {
            write(fd, input, strlen(input));
        }
        
        free(input);

        char buf[BUFFER_SIZE];
        ssize_t nread;
        nread = Read(fd, buf, BUFFER_SIZE - 1);
        if (nread > 0) {
            buf[nread] = '\0';
            printf("Сервер: %s\n", buf);
        } else if (nread == 0) {
            printf("Сервер закрыл соединение\n");
            break;
        }
    }

    close(fd);
    return 0;
}