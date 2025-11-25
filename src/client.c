#include "errproc.h"
#include <arpa/inet.h> //htonl,  htons,  ntohl,  ntohs
#include <errno.h>     //erors
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> //socket, listen, acept
#include <sys/types.h>  //
#include <unistd.h>     //
#include "errproc.h"


int main (){

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in adr = {0};
  adr.sin_family = AF_INET;
  adr.sin_port = htons(34521);
  Inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);//include/errproc.h
  Connect(fd, (struct sockaddr *)&adr, sizeof(adr));//include/errproc.h
  printf("Подключение к серверу успешно!\n\n"
       "Вы можете:\n"
       "• Использовать команды через '/'\n\n"
       "Используйте \"/help\" для отображения списка команд\n"
       "Используйте \"/exit\" чтобы выйти\n\n");
  write(fd, "VSEM SOSAT\n", 10);
  char buf[256];
  ssize_t nread;
  nread = read(fd, buf, 255); if (nread == -1) {
    perror("readfailure");
    exit(EXIT_FAILURE);
  }
  if (nread == 0) {
    printf("EOF:occured\n");
  }
  close(fd);

  return 0;
}