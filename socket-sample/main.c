#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define PORT 8000

int main()
{
  int fd_server, fd_client, on = 1;
  struct sockaddr_in addr_server = {
    .sin_family = AF_INET,
    .sin_addr.s_addr = htonl(INADDR_ANY),
    .sin_port = htons(PORT),
  };

  if ((fd_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    return -1;
  }

  setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);

  if (bind(fd_server, (struct sockaddr *)&addr_server, sizeof addr_server) < 0) {
    perror("bind");
    return -2;
  }

  if (listen(fd_server, 50) < 0) {
    perror("listen");
    return -3;
  }

  printf("Listening on port %d\n", PORT);

  while((fd_client = accept(fd_server, NULL, 0))) {
    printf("connection from %i\n", fd_client);
    char buf[32];
    while(recv(fd_client, buf, sizeof buf, 0)) {
      write(fd_client, buf, strlen(buf));
    }
    printf("closing\n");
    close(fd_client);
    printf("closed\n");
  }
  exit(0);
  return 0;
}
