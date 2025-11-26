#ifndef ERPROC_H
#define ERPROC_H
// include stobi weror ne jalovalsya
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>

int Socket(int domain, int type, int protocol);

void Bind(int sockfd, const struct sockaddr_in *addr, socklen_t addrlen);

void Listen(int socket, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addr_len);

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Inet_pton(int af, const char *restrict src, void *restrict dst);

ssize_t Read(int fd, void *buf, size_t count);

int Epoll_create1(int __flags);

int Epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

int Epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

#endif