#include "errproc.h"
#include <arpa/inet.h> //htonl,  htons,  ntohl,  ntohs
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h> //socket, listen, acept
#include <sys/types.h>  //
#include <unistd.h>     //

#define port 34521
#define MAX_EVENTS 111
#define BUFFER_SIZE 1024

int main() {
  int count_users = 0;
  int epoll_fd; // try))))
  int server = Socket(AF_INET, SOCK_STREAM, 0);
  struct epoll_event event, events[MAX_EVENTS];
  struct sockaddr_in adr = {
      0}; // vse polya proinitsializirovani 0 "0.0.0.0" - all ip to lisen
  adr.sin_family = AF_INET;
  adr.sin_port = htons(port);
  adr.sin_addr.s_addr = htonl(INADDR_ANY);
  Bind(server, &adr, sizeof adr);
  Listen(server, MAX_EVENTS); // que max 5->111
  printf("Server listening on port %d ...\n", port);
  epoll_fd = Epoll_create1(0);
  event.events = EPOLLIN;
  event.data.fd = server;
  printf("Epoll initialized. Waiting......\n");
  while (1) {
    int nfds = Epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    printf("Received %d events\n", nfds);
    for (int i = 0; i < nfds; i++) {

      if (events[i].data.fd == server) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_fd =
            Accept(server, (struct sockaddr *)&client_addr, &client_len);
        count_users++;
        printf("New client connected! fd=%d, Online:%d\n", client_fd,
               count_users);
        event.events = EPOLLIN;
        event.data.fd = client_fd;
        Epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
        char *welcome = "Подключение к серверу успешно!\n\n"
                        "Вы можете:\n"
                        "• Использовать команды через '/'\n\n"
                        "Используйте \"/help\" для отображения списка команд\n"
                        "Используйте \"/exit\" чтобы выйти\n\n";
        write(client_fd, welcome, strlen(welcome));

      } else {

        int client_fd = events[i].data.fd;
        char buf[BUFFER_SIZE];

        ssize_t nread = read(client_fd, buf, BUFFER_SIZE - 1);

        if (nread > 0) {
          buf[nread] = '\0';
          printf("Received from client %d: %s", client_fd, buf);

          write(client_fd, buf, nread);

        } else if (nread == 0) {
          printf("Client %d disconnected  online: %d \n", client_fd,
                 count_users - 1);
          close(client_fd);
          count_users--;
        } else {
          perror("read");
          close(client_fd);
          printf("Client %d disconnected  online: %d \n", client_fd,
                 count_users - 1);
          count_users--;
        }
      }
    }
  }

  close(server);
  close(epoll_fd);
  return 0;
}

//   struct sockaddr_in client_addr;
//   socklen_t client_len = sizeof(client_addr);
//   int fd = Accept(server, (struct sockaddr *)&client_addr, &client_len);
//   printf("Client connected!\n");
//   ssize_t nread;
//   char buf[256];
//   nread = read(fd, buf, 255);
//   if (nread == -1) {
//     perror("read failed");
//     exit(EXIT_FAILURE);
//   }
//   if (nread == 0) {
//     printf("END OF FILE\n");
//   }
//   write(STDOUT_FILENO, buf, nread);
//   sleep(15);
//   close(fd);
//   close(server);
// }