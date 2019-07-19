//http://www.binarytides.com/server-client-example-c-sockets-linux/
//https://gist.github.com/silv3rm00n/5821760
//http://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
//http://stackoverflow.com/questions/13669474/multiclient-server-using-fork
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "ntshell.h"
#define PORT 8000

static int _read(char *buf, int cnt, void *extobj)
{
  const int fd_client = *(int*)extobj;
  return recv(fd_client, buf, cnt, 0);
}

static int _write(const char * buf, const int cnt, void * const extobj)
{
  const int fd_client = *(int*)extobj;
   return write(fd_client, buf, cnt);
}

static int _callback(const char *text, void *extobj)
{
  const int fd_client = *(int*)extobj;
  if (strlen(text) > 0) {
    printf("User input text: %s\r\n", text);
  }
  return 0;
}

int start_shell(int fd_client)
{
  ntshell_t sh;
  ntshell_init(&sh, _read, _write, _callback, &fd_client);
  ntshell_set_prompt(&sh, "Hodor>");
  ntshell_execute(&sh);
  //char buf[32];
  //while(recv(fd_client, buf, sizeof buf, 0)) {
   // write(fd_client, buf, strlen(buf));
  //}
  return 0;
}

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
    start_shell(fd_client);
    printf("closing\n");
    close(fd_client);
    printf("closed\n");
  }
  exit(0);
  return 0;
}
