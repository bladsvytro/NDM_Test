#include <arpa/inet.h>
// #include <errproc.h> // заголовочный
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h> //socket, listen, acept
#include <sys/types.h>  //
#include <unistd.h>     //

int Socket(int domain, int type, int protocol) {
  int res = socket(domain, type, protocol);
  if (res == -1) {
    perror("Socket failure");
    exit(EXIT_FAILURE);
  }
  return res;
}

void Bind(int sockfd, const struct sockaddr_in *addr, socklen_t addrlen) {
  int res = bind(sockfd, (struct sockaddr *)addr, addrlen);
  if (res == -1) {
    perror("Bind failure");
    exit(EXIT_FAILURE);
  }
}

void Listen(int socket, int backlog) {
  int res = listen(socket, backlog);
  if (res == -1) {
    perror("listen failure");
    exit(EXIT_FAILURE);
  }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addr_len) {
  int res = accept(sockfd, addr, addr_len);
  if (res == -1) {
    perror("accept failed");
    exit(EXIT_FAILURE);
  }
  return res;
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  int res = connect(sockfd, addr, addrlen);
  if (res == -1) {
    perror("Connect failed");
    exit(EXIT_FAILURE);
  }
}

void Inet_pton(int af, const char *restrict src, void *restrict dst) {
  int res = inet_pton(af, src, dst);
  if (res == -1) {
    perror(" failed");
    exit(EXIT_FAILURE);
  }
  if (res == 0) {
    printf("inet_pton failed: src is empty"
           "src does not contain a character string representing a valid "
           "network address in the specified ad‐"
           "dress family.  If af does not contain a valid address family");
  }
}

ssize_t Read(int fd, void *buf, size_t count) {
  ssize_t nread = read(fd, buf, count);

  if (nread == -1) {
    perror("read failure");
    exit(EXIT_FAILURE);
  }
  if (nread == 0) {
    printf("EOF occurred\n");
  }
  return nread;
}

int Epoll_create1(int __flags) {
  int res = epoll_create1(__flags);
  if (res == -1) {
    perror("epoll_create1 failure");
    exit(EXIT_FAILURE);
  }
  return res;
}

// Управление epoll (добавление/удаление/изменение сокетов)
int Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event) {
  int res = epoll_ctl(epfd, op, fd, event);
  if (res == -1) {
    perror("Epoll_ctl failure");
    exit(EXIT_FAILURE);
  }
  return res;
}

// Ожидание событий epoll
int Epoll_wait(int epfd, struct epoll_event *events, int maxevents,
               int timeout) {
  int res = epoll_wait(epfd, events, maxevents, timeout);
  if (res == -1) {
    perror("Epoll_wait failure");
    exit(EXIT_FAILURE);
  }
  return res;
}