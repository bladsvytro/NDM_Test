#include "errproc.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define port 34521 // random
#define MAX_EVENTS 111
#define BUFFER_SIZE 1024

int main() {
  int count_users = 0;
  int epoll_fd;
  int server = Socket(AF_INET, SOCK_STREAM, 0);
  struct epoll_event event, events[MAX_EVENTS];
  struct sockaddr_in adr = {0};
  adr.sin_family = AF_INET;
  adr.sin_port = htons(port);
  adr.sin_addr.s_addr = htonl(INADDR_ANY);
  Bind(server, &adr, sizeof adr);
  Listen(server, MAX_EVENTS);
  printf("Server listening on port %d ...\n", port);
  epoll_fd = Epoll_create1(0);
  event.events = EPOLLIN;
  event.data.fd = server;
  Epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server, &event);
  printf("Epoll initialized. Waiting......\n");

  while (1) {
    int nfds = Epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

    for (int i = 0; i < nfds; i++) {
      if (events[i].data.fd == server) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd =
            Accept(server, (struct sockaddr *)&client_addr, &client_len);

        if (client_fd < 0) {
          perror("Accept failed");
          continue;
        }

        count_users++;
        printf("New client connected! fd=%d, Online:%d, IP:%s\n", client_fd,
               count_users, inet_ntoa(client_addr.sin_addr));
        char *welcome = "Connected to server!\nUse /help for commands\n";
        write(client_fd, welcome, strlen(welcome));
        event.events = EPOLLIN;
        event.data.fd = client_fd;
        Epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);

      } else {
        int client_fd = events[i].data.fd;
        char buf[BUFFER_SIZE];
        ssize_t nread = read(client_fd, buf, BUFFER_SIZE - 1);

        if (nread > 0) {
          buf[nread] = '\0';

          if (buf[nread - 1] == '\n') {
            buf[nread - 1] = '\0';
          }

          printf("Client %d: %s\n", client_fd, buf);

          if (strcmp(buf, "TIME") == 0) {
            time_t raw_time;
            struct tm *time_info;
            char time_buffer[64];
            time(&raw_time);
            time_info = localtime(&raw_time);
            strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S",
                     time_info);
            write(client_fd, time_buffer, strlen(time_buffer));
          } else if (strcmp(buf, "STATS") == 0) {
            char stats_buffer[128];
            snprintf(stats_buffer, sizeof(stats_buffer),
                     "Total connected: %d\nCurrently online: %d", count_users,
                     count_users);
            write(client_fd, stats_buffer, strlen(stats_buffer));
          } else if (strcmp(buf, "SHUTDOWN") == 0) {
            write(client_fd, "Server shutdown", 15);
          } else if (strcmp(buf, "EXIT") == 0) {
            write(client_fd, "Goodbye!", 8);
            close(client_fd);
            count_users--;
          } else {
            write(client_fd, buf, strlen(buf));
          }
        } else if (nread == 0) {
          printf("Client %d disconnected. Online: %d\n", client_fd,
                 count_users - 1);
          close(client_fd);
          count_users--;
        } else {
          printf("Client %d error. Online: %d\n", client_fd, count_users - 1);
          close(client_fd);
          count_users--;
        }
      }
    }
  }

  close(server);
  close(epoll_fd);
  return 0;
}