#include <arpa/inet.h>
#include <errno.h> //erors
#include <errproc.h>
#include <stdio.h>
#include <stdlib.h>
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