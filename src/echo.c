#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("usage: argc < 3\n");
    return -1;
  }

  char *ip = argv[1];
  uint16_t port = atoi(argv[2]);
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  assert(listen_fd > 0);

  struct sockaddr_in sock;
  memset(&sock, 0, sizeof(sock));
  sock.sin_family = AF_INET;
  sock.sin_port = htons(port);
  
  inet_pton(AF_INET, ip, &sock.sin_addr);

  int ret = bind(listen_fd, (struct sockaddr*)&sock, sizeof(sock));

  assert(ret != -1);

  ret = listen(listen_fd, 5);
  assert(ret != -1);

  struct sockaddr_in client;
  socklen_t client_len = sizeof(client);
  int sockfd = accept(listen_fd, (struct sockaddr *)&client, &client_len);
  assert(sockfd > 0);
  char buf[128];
  int receive_size = 0;
  memset(buf, 0 ,128);
  receive_size = recv(sockfd, buf, 128, 0);
  send(sockfd, buf, receive_size, 0);

  close(sockfd);
  close(listen_fd);
  return 0;
}