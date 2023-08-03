#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include "threadpool.h"
#include "task_args.h"
#include "http_test.h"

#define MAX_EVENTS 5
#define BUFFER_SIZE 10

int set_nonblocking(int fd)
{
  int old_state = fcntl(fd, F_GETFL);
  int new_state = old_state | O_NONBLOCK;
  fcntl(fd, F_SETFL, new_state);
  return old_state;
}

void reset_oneshot(int epollfd, int fd)
{
  struct epoll_event event;
  event.data.fd = fd;
  event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
  epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

void addfd(int epollfd, int fd, bool oneshot)
{
  struct epoll_event event;
  event.events = EPOLLIN | EPOLLET;
  if (oneshot)
  {
    event.events |= EPOLLONESHOT;
  }
  event.data.fd = fd;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
  set_nonblocking(fd);
}

void accept_connction(int listen_fd, int epollfd)
{
  struct sockaddr_in client;
  socklen_t client_len = sizeof(client);
  int fd = accept(listen_fd, (struct sockaddr *)&client, &client_len);
  char ip[15];
  memset(ip, 0, 15);
  inet_ntop(AF_INET, &client.sin_addr, ip, sizeof(client));
  printf("ip: %s prot: %d, fd: %d connect\n", ip, ntohs(client.sin_port), fd);
  addfd(epollfd, fd, true);
}

void *echo_server_main(void *arg)
{
  char buf[BUFFER_SIZE];
  task_args_t *args = (task_args_t *)(arg);
  int epollfd = args->epollfd;
  int eventfd = (int)(args->arg);
  while (1) // epoll et 循环
  {
    memset(buf, 0, BUFFER_SIZE);
    int receive_byte = recv(eventfd, buf, BUFFER_SIZE, 0);

    if (receive_byte < 0)
    {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
      {
        reset_oneshot(epollfd, eventfd);
        break;
      }
      if (errno == EINTR)
      {
        continue;
      }
      printf("fd: %d error occurred\n", eventfd);
      close(eventfd);
      break;
    }
    else if (receive_byte == 0)
    {
      printf("fd %d disconnect\n", eventfd);
      close(eventfd);
      break;
    }
    else
    {
      printf("receive data from fd: %d, data: %s\n", eventfd, buf);
      send(eventfd, buf, receive_byte, 0);
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("usage: argc < 3\n");
    return 0;
  }

  char *ip = argv[1];
  uint16_t port = atoi(argv[2]);
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  inet_pton(AF_INET, ip, (void *)&server.sin_addr);

  int ret = bind(listen_fd, (struct sockaddr *)&server, sizeof(server));
  assert(ret != -1);
  ret = listen(listen_fd, 500);
  assert(ret != -1);

  struct epoll_event events[MAX_EVENTS];
  int epollfd = epoll_create(500);
  addfd(epollfd, listen_fd, false);
  threadpool_t *pool = threadpool_init(10);
  while (1)
  {
    int n = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (n < 0)
    {
      printf("epoll wait error\n");
      return -1;
    }
    for (int i = 0; i < n; i++)
    {
      int eventfd = events[i].data.fd;
      if (eventfd == listen_fd)
      {
        printf("accept_connection\n");
        accept_connction(listen_fd, epollfd);
      }
      else
      {
        if (events[i].events & EPOLLIN)
        {
          printf("-----start task----\n");
          // task_args_t *args = (task_args_t *)malloc(sizeof(task_args_t));
          // args->epollfd = epollfd;
          // args->arg = (void *)eventfd;
          // task_t *task = task_create(echo_server_main, (void *)args);
          task_t *task = task_create(accept_request, (void *)eventfd);
          threadpool_append(pool, task);
        }
      }
    }
  }
  close(listen_fd);
  return 0;
}