#include <arpa/inet.h> //htonl,  htons,  ntohl,  ntohs
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //socket, listen, acept
#include <sys/types.h>  //
#include <unistd.h>     //
#include "errproc.h"

int main() {
  int server = Socket(AF_INET, SOCK_STREAM, 0); // protocol TCP IpV4
  struct sockaddr_in adr = {
      0}; // vse polya proinitsializirovani 0 "0.0.0.0" - all ip to lisen
  adr.sin_family = AF_INET;
  adr.sin_port = htons(34521); // host to network short
  adr.sin_addr.s_addr = htonl(INADDR_ANY);
  Bind(server, &adr, sizeof adr);
  Listen(server, 5); // que max 5
  printf("Server listening on port 34521...\n");
  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  int fd = Accept(server, (struct sockaddr *)&client_addr, &client_len);
  printf("Client connected!\n");
  ssize_t nread;
  char buf[256];
  nread = read(fd, buf, 255);
  if (nread == -1) {
    perror("read failed");
    exit(EXIT_FAILURE);
  }
  if (nread == 0) {
    printf("END OF FILE\n");
  }
  write(STDOUT_FILENO, buf, nread);
  sleep(15);
  close(fd);
  close(server);
}